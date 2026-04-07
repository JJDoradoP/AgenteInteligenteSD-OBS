#include <omnetpp.h>
#include <map>
#include "../msgs/Obs_m.h"

using namespace omnetpp;

class Edge : public cSimpleModule
{
  private:
    int edgeId = -1;
    int dstEdge = -1;

    int pktBytes = 200;
    int LmaxTotal = 24;
    simtime_t TmaxTotal = 0.01;
    simtime_t offsetBase = 0.003;

    double rateVoice = 0;
    double rateData  = 0;
    double rateVideo = 0;

    double baseRateVoice = 0;
    double baseRateData  = 0;
    double baseRateVideo = 0;

    int bufVoice = 0;
    int bufData  = 0;
    int bufVideo = 0;

    bool assembling = false;
    simtime_t assemblyStart = 0;

    cMessage *evVoice = nullptr;
    cMessage *evData  = nullptr;
    cMessage *evVideo = nullptr;
    cMessage *evFlush = nullptr;

    int nextBurstId = 0;

    // ===== Métricas de retardo =====
    cOutVector delayVector;
    long burstsReceived = 0;
    simtime_t totalDelay = 0;
    simtime_t maxDelay = 0;

    std::map<int, Burst*> pendingBursts;

    int lastPhase = -1;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    void scheduleNextArrival(int svc);
    void onPktArrival(int svc);
    void flushBurst(const char *reason);
    void handlePathReply(PathReply *rep);
    void sendEdgeStatus();

    int currentPhase() const;
    double trafficMultiplier() const;
    double currentRateVoice() const;
    double currentRateData() const;
    double currentRateVideo() const;
    void logPhaseIfChanged();

    int totalPkts() const { return bufVoice + bufData + bufVideo; }
};

Define_Module(Edge);

//------------------------------------------------

int Edge::currentPhase() const
{
    simtime_t t = simTime();

    if (t < 0.05)
        return 0;   // base
    else if (t < 0.10)
        return 1;   // pico
    else if (t < 0.15)
        return 2;   // media
    else
        return 3;   // recuperacion
}

double Edge::trafficMultiplier() const
{
    int phase = currentPhase();

    if (phase == 0)
        return 1.0;
    else if (phase == 1)
        return 2.8;
    else if (phase == 2)
        return 1.6;
    else
        return 1.0;
}

double Edge::currentRateVoice() const
{
    return baseRateVoice * trafficMultiplier();
}

double Edge::currentRateData() const
{
    return baseRateData * trafficMultiplier();
}

double Edge::currentRateVideo() const
{
    return baseRateVideo * trafficMultiplier();
}

void Edge::logPhaseIfChanged()
{
    int phase = currentPhase();
    if (phase == lastPhase)
        return;

    lastPhase = phase;

    const char *phaseName = "";
    if (phase == 0) phaseName = "BASE";
    else if (phase == 1) phaseName = "PICO";
    else if (phase == 2) phaseName = "MEDIA";
    else phaseName = "BASE-RECUP";

    EV_INFO << "[EDGE " << edgeId << "] CAMBIO FASE -> " << phaseName
            << " mult=" << trafficMultiplier()
            << " rates(V/D/Vid)="
            << currentRateVoice() << "/"
            << currentRateData()  << "/"
            << currentRateVideo() << "\n";
}

void Edge::sendEdgeStatus()
{
    auto *st = new EdgeStatus("EdgeStatus");
    st->setEdgeId(edgeId);
    st->setTotalBuffered(totalPkts());
    st->setNVoice(bufVoice);
    st->setNData(bufData);
    st->setNVideo(bufVideo);

    EV_INFO << "[EDGE " << edgeId << "] Envio EdgeStatus total=" << totalPkts()
            << " (V/D/Vid)=" << bufVoice << "/" << bufData << "/" << bufVideo << "\n";

    send(st, "ctrlOut");
}

//------------------------------------------------

void Edge::initialize()
{
    edgeId     = par("edgeId");
    dstEdge    = par("dstId");

    pktBytes   = par("pktBytes");
    LmaxTotal  = par("LmaxTotal");
    TmaxTotal  = par("TmaxTotal");
    offsetBase = par("offsetBase");

    rateVoice  = par("rateVoice");
    rateData   = par("rateData");
    rateVideo  = par("rateVideo");

    baseRateVoice = par("rateVoice").doubleValue();
    baseRateData  = par("rateData").doubleValue();
    baseRateVideo = par("rateVideo").doubleValue();

    evVoice = new cMessage("voiceArrival");
    evData  = new cMessage("dataArrival");
    evVideo = new cMessage("videoArrival");
    evFlush = new cMessage("flushTimer");

    delayVector.setName("burstEndToEndDelay");

    logPhaseIfChanged();


    scheduleNextArrival(0);
    scheduleNextArrival(1);
    scheduleNextArrival(2);

    sendEdgeStatus();

    EV_INFO << "[EDGE " << edgeId << "] Inicializado dstEdge=" << dstEdge
            << " LmaxTotal=" << LmaxTotal
            << " TmaxTotal=" << TmaxTotal
            << " offsetBase=" << offsetBase
            << " baseRates(V/D/Vid)=" << baseRateVoice << "/" << baseRateData << "/" << baseRateVideo
            << "\n";
}

