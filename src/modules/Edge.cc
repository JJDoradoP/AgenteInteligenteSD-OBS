#include <omnetpp.h>
#include "../msgs/Obs_m.h"
#include <map>
#include <deque>
#include <cstring>
#include <algorithm>

using namespace omnetpp;

class Edge : public cSimpleModule {
  private:
    static const int NUM_SVC = 3;

    // ===== Parámetros =====
    int edgeId;
    int dstEdgeId;
    int LmaxTotal;
    simtime_t TmaxTotal;
    simtime_t offsetBase;
    simtime_t ackTimeout;

    // Umbrales por servicio (si quieres mantener el ensamblaje por servicio)
    int LmaxSvc[NUM_SVC];
    simtime_t TmaxSvc[NUM_SVC];

    // ===== Ensamblaje jerárquico: dstEdge -> svc =====
    struct AssemblyUnit {
        std::deque<DataPkt*> q;
        int bytes = 0;
        cMessage *timer = nullptr;      // timer por (dst,svc)
    };

    // assemblies[dstEdge][svc]
    std::map<int, std::map<int, AssemblyUnit>> assemblies;

    // burstId único
    int burstSeq = 0;

    // ===== Espera de ACK =====
    std::map<int, Burst*> pendingBursts;
    std::map<int, cMessage*> ackTimers;

    // ===== Métricas globales =====
    long burstsAssembled = 0, burstsSent = 0, burstsBlocked = 0, burstsRx = 0;
    long bytesAssembled  = 0, bytesSent  = 0, bytesBlocked  = 0, bytesRx  = 0;
    double delayBurstSum = 0.0;
    long delayBurstCount = 0;

    // ===== Helpers: codificar dst+svc en kind =====
    static int packKey(int dst, int svc) { return ((dst & 0xFFFF) << 8) | (svc & 0xFF); }
    static int unpackDst(int key) { return (key >> 8) & 0xFFFF; }
    static int unpackSvc(int key) { return key & 0xFF; }

  protected:
    virtual void initialize() override {
        edgeId     = par("edgeId");
        dstEdgeId  = par("dstEdgeId");
        LmaxTotal  = par("LmaxTotal");
        TmaxTotal  = par("TmaxTotal");
        offsetBase = par("offsetBase");
        ackTimeout = hasPar("ackTimeout") ? (simtime_t)par("ackTimeout") : 0.02;

        for (int s = 0; s < NUM_SVC; ++s) {
            LmaxSvc[s] = LmaxTotal;
            TmaxSvc[s] = TmaxTotal;
        }

        EV_INFO << "[EDGE " << edgeId << "] init(SIN AGENTE) dstEdgeId=" << dstEdgeId
                << " LmaxTotal=" << LmaxTotal
                << " TmaxTotal=" << TmaxTotal
                << " offsetBase=" << offsetBase
                << " ackTimeout=" << ackTimeout << "\n";
    }

