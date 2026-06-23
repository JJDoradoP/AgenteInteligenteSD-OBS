#include <omnetpp.h>
#include "../msgs/Obs_m.h"
#include <vector>
#include <map>
#include <algorithm>

using namespace omnetpp;

class Core : public cSimpleModule {
  private:
    int coreId;

    std::map<int, int> burstOutPort;

    int findOutPortToCore(int targetCore) {
        for (int k = 0; k < gateSize("dataOut"); ++k) {
            cGate *g = gate("dataOut", k);
            if (!g || !g->isConnected()) continue;
            cModule *nbr = g->getNextGate()->getOwnerModule();
            if (nbr->hasPar("coreId") && nbr->par("coreId").intValue() == targetCore)
                return k; // Retornará el índice base (0, 5, o 10)
        }
        return -1;
    }

    int findOutPortToEdge(int dstEdge) {
        for (int k = 0; k < gateSize("dataOut"); ++k) {
            cGate *g = gate("dataOut", k);
            if (!g || !g->isConnected()) continue;
            cModule *nbr = g->getNextGate()->getOwnerModule();
            if (nbr->hasPar("edgeId") && nbr->par("edgeId").intValue() == dstEdge)
                return k; // Retornará el índice base (0, 5, o 10)
        }
        return -1;
    }

  protected:
    virtual void initialize() override {
        coreId = par("coreId");
        EV_INFO << "[CORE " << coreId << "] Inicializado en modo JET DWDM (5 Lambdas)\n";
    }

    virtual void handleMessage(cMessage *msg) override {

        const char *base = msg->getArrivalGate() ? msg->getArrivalGate()->getBaseName() : "";

        // ===== BCP (Plano de Control - Se mantiene igual) =====
        if (auto *bcp = dynamic_cast<BCP*>(msg)) {
            int hop = bcp->getHopIndex();
            int len = bcp->getPathLen();

            if (hop >= len || bcp->getPath(hop) != coreId) {
                EV_WARN << "[CORE " << coreId << "] BCP inválido. DROP.\n";
                delete bcp;
                return;
            }

            int outPort = -1;

            if (hop == len - 1) {
                outPort = findOutPortToEdge(bcp->getDstEdge());
            } else {
                int nextCore = bcp->getPath(hop + 1);
                outPort = findOutPortToCore(nextCore);
            }

            if (outPort < 0) {
                EV_WARN << "[CORE " << coreId << "] No hay puerto de salida. DROP BCP.\n";
                delete bcp;
                return;
            }

            // Guardamos el índice base del enlace hacia donde debe ir la ráfaga
            burstOutPort[bcp->getBurstId()] = outPort;

            // reenviar BCP al controlador para el siguiente hop
            bcp->setHopIndex(hop + 1);
            send(bcp, "ctrlOut");
            return;
        }

        // ===== BURST (Plano de Datos Óptico - Modificado para 5 Lambdas) =====
        if (strcmp(base, "dataIn") == 0) {
            auto *burst = check_and_cast<Burst*>(msg);
            int burstId = burst->getBurstId();

            auto it = burstOutPort.find(burstId);
            if (it == burstOutPort.end()) {
                EV_WARN << "[CORE " << coreId << "] Burst sin reserva. DROP burstId=" << burstId << "\n";
                delete burst;
                return;
            }

            int outPort = it->second;
            burstOutPort.erase(it);

            // 1. Calculamos matemáticamente la "puerta base" (0, 5 o 10)
            // Esto asegura que miremos solo el grupo de 5 cables correcto
            int baseIndex = (outPort / 5) * 5;
            int freeLambda = -1;

            // 2. ALGORITMO FIRST-FIT: Barrido de los 5 canales de este enlace
            for (int i = 0; i < 5; i++) {
                cGate *outGate = gate("dataOut", baseIndex + i);
                cDatarateChannel *txCh = dynamic_cast<cDatarateChannel*>(outGate->getTransmissionChannel());

                if (txCh && !txCh->isBusy()) {
                    freeLambda = baseIndex + i; // ¡Encontramos un color (lambda) libre!
                    break;
                }
            }

            // 3. ¿Qué pasa si la autopista de 5 carriles está llena?
            if (freeLambda == -1) {
                EV_WARN << "[CORE " << coreId << "] COLISIÓN ÓPTICA MASIVA (5 lambdas ocupadas). DROP BurstId=" << burstId << "\n";

                // Avisar al Edge que hubo colisión para que el Agente RL sea castigado
                auto *nack = new BurstNack("BurstNack");
                nack->setBurstId(burstId);
                nack->setSvc(burst->getSvc());
                nack->setSrcEdge(burst->getSrcEdge());
                nack->setCoreId(coreId);   // P1-A: informar qué Core bloqueó
                send(nack, "ctrlOut");
                delete burst;
                return;
            }

            // 4. Si hay espacio, la luz viaja por la lambda libre
            send(burst, "dataOut", freeLambda);
            return;
        }

        EV_WARN << "[CORE " << coreId << "] Mensaje desconocido: " << msg->getName() << "\n";
        delete msg;
    }

    virtual void finish() override {
        // OMNeT++ se encarga de cancelar eventos.
    }
};

Define_Module(Core);
