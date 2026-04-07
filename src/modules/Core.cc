#include <omnetpp.h>
#include <queue>
#include <unordered_map>
#include "../msgs/Obs_m.h"

using namespace omnetpp;

class Core : public cSimpleModule
{
  private:
    int coreId;

    std::queue<Burst*> burstQueue;
    cMessage *serveEvent = nullptr;

    simtime_t serviceTime;
    simtime_t statusPeriod;

    // neighborCoreId -> gateIndex en dataOut[]
    std::unordered_map<int,int> outCoreGateIndex;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    void handleBCP(BCP *bcp);
    void handleBurst(Burst *burst);
    void sendStatus();
    void serveNextBurst();

    int findOutGateToCore(int neighborCoreId) const;
};

Define_Module(Core);

//------------------------------------------------

void Core::initialize()
{
    coreId = par("coreId");
    serviceTime = par("serviceTime");
    statusPeriod = par("statusPeriod");

    serveEvent = new cMessage("serveEvent");

    // Construir mapa de salida hacia cores vecinos usando conexiones reales
    for (int k = 0; k < gateSize("dataOut"); ++k) {
        cGate *g = gate("dataOut", k);
        if (!g->isConnected()) continue;

        cGate *next = g->getNextGate();
        if (!next) continue;

        cModule *destMod = next->getOwnerModule();
        if (!destMod) continue;

        const char *nedType = destMod->getNedTypeName();
        if (nedType && strcmp(nedType, "src.modules.Core") == 0) {
            int neighId = (int)destMod->par("coreId").intValue();
            outCoreGateIndex[neighId] = k;

            EV_INFO << "[CORE " << coreId << "] MAP: dataOut[" << k << "] -> core "
                    << neighId << "\n";
        } else {
            EV_INFO << "[CORE " << coreId << "] MAP: dataOut[" << k << "] -> (no-core) "
                    << (nedType ? nedType : "unknown") << "\n";
        }
    }

    sendStatus();

    EV_INFO << "[CORE " << coreId << "] Inicializado serviceTime=" << serviceTime
            << " statusPeriod=" << statusPeriod << "\n";
}

//------------------------------------------------

void Core::handleMessage(cMessage *msg)
{
    if (msg == serveEvent) {
        EV_INFO << "[CORE " << coreId << "] Evento interno serveEvent\n";
        serveNextBurst();
        return;
    }

    if (auto *bcp = dynamic_cast<BCP*>(msg)) {
        handleBCP(bcp);
        return;
    }

    if (auto *burst = dynamic_cast<Burst*>(msg)) {
        handleBurst(burst);
        return;
    }

    EV_WARN << "[CORE " << coreId << "] Mensaje desconocido '" << msg->getName()
            << "', se descarta.\n";
    delete msg;
}

//------------------------------------------------

void Core::handleBCP(BCP *bcp)
{
    int hop = bcp->getHopIndex();
    int len = bcp->getPathLen();

    EV_INFO << "[CORE " << coreId << "] BCP recibido burstId=" << bcp->getBurstId()
            << " hop=" << hop << "/" << len << "\n";

    // Incrementar hopIndex y devolver al controlador (ctrlOut[0])
    bcp->setHopIndex(hop + 1);

    EV_INFO << "[CORE " << coreId << "] -> devuelvo BCP burstId=" << bcp->getBurstId()
            << " al controlador con hop=" << bcp->getHopIndex() << "\n";
    send(bcp, "ctrlOut", 0);
}

//------------------------------------------------

void Core::handleBurst(Burst *burst)
{
    EV_INFO << "[CORE " << coreId << "] RECIBO BURST burstId=" << burst->getBurstId()
            << " srcEdge=" << burst->getSrcEdge()
            << " dstEdge=" << burst->getDstEdge()
            << " hopIndex=" << burst->getHopIndex()
            << " pathLen=" << burst->getPathLen()
            << "\n";

    burstQueue.push(burst);

    if (!serveEvent->isScheduled()) {
        EV_INFO << "[CORE " << coreId << "] -> programo serveEvent en t="
                << (simTime() + serviceTime) << "\n";
        scheduleAt(simTime() + serviceTime, serveEvent);
    }

    sendStatus();
}

//------------------------------------------------

int Core::findOutGateToCore(int neighborCoreId) const
{
    auto it = outCoreGateIndex.find(neighborCoreId);
    if (it == outCoreGateIndex.end())
        return -1;
    return it->second;
}

//------------------------------------------------

void Core::serveNextBurst()
{
    if (burstQueue.empty()) {
        EV_INFO << "[CORE " << coreId << "] Cola vacia, nada que servir\n";
        return;
    }

    Burst *burst = burstQueue.front();
    burstQueue.pop();

    int hop = burst->getHopIndex();
    int len = burst->getPathLen();

    EV_INFO << "[CORE " << coreId << "] SIRVO burstId=" << burst->getBurstId()
            << " hop=" << hop << "/" << len << "\n";

    // avanzamos hop
    burst->setHopIndex(hop + 1);

    // ¿Quedan cores?
    if (hop + 1 < len) {
        int nextCore = burst->getPath(hop + 1);
        int gateIdx = findOutGateToCore(nextCore);

        if (gateIdx < 0) {
            EV_ERROR << "[CORE " << coreId << "] No hay enlace de datos hacia core "
                     << nextCore << ". Descargo burstId=" << burst->getBurstId() << "\n";
            delete burst;
        } else {
            EV_INFO << "[CORE " << coreId << "] REENVIO burstId=" << burst->getBurstId()
                    << " -> core " << nextCore
                    << " por dataOut[" << gateIdx << "]\n";
            send(burst, "dataOut", gateIdx);
        }
    }
    else {
        // Último core → salida hacia Edge (en tu topología, 1 edge por core)
        int sent = 0;
        for (int k = 0; k < gateSize("dataOut"); ++k) {
            cGate *g = gate("dataOut", k);
            if (!g->isConnected()) continue;

            cGate *next = g->getNextGate();
            if (!next) continue;

            cModule *destMod = next->getOwnerModule();
            if (!destMod) continue;

            const char *nedType = destMod->getNedTypeName();
            // si NO es core, asumimos edge
            if (!(nedType && strcmp(nedType, "src.modules.Core") == 0)) {
                EV_INFO << "[CORE " << coreId << "] SALIDA A EDGE burstId=" << burst->getBurstId()
                        << " dstEdge=" << burst->getDstEdge()
                        << " por dataOut[" << k << "]\n";
                send(burst, "dataOut", k);
                sent = 1;
                break;
            }
        }

        if (!sent) {
            EV_ERROR << "[CORE " << coreId << "] No encontre salida hacia Edge. Descargo burstId="
                     << burst->getBurstId() << "\n";
            delete burst;
        }
    }

    // Programar siguiente si hay más
    if (!burstQueue.empty()) {
        EV_INFO << "[CORE " << coreId << "] Aun hay bursts en cola -> reprogramo serveEvent\n";
        scheduleAt(simTime() + serviceTime, serveEvent);
    }

    sendStatus();
}

//------------------------------------------------

void Core::sendStatus()
{
    auto *st = new CoreStatus("CoreStatus");
    st->setCoreId(coreId);
    st->setQueuedBursts((int)burstQueue.size());
    st->setEstDelay(0);

    EV_INFO << "[CORE " << coreId << "] Envio CoreStatus queuedBursts="
            << (int)burstQueue.size() << "\n";

    // cada core solo tiene ctrlOut[0] conectado al controller
    send(st, "ctrlOut", 0);
}