    virtual void handleMessage(cMessage *msg) override {

        // =========================================================
        // 1) Timeout de ACK => burst bloqueado
        // =========================================================
        if (msg->isSelfMessage() && strcmp(msg->getName(), "ackTimeout") == 0) {
            int bid = msg->getKind();
            ackTimers.erase(bid);

            auto it = pendingBursts.find(bid);
            if (it != pendingBursts.end()) {
                Burst *b = it->second;
                int svc = b->getSvc();

                burstsBlocked++;
                bytesBlocked += b->getBytes();

                EV_WARN << "[EDGE " << edgeId << "] ACK TIMEOUT -> Burst bloqueado bid="
                        << bid << " bytes=" << b->getBytes() << " svc=" << svc << "\n";

                delete b;
                pendingBursts.erase(it);
            }

            delete msg;
            return;
        }

        // =========================================================
        // 2) Timer de ensamblaje por (dst,svc) => Tmax
        // =========================================================
        if (msg->isSelfMessage() && strcmp(msg->getName(), "assemblyTimer") == 0) {
            int key = msg->getKind();
            int dst = unpackDst(key);
            int svc = unpackSvc(key);

            auto &au = assemblies[dst][svc];
            au.timer = nullptr;

            delete msg;
            assembleBurst(dst, svc, "Tmax");
            return;
        }

        // =========================================================
        // 3) ACK recibido => enviar Burst
        // =========================================================
        if (auto *ack = dynamic_cast<BurstAck*>(msg)) {
            int bid = ack->getBurstId();

            auto itT = ackTimers.find(bid);
            if (itT != ackTimers.end()) {
                cancelAndDelete(itT->second);
                ackTimers.erase(itT);
            }

            auto it = pendingBursts.find(bid);
            if (it != pendingBursts.end()) {
                Burst *b = it->second;
                int svc = b->getSvc();

                b->setSendTime(simTime().dbl());

                burstsSent++;
                bytesSent += b->getBytes();

                EV_INFO << "[EDGE " << edgeId << "] ACK recibido -> enviando Burst#"
                        << bid << " bytes=" << b->getBytes() << " svc=" << svc << "\n";

                send(b, "dataOut");
                pendingBursts.erase(it);
            }

            delete ack;
            return;
        }

        // =========================================================
        // 4) Burst recibido desde core (edge destino) => métricas + desensamble
        // =========================================================
        if (msg->getArrivalGate() && strcmp(msg->getArrivalGate()->getBaseName(), "burstIn") == 0) {
            auto *b = check_and_cast<Burst*>(msg);

            int svc = b->getSvc();

            burstsRx++;
            bytesRx += b->getBytes();

            double d = simTime().dbl() - b->getAssemblyTime();
            delayBurstSum += d;
            delayBurstCount++;

            EV_INFO << "[EDGE " << edgeId << "] Burst recibido bid=" << b->getBurstId()
                    << " bytes=" << b->getBytes()
                    << " svc=" << svc
                    << " delay=" << d << "s\n";

            // Desensamble -> DataPkts al usuario local (si tu NED tiene appOut)
            int pktSize = 500;
            int remaining = b->getBytes();
            while (remaining > 0) {
                int chunk = std::min(remaining, pktSize);
                auto *p = new DataPkt("DataPkt_rx");
                p->setSvc(svc);
                p->setByteLength(chunk);
                send(p, "appOut");
                remaining -= chunk;
            }

            delete b;
            return;
        }

        // =========================================================
        // 5) DataPkt desde usuario => clasificar por destino y servicio
        // =========================================================
        if (msg->getArrivalGate() && strcmp(msg->getArrivalGate()->getBaseName(), "dataIn") == 0) {
            auto *pkt = check_and_cast<DataPkt*>(msg);

            int dst = dstEdgeId;
            int svc = pkt->getSvc();

            AssemblyUnit &au = assemblies[dst][svc];
            au.q.push_back(pkt);
            au.bytes += pkt->getByteLength();

            // primer paquete => arrancar Tmax
            if (au.q.size() == 1 && au.timer == nullptr) {
                au.timer = new cMessage("assemblyTimer");
                au.timer->setKind(packKey(dst, svc));
                scheduleAt(simTime() + TmaxSvc[svc], au.timer);
            }

            // condición Lmax
            if (au.bytes >= LmaxSvc[svc]) {
                if (au.timer && au.timer->isScheduled()) {
                    cancelEvent(au.timer);
                    delete au.timer;
                    au.timer = nullptr;
                }
                assembleBurst(dst, svc, "Lmax");
            }
            return;
        }

        EV_WARN << "[EDGE " << edgeId << "] Mensaje desconocido: " << msg->getName() << "\n";
        delete msg;
    }