//------------------------------------------------

void Edge::scheduleNextArrival(int svc)
{
    logPhaseIfChanged();

    double rate = 0.0;
    cMessage *ev = nullptr;

    if (svc == 0) {
        rate = currentRateVoice();
        ev = evVoice;
    }
    else if (svc == 1) {
        rate = currentRateData();
        ev = evData;
    }
    else {
        rate = currentRateVideo();
        ev = evVideo;
    }

    if (rate <= 0.0)
        return;

    simtime_t dt = exponential(1.0 / rate);
    scheduleAt(simTime() + dt, ev);
}

//------------------------------------------------

void Edge::onPktArrival(int svc)
{
    logPhaseIfChanged();

    if (svc == 0) bufVoice++;
    else if (svc == 1) bufData++;
    else bufVideo++;

    sendEdgeStatus();

    EV_INFO << "[EDGE " << edgeId << "] Llega pkt svc=" << svc
            << " mult=" << trafficMultiplier()
            << " buffer(V/D/Vid)=" << bufVoice << "/" << bufData << "/" << bufVideo
            << " total=" << totalPkts() << "\n";

    if (!assembling) {
        assembling = true;
        assemblyStart = simTime();
        scheduleAt(simTime() + TmaxTotal, evFlush);

        EV_INFO << "[EDGE " << edgeId << "] Inicio ensamblaje: timeout en t="
                << (simTime() + TmaxTotal) << "\n";
    }

    if (totalPkts() >= LmaxTotal) {
        EV_INFO << "[EDGE " << edgeId << "] THRESHOLD: total=" << totalPkts()
                << " >= LmaxTotal=" << LmaxTotal << " -> FLUSH\n";
        cancelEvent(evFlush);
        flushBurst("threshold");
    }
}

//------------------------------------------------

void Edge::flushBurst(const char *reason)
{
    int tot = totalPkts();
    if (tot == 0) {
        assembling = false;
        return;
    }

    simtime_t aDelay = simTime() - assemblyStart;
    int burstId = nextBurstId++;

    auto *burst = new Burst("Burst");
    burst->setBurstId(burstId);
    burst->setSrcEdge(edgeId);
    burst->setDstEdge(dstEdge);

    burst->setNVoice(bufVoice);
    burst->setNData(bufData);
    burst->setNVideo(bufVideo);

    burst->setBytes(tot * pktBytes);

    burst->setHopIndex(0);
    burst->setPathLen(0);
    burst->setPathArraySize(0);

    burst->setCreationTime(simTime());

    pendingBursts[burstId] = burst;

    auto *bcp = new BCP("BCP");
    bcp->setBurstId(burstId);
    bcp->setSrcEdge(edgeId);
    bcp->setDstEdge(dstEdge);
    bcp->setHopIndex(0);
    bcp->setPathLen(0);

    EV_INFO << "[EDGE " << edgeId << "] FLUSH(" << reason << ") burstId=" << burstId
            << " comp(V/D/Vid)=" << bufVoice << "/" << bufData << "/" << bufVideo
            << " bytes=" << (tot * pktBytes)
            << " assemblyDelay=" << aDelay
            << " -> envio BCP y espero PathReply\n";

    send(bcp, "ctrlOut");

    bufVoice = bufData = bufVideo = 0;
    assembling = false;
    sendEdgeStatus();
}

//------------------------------------------------

