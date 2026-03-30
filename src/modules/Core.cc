#include <omnetpp.h>
#include "../msgs/Obs_m.h"
#include <vector>
#include <map>
#include <algorithm>

using namespace omnetpp;

class Core : public cSimpleModule {
  private:
    int coreId;

    // Reservas por puerto de salida
    std::vector<std::vector<std::pair<simtime_t, simtime_t>>> reserved;

    // burstId -> outPort
    std::map<int, int> burstOutPort;

    static bool overlap(simtime_t a1, simtime_t a2, simtime_t b1, simtime_t b2) {
        return (a1 < b2) && (b1 < a2);
    }

    double getDatarate(int outPort) {
        auto *ch = dynamic_cast<cDatarateChannel*>(gate("dataOut", outPort)->getTransmissionChannel());
        return ch ? ch->getDatarate() : 1e9;
    }

    int findOutPortToCore(int targetCore) {
        for (int k = 0; k < gateSize("dataOut"); ++k) {
            cGate *g = gate("dataOut", k);
            if (!g || !g->isConnected()) continue;
            cModule *nbr = g->getNextGate()->getOwnerModule();
            if (nbr->hasPar("coreId") && nbr->par("coreId").intValue() == targetCore)
                return k;
        }
        return -1;
    }

    int findOutPortToEdge(int dstEdge) {
        for (int k = 0; k < gateSize("dataOut"); ++k) {
            cGate *g = gate("dataOut", k);
            if (!g || !g->isConnected()) continue;
            cModule *nbr = g->getNextGate()->getOwnerModule();
            if (nbr->hasPar("edgeId") && nbr->par("edgeId").intValue() == dstEdge)
                return k;
        }
        return -1;
    }

  protected:
    virtual void initialize() override {
        coreId = par("coreId");
        reserved.resize(gateSize("dataOut"));
        EV_INFO << "[CORE " << coreId << "] Inicializado\n";
    }

    virtual void handleMessage(cMessage *msg) override {

        const char *base = msg->getArrivalGate() ? msg->getArrivalGate()->getBaseName() : "";

        // ===== BCP =====
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

            // ventana temporal: usar burstTime (tiempo real esperado del burst)
            simtime_t tStart = simTime() + bcp->getOffset();
            double txTime = (double)bcp->getBytes() * 8 / getDatarate(outPort);
            simtime_t tEnd = tStart + txTime;

            for (auto &iv : reserved[outPort]) {
                if (overlap(tStart, tEnd, iv.first, iv.second)) {
                    EV_WARN << "[CORE " << coreId << "] Colisión temporal. DROP BCP burstId="
                            << bcp->getBurstId() << "\n";
                    delete bcp;
                    return;
                }
            }

            reserved[outPort].push_back({tStart, tEnd});
            burstOutPort[bcp->getBurstId()] = outPort;

            EV_INFO << "[CORE " << coreId << "] Reserva OK burstId=" << bcp->getBurstId()
                    << " outPort=" << outPort << " [" << tStart << "," << tEnd << ")\n";

            // reenviar BCP al controlador para el siguiente hop
            bcp->setHopIndex(hop + 1);
            send(bcp, "ctrlOut");
            return;
        }

        // ===== BURST =====
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
            EV_INFO << "[CORE " << coreId << "] FWD Burst#" << burstId << " -> dataOut[" << outPort << "]\n";
            send(burst, "dataOut", outPort);

            burstOutPort.erase(it);
            return;
        }

        EV_WARN << "[CORE " << coreId << "] Mensaje desconocido: " << msg->getName() << "\n";
        delete msg;


    }

    virtual void finish() override {
        burstOutPort.clear();
        for (auto &v : reserved) v.clear();
    }
};

Define_Module(Core);