    void assembleBurst(int dst, int svc, const char *reason) {
        AssemblyUnit &au = assemblies[dst][svc];
        if (au.q.empty() || au.bytes <= 0) return;

        int bytes = 0;
        int pkts = 0;

        while (!au.q.empty()) {
            DataPkt *p = au.q.front();
            if (strcmp(reason, "Lmax") == 0 &&
                bytes + p->getByteLength() > LmaxSvc[svc])
                break;

            au.q.pop_front();
            bytes += p->getByteLength();
            delete p;
            pkts++;
        }

        if (bytes == 0 && !au.q.empty()) {
            DataPkt *p = au.q.front();
            au.q.pop_front();
            bytes = p->getByteLength();
            delete p;
            pkts = 1;
        }

        au.bytes -= bytes;
        if (au.bytes < 0) au.bytes = 0;

        int burstId = edgeId * 1000000 + (burstSeq++);

        burstsAssembled++;
        bytesAssembled += bytes;

        EV_INFO << "[EDGE " << edgeId << "] Burst ensamblado reason=" << reason
                << " bid=" << burstId << " dst=" << dst << " svc=" << svc
                << " bytes=" << bytes << " -> BCP + esperar ACK\n";

        auto *bcp = new BCP("BCP");
        bcp->setBurstId(burstId);
        bcp->setSrcEdge(edgeId);
        bcp->setDstEdge(dst);
        bcp->setBytes(bytes);
        bcp->setOffset(offsetBase.dbl());
        bcp->setPathLen(0);
        bcp->setHopIndex(0);
        send(bcp, "ctrlOut");

        auto *burst = new Burst("Burst");
        burst->setBurstId(burstId);
        burst->setSrcEdge(edgeId);
        burst->setDstEdge(dst);
        burst->setBytes(bytes);
        burst->setSvc(svc);
        burst->setAssemblyTime(simTime().dbl());
        burst->setSendTime(-1);

        burst->setNVoice((svc==0)?pkts:0);
        burst->setNData((svc==1)?pkts:0);
        burst->setNVideo((svc==2)?pkts:0);

        pendingBursts[burstId] = burst;

        auto *to = new cMessage("ackTimeout");
        to->setKind(burstId);
        scheduleAt(simTime() + ackTimeout, to);
        ackTimers[burstId] = to;

        // si quedan paquetes, rearma timer
        if (!au.q.empty() && au.timer == nullptr) {
            au.timer = new cMessage("assemblyTimer");
            au.timer->setKind(packKey(dst, svc));
            scheduleAt(simTime() + TmaxSvc[svc], au.timer);
        }
    }

    virtual void finish() override {
        // limpiar ensamblajes
        for (auto &d : assemblies) {
            for (auto &s : d.second) {
                auto &au = s.second;
                if (au.timer) {
                    if (au.timer->isScheduled()) cancelEvent(au.timer);
                    delete au.timer;
                    au.timer = nullptr;
                }
                while (!au.q.empty()) {
                    delete au.q.front();
                    au.q.pop_front();
                }
            }
        }
        assemblies.clear();

        for (auto &kv : ackTimers) cancelAndDelete(kv.second);
        ackTimers.clear();

        for (auto &kv : pendingBursts) delete kv.second;
        pendingBursts.clear();

        double PbBurst = (burstsAssembled > 0) ? (double)burstsBlocked / burstsAssembled : 0;
        double PbBytes = (bytesAssembled > 0) ? (double)bytesBlocked / bytesAssembled : 0;
        double thrBps  = (simTime().dbl() > 0) ? (double)bytesRx / simTime().dbl() : 0;
        double avgDelay = (delayBurstCount > 0) ? (delayBurstSum / delayBurstCount) : 0.0;

        EV_INFO << "[SUMMARY][EDGE " << edgeId << "] "
                << "Pb_burst=" << PbBurst
                << " Pb_bytes=" << PbBytes
                << " Throughput_Bps=" << thrBps
                << " AvgDelay_s=" << avgDelay
                << "\n";
    }
};

Define_Module(Edge);