void Edge::handlePathReply(PathReply *rep)
{
    int burstId = rep->getBurstId();
    auto it = pendingBursts.find(burstId);

    if (it == pendingBursts.end()) {
        EV_WARN << "[EDGE " << edgeId << "] PathReply burstId=" << burstId
                << " pero NO existe burst pendiente -> descarto\n";
        delete rep;
        return;
    }

    Burst *burst = it->second;
    pendingBursts.erase(it);

    int len = rep->getPathLen();
    burst->setPathLen(len);
    burst->setPathArraySize(len);

    EV_INFO << "[EDGE " << edgeId << "] PathReply burstId=" << burstId
            << " pathLen=" << len << " path=";

    for (int i = 0; i < len; ++i) {
        burst->setPath(i, rep->getPath(i));
        EV_INFO << rep->getPath(i) << " ";
    }
    EV_INFO << "\n";

    burst->setHopIndex(0);

    EV_INFO << "[EDGE " << edgeId << "] Envio BURST burstId=" << burstId
            << " offsetBase=" << offsetBase
            << " comp(V/D/Vid)=" << burst->getNVoice() << "/"
                                 << burst->getNData()  << "/"
                                 << burst->getNVideo()
            << " bytes=" << burst->getBytes()
            << "\n";

    sendDelayed(burst, offsetBase, "dataOut");
    delete rep;
}

//------------------------------------------------

void Edge::handleMessage(cMessage *msg)
{
    if (msg == evVoice) { onPktArrival(0); scheduleNextArrival(0); return; }
    if (msg == evData)  { onPktArrival(1); scheduleNextArrival(1); return; }
    if (msg == evVideo) { onPktArrival(2); scheduleNextArrival(2); return; }

    if (msg == evFlush) {
        EV_INFO << "[EDGE " << edgeId << "] TIMEOUT -> FLUSH\n";
        flushBurst("timeout");
        return;
    }

    if (auto *up = dynamic_cast<ParamUpdate*>(msg)) {
        if (up->getEdgeId() == edgeId) {
            LmaxTotal = up->getLmaxTotal();
            TmaxTotal = up->getTmaxTotal();

            EV_INFO << "[EDGE " << edgeId << "] ParamUpdate: LmaxTotal=" << LmaxTotal
                    << " TmaxTotal=" << TmaxTotal << "\n";

            if (assembling) {
                simtime_t newFlushTime = assemblyStart + TmaxTotal;

                cancelEvent(evFlush);

                if (newFlushTime <= simTime()) {
                    EV_INFO << "[EDGE " << edgeId << "] Nuevo TmaxTotal deja flush en el pasado "
                            << "(newFlushTime=" << newFlushTime << ", simTime=" << simTime()
                            << ") -> FLUSH inmediato\n";
                    flushBurst("paramUpdatePastDeadline");
                } else {
                    scheduleAt(newFlushTime, evFlush);
                    EV_INFO << "[EDGE " << edgeId << "] Reprogramo flushTimer a t="
                            << newFlushTime << "\n";
                }
            }
        }
        delete up;
        return;
    }

    if (auto *rep = dynamic_cast<PathReply*>(msg)) {
        handlePathReply(rep);
        return;
    }

    if (auto *burst = dynamic_cast<Burst*>(msg)) {
        simtime_t delay = simTime() - burst->getCreationTime();

        burstsReceived++;
        totalDelay += delay;
        if (delay > maxDelay)
            maxDelay = delay;

        delayVector.record(delay);

        EV_INFO << "[EDGE " << edgeId << "] RECIBO burstId=" << burst->getBurstId()
                << " srcEdge=" << burst->getSrcEdge()
                << " dstEdge=" << burst->getDstEdge()
                << " comp(V/D/Vid)=" << burst->getNVoice() << "/"
                                   << burst->getNData()  << "/"
                                   << burst->getNVideo()
                << " bytes=" << burst->getBytes()
                << " delay=" << delay
                << " (FIN)\n";

        delete burst;
        return;
    }
    EV_WARN << "[EDGE " << edgeId << "] Mensaje desconocido '" << msg->getName()
            << "' -> descarto\n";
    delete msg;
}

//------------------------------------------------

void Edge::finish()
{
    if (burstsReceived > 0) {
        simtime_t avgDelay = totalDelay / burstsReceived;

        EV_INFO << "[EDGE " << edgeId << "] RESUMEN RETARDO: "
                << "burstsReceived=" << burstsReceived
                << " avgDelay=" << avgDelay
                << " maxDelay=" << maxDelay
                << "\n";

        recordScalar("burstsReceived", burstsReceived);
        recordScalar("avgBurstDelay", avgDelay);
        recordScalar("maxBurstDelay", maxDelay);
    }
    else {
        EV_INFO << "[EDGE " << edgeId << "] RESUMEN RETARDO: burstsReceived=0\n";
        recordScalar("burstsReceived", 0);
        recordScalar("avgBurstDelay", 0);
        recordScalar("maxBurstDelay", 0);
    }
    if (evVoice) cancelAndDelete(evVoice);
    if (evData)  cancelAndDelete(evData);
    if (evVideo) cancelAndDelete(evVideo);
    if (evFlush) cancelAndDelete(evFlush);

    for (auto &kv : pendingBursts)
        delete kv.second;
    pendingBursts.clear();

}
