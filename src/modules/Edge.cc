#include <omnetpp.h>
#include "../msgs/Obs_m.h"
#include <map>
#include <deque>
#include <cstring>
#include <algorithm>

using namespace omnetpp;

namespace net_stats {
    static long globalAssembled = 0;
    static long globalBlocked = 0;
    static long globalRx = 0;
    static int edgesFinished = 0;
    static int totalEdges = 0;
    static bool initialized = false;
}

class Edge : public cSimpleModule {
  private:
    static const int NUM_SVC = 3;

    int edgeId;
    int dstEdgeId;
    int LmaxTotal;
    simtime_t TmaxTotal;

    // NUEVA VARIABLE: Tiempo de procesamiento por nodo Core
    simtime_t coreProcessingTime;

    simtime_t ackTimeout;

    simtime_t reportInterval;
    cMessage *reportTimer = nullptr;

    double winDelaySum[NUM_SVC]{};
    long winDelayCount[NUM_SVC]{};

    long winPktsIn[NUM_SVC]{};
    long winBytesIn[NUM_SVC]{};
    long winBurstsAssembled[NUM_SVC]{};
    long winBurstsBlocked[NUM_SVC]{};

    int LmaxSvc[NUM_SVC];
    simtime_t TmaxSvc[NUM_SVC];

    struct AssemblyUnit {
        int bytes = 0;
        int pkts = 0; // Contador lógico
        cMessage *timer = nullptr;
    };

    std::map<int, std::map<int, AssemblyUnit>> assemblies;

    int burstSeq = 0;

    long burstsAssembled = 0, burstsSent = 0, burstsBlocked = 0, burstsRx = 0;
    int64_t bytesAssembled = 0, bytesSent = 0, bytesBlocked = 0, bytesRx = 0;

    // ===== Métricas por servicio =====
    long burstsAssembledSvc[NUM_SVC]{};
    long burstsBlockedSvc[NUM_SVC]{};
    long burstsRxSvc[NUM_SVC]{};

    int64_t bytesAssembledSvc[NUM_SVC]{};
    int64_t bytesBlockedSvc[NUM_SVC]{};
    int64_t bytesRxSvc[NUM_SVC]{};

    double delayBurstSum = 0.0;
    long delayBurstCount = 0;

    double delayBurstSumSvc[NUM_SVC]{};
    long delayBurstCountSvc[NUM_SVC]{};

    std::deque<Burst*> txQueue;
    cMessage *txTimer = nullptr;

    static int packKey(int dst, int svc) { return ((dst & 0xFFFF) << 8) | (svc & 0xFF); }
    static int unpackDst(int key) { return (key >> 8) & 0xFFFF; }
    static int unpackSvc(int key) { return key & 0xFF; }

    enum TrafficClass {
        TC_EF = 0,
        TC_AF = 1,
        TC_BE = 2
    };

    int svcFromClass(int tc) const {
        if (tc == TC_EF) return 0; // voz
        if (tc == TC_AF) return 2; // video
        return 1;                  // datos
    }

    int classFromDscp(int dscp) const {
        if (dscp == 46) return TC_EF; // EF
        if (dscp == 34) return TC_AF; // AF
        return TC_BE;                 // BE
    }

    void assembleBurst(int dst, int tc, const char *reason) {

        AssemblyUnit &au = assemblies[dst][tc];
        if (au.bytes <= 0) return;

        int bytes = au.bytes;
        int pkts = au.pkts;

        int svc;
        if (tc == TC_EF) svc = 0;
        else if (tc == TC_AF) svc = 2;
        else svc = 1;

        // Si es por Lmax, cortamos exacto en el límite
        if (strcmp(reason, "Lmax") == 0 && bytes > LmaxSvc[svc]) {
            bytes = LmaxSvc[svc];
            // Calculamos proporcionalmente cuántos paquetes lógicos se van
            int pktsToSend = std::max(1, (int)((double)pkts * ((double)bytes / au.bytes)));
            au.pkts -= pktsToSend;
            pkts = pktsToSend;
        } else {
            // Si es por Tmax, vaciamos todo
            au.pkts = 0;
        }

        au.bytes -= bytes;
        if (au.bytes < 0) au.bytes = 0;
        if (au.pkts < 0) au.pkts = 0;

        int burstId = edgeId * 1000000 + (burstSeq++);
        burstsAssembled++;
        bytesAssembled += bytes;
        winBurstsAssembled[svc]++;

        // métricas por servicio
        burstsAssembledSvc[svc]++;
        bytesAssembledSvc[svc] += bytes;

        EV_INFO << "[EDGE " << edgeId << "] Burst ensamblado reason=" << reason
                << " bid=" << burstId << " dst=" << dst << " svc=" << svc
                << " bytes=" << bytes << " -> BCP + esperar ACK\n";

        // --- CÁLCULO DEL OFFSET DINÁMICO ---
        // En topología anillo, la distancia al destino es de 2 Cores.
        int H = 2;
        simtime_t dynamicOffset = coreProcessingTime * H;
        // -----------------------------------

        auto *bcp = new BCP("BCP");
        bcp->setBurstId(burstId);
        bcp->setSrcEdge(edgeId);
        bcp->setDstEdge(dst);
        bcp->setBytes(bytes);
        bcp->setOffset(dynamicOffset.dbl()); // <-- ASIGNAMOS EL OFFSET DINÁMICO
        bcp->setPathLen(0);
        bcp->setHopIndex(0);
        send(bcp, "ctrlOut");

        auto *burst = new Burst("Burst");
        burst->setBurstId(burstId);
        burst->setSrcEdge(edgeId);
        burst->setDstEdge(dst);
        burst->setBytes(bytes);
        burst->setByteLength(bytes); // <-- CRITICAL: OMNeT++ necesita esto para ocupar el canal
        burst->setSvc(svc);
        burst->setAssemblyTime(simTime().dbl());
        burst->setSendTime(-1);

        burst->setNVoice((svc == 0) ? pkts : 0);
        burst->setNData((svc == 1) ? pkts : 0);
        burst->setNVideo((svc == 2) ? pkts : 0);

        // JET: Esperar el tiempo calculado dinámicamente y enviar ciegamente
        auto *offsetT = new cMessage("offsetTimer");
        offsetT->setContextPointer(burst);
        scheduleAt(simTime() + dynamicOffset, offsetT); // <-- ESPERAMOS EL OFFSET DINÁMICO

        if (au.pkts > 0 && au.timer == nullptr) {
            au.timer = new cMessage("assemblyTimer");
            au.timer->setKind(packKey(dst, tc));
            scheduleAt(simTime() + TmaxSvc[svc], au.timer);
        }
    }

    void sendTrafficReports() {
        for (int svc = 0; svc < NUM_SVC; ++svc) {
            int tc = (svc == 0) ? TC_EF : (svc == 2 ? TC_AF : TC_BE);
            long qPkts = 0;
            long qBytes = 0;
            for (auto const& dstPair : assemblies) {
                if (dstPair.second.count(tc)) {
                    qPkts += dstPair.second.at(tc).pkts;
                    qBytes += dstPair.second.at(tc).bytes;
                }
            }

            auto *rep = new TrafficReport("TrafficReport");
            rep->setEdgeId(edgeId);
            rep->setSvc(svc);
            rep->setInterval(reportInterval);
            rep->setPktsIn(winPktsIn[svc]);
            rep->setBytesIn(winBytesIn[svc]);
            rep->setBurstsAssembled(winBurstsAssembled[svc]);
            rep->setBurstsBlocked(winBurstsBlocked[svc]);
            rep->setQueuePkts(qPkts);
            rep->setQueueBytes(qBytes);

            double intervalDelay = (winDelayCount[svc] > 0) ? (winDelaySum[svc] / (double)winDelayCount[svc]) : 0.0;
            rep->setAvgDelayWindow(intervalDelay);

            // Reiniciar contadores para la siguiente ventana
            winDelaySum[svc] = 0.0;
            winDelayCount[svc] = 0;
            rep->setCurrLmax(LmaxSvc[svc]);
            rep->setCurrTmax(TmaxSvc[svc]);

            send(rep, "ctrlOut");

            winPktsIn[svc] = 0;
            winBytesIn[svc] = 0;
            winBurstsAssembled[svc] = 0;
            winBurstsBlocked[svc] = 0;
        }
    }

    void applyBurstifierConfig(int svc, int newL, simtime_t newT) {
        if (svc < 0 || svc >= NUM_SVC) return;

        LmaxSvc[svc] = newL;
        TmaxSvc[svc] = newT;

        EV_INFO << "[EDGE " << edgeId << "] Nueva config svc=" << svc
                << " Lmax=" << newL
                << " Tmax=" << newT << "\n";
    }

  protected:
    virtual void initialize() override {
        if (!net_stats::initialized || net_stats::edgesFinished == net_stats::totalEdges) {
            net_stats::globalAssembled = 0;
            net_stats::globalBlocked = 0;
            net_stats::globalRx = 0;
            net_stats::edgesFinished = 0;
            net_stats::totalEdges = 0;
            net_stats::initialized = true;
        }
        net_stats::totalEdges++;

        edgeId     = par("edgeId");
        dstEdgeId  = par("dstEdgeId");
        LmaxTotal  = par("LmaxTotal");
        TmaxTotal  = par("TmaxTotal");

        // LECTURA DEL NUEVO PARÁMETRO
        coreProcessingTime = par("coreProcessingTime");

        ackTimeout = hasPar("ackTimeout") ? (simtime_t)par("ackTimeout") : 0.02;

        for (int s = 0; s < NUM_SVC; ++s) {
            LmaxSvc[0] = LmaxTotal / 2;   // EF/voz
            TmaxSvc[0] = TmaxTotal / 2;

            LmaxSvc[1] = LmaxTotal;       // BE/datos
            TmaxSvc[1] = TmaxTotal;

            LmaxSvc[2] = (int)(LmaxTotal * 0.75); // AF/video
            TmaxSvc[2] = TmaxTotal * 0.75;
        }

        EV_INFO << "[EDGE " << edgeId << "] init dstEdgeId=" << dstEdgeId
                << " LmaxTotal=" << LmaxTotal
                << " TmaxTotal=" << TmaxTotal
                << " coreProcessingTime=" << coreProcessingTime // <-- LOG ACTUALIZADO
                << " ackTimeout=" << ackTimeout << "\n";

        reportInterval = hasPar("reportInterval") ? (simtime_t)par("reportInterval") : 0.05;
        reportTimer = new cMessage("reportTimer");
        scheduleAt(simTime() + reportInterval, reportTimer);
        txTimer = new cMessage("txTimer");
    }

    virtual void handleMessage(cMessage *msg) override {
        if (msg->isSelfMessage() && strcmp(msg->getName(), "reportTimer") == 0) {
            sendTrafficReports();
            scheduleAt(simTime() + reportInterval, reportTimer);
            return;
        }

        if (msg->isSelfMessage() && strcmp(msg->getName(), "offsetTimer") == 0) {
            Burst *b = static_cast<Burst*>(msg->getContextPointer());
            if (b) {
                txQueue.push_back(b);
                if (!txTimer->isScheduled()) {
                    scheduleAt(simTime(), txTimer);
                }
            }
            delete msg;
            return;
        }

        if (auto *nack = dynamic_cast<BurstNack*>(msg)) {
            int svc = nack->getSvc();
            burstsBlocked++;
            bytesBlocked += LmaxSvc[svc];
            winBurstsBlocked[svc]++;
            burstsBlockedSvc[svc]++;
            bytesBlockedSvc[svc] += LmaxSvc[svc];
            delete msg;
            return;
        }

        // === 1. BLOQUE TX TIMER (Algoritmo First-Fit 5 Lambdas) ===
        if (msg == txTimer) {
            if (txQueue.empty()) return;
            Burst *b = txQueue.front();

            int freeLambda = -1;
            simtime_t earliestFree = -1;

            for (int i = 0; i < 5; i++) {
                cGate *outGate = gate("dataOut", i);
                cDatarateChannel *txCh = dynamic_cast<cDatarateChannel*>(outGate->getTransmissionChannel());

                if (txCh) {
                    if (!txCh->isBusy()) {
                        freeLambda = i;
                        break;
                    } else {
                        simtime_t freeAt = txCh->getTransmissionFinishTime();
                        if (earliestFree < 0 || freeAt < earliestFree) {
                            earliestFree = freeAt;
                        }
                    }
                }
            }

            if (freeLambda == -1) {
                if (earliestFree <= simTime()) earliestFree = simTime() + SimTime(1, SIMTIME_US);
                scheduleAt(earliestFree, txTimer);
                return;
            }

            txQueue.pop_front();
            int svc = b->getSvc();
            burstsSent++;
            long burstBytes = b->getBytes();
            bytesSent += burstBytes;
            b->setSendTime(simTime().dbl());

            send(b, "dataOut", freeLambda);

            if (!txQueue.empty()) {
                scheduleAt(simTime(), txTimer);
            }
            return;
        }

        if (msg->isSelfMessage() && strcmp(msg->getName(), "assemblyTimer") == 0) {
            int key = msg->getKind();
            int dst = unpackDst(key);
            int tc = unpackSvc(key);

            auto &au = assemblies[dst][tc];
            au.timer = nullptr;

            delete msg;
            assembleBurst(dst, tc, "Tmax");
            return;
        }

        // === 2. RECEPCIÓN DE PAQUETES ===
        if (msg->getArrivalGate() && strcmp(msg->getArrivalGate()->getBaseName(), "dataIn") == 0) {
            auto *pkt = check_and_cast<DataPkt*>(msg);

            int dst = intuniform(0, 2);
            while (dst == edgeId) dst = intuniform(0, 2);
            int svc = pkt->getSvc();
            int tc = classFromDscp(pkt->getDscp());

            winPktsIn[svc]++;
            winBytesIn[svc] += pkt->getByteLength();

            AssemblyUnit &au = assemblies[dst][tc];
            au.bytes += pkt->getByteLength();
            au.pkts++;
            delete pkt; // Destruimos el paquete para salvar la RAM

            if (au.pkts == 1 && au.timer == nullptr) {
                au.timer = new cMessage("assemblyTimer");
                au.timer->setKind(packKey(dst, tc));
                scheduleAt(simTime() + TmaxSvc[svc], au.timer);
            }

            if (au.bytes >= LmaxSvc[svc]) {
                if (au.timer && au.timer->isScheduled()) {
                    cancelEvent(au.timer);
                    delete au.timer;
                    au.timer = nullptr;
                }
                assembleBurst(dst, tc, "Lmax");
            }
            return;
        }

        // === 3. RECEPCIÓN DE RÁFAGAS ===
        if (auto *b = dynamic_cast<Burst*>(msg)) {
            int svc = b->getSvc();
            long burstBytes = b->getBytes();

            double retardo = (simTime() - b->getAssemblyTime()).dbl();

            delayBurstSum += retardo;
            delayBurstCount++;
            delayBurstSumSvc[svc] += retardo;
            delayBurstCountSvc[svc]++;

            burstsRx++;
            bytesRx += burstBytes;
            burstsRxSvc[svc]++;
            bytesRxSvc[svc] += burstBytes;

            auto *p = new DataPkt("DataPkt_rx");
            p->setSvc(svc);
            p->setByteLength(burstBytes);
            send(p, "appOut");

            delete b;
            return;
        }

        if (auto *cfg = dynamic_cast<BurstifierConfig*>(msg)) {
            if (cfg->getEdgeId() == edgeId) {
                applyBurstifierConfig(cfg->getSvc(), cfg->getNewLmax(), cfg->getNewTmax());
            }
            delete cfg;
            return;
        }

        EV_WARN << "[EDGE " << edgeId << "] Mensaje desconocido: " << msg->getName() << "\n";
        delete msg;
    }

    virtual void finish() override {
        double PbBurst = (burstsAssembled > 0) ? (double)burstsBlocked / (double)burstsAssembled : 0.0;
        double PbBytes = (bytesAssembled > 0) ? (double)bytesBlocked / (double)bytesAssembled : 0.0;
        double thrBytesps = (simTime().dbl() > 0) ? (double)bytesRx / simTime().dbl() : 0.0;
        double thrbps = (simTime().dbl() > 0) ? ((double)bytesRx * 8.0) / simTime().dbl() : 0.0;
        double avgDelay = (delayBurstCount > 0) ? (delayBurstSum / (double)delayBurstCount) : 0.0;

        // métricas globales
        recordScalar("burstsAssembled", burstsAssembled);
        recordScalar("burstsBlocked", burstsBlocked);
        recordScalar("burstsRx", burstsRx);

        recordScalar("bytesAssembled", bytesAssembled);
        recordScalar("bytesBlocked", bytesBlocked);
        recordScalar("bytesRx", bytesRx);

        recordScalar("Pb_burst", PbBurst);
        recordScalar("Pb_bytes", PbBytes);
        recordScalar("Throughput_Bytesps", thrBytesps);
        recordScalar("Throughput_bps", thrbps);
        recordScalar("AvgDelay_s", avgDelay);

        EV_INFO << "[SUMMARY][EDGE " << edgeId << "] "
                << "Bursts_Created=" << burstsAssembled
                << " Bursts_Received=" << burstsRx
                << " Pb_burst=" << PbBurst
                << " Pb_bytes=" << PbBytes
                << " Throughput_Bytesps=" << thrBytesps
                << " Throughput_bps=" << thrbps
                << " AvgDelay_s=" << avgDelay
                << "\n";

        // métricas por servicio
        for (int s = 0; s < NUM_SVC; ++s) {
            double PbBurstSvc = (burstsAssembledSvc[s] > 0) ?
                (double)burstsBlockedSvc[s] / (double)burstsAssembledSvc[s] : 0.0;

            double PbBytesSvc = (bytesAssembledSvc[s] > 0) ?
                (double)bytesBlockedSvc[s] / (double)bytesAssembledSvc[s] : 0.0;

            double thrBytespsSvc = (simTime().dbl() > 0) ?
                (double)bytesRxSvc[s] / simTime().dbl() : 0.0;

            double thrbpsSvc = (simTime().dbl() > 0) ?
                ((double)bytesRxSvc[s] * 8.0) / simTime().dbl() : 0.0;

            double avgDelaySvc = (delayBurstCountSvc[s] > 0) ?
                delayBurstSumSvc[s] / (double)delayBurstCountSvc[s] : 0.0;

            std::string suffix;
            if (s == 0) suffix = "voz";
            else if (s == 1) suffix = "datos";
            else if (s == 2) suffix = "video";
            else suffix = "unk";

            recordScalar(("Pb_burst_" + suffix).c_str(), PbBurstSvc);
            recordScalar(("Pb_bytes_" + suffix).c_str(), PbBytesSvc);
            recordScalar(("Throughput_Bytesps_" + suffix).c_str(), thrBytespsSvc);
            recordScalar(("Throughput_bps_" + suffix).c_str(), thrbpsSvc);
            recordScalar(("AvgDelay_s_" + suffix).c_str(), avgDelaySvc);

            recordScalar(("burstsAssembled_" + suffix).c_str(), burstsAssembledSvc[s]);
            recordScalar(("burstsBlocked_" + suffix).c_str(), burstsBlockedSvc[s]);
            recordScalar(("burstsRx_" + suffix).c_str(), burstsRxSvc[s]);

            recordScalar(("bytesAssembled_" + suffix).c_str(), bytesAssembledSvc[s]);
            recordScalar(("bytesBlocked_" + suffix).c_str(), bytesBlockedSvc[s]);
            recordScalar(("bytesRx_" + suffix).c_str(), bytesRxSvc[s]);

            EV_INFO << "[SUMMARY][EDGE " << edgeId << "][SVC " << suffix << "] "
                    << "Bursts_Created=" << burstsAssembledSvc[s]
                    << " Bursts_Received=" << burstsRxSvc[s]
                    << " Pb_burst=" << PbBurstSvc
                    << " Pb_bytes=" << PbBytesSvc
                    << " Throughput_Bytesps=" << thrBytespsSvc
                    << " Throughput_bps=" << thrbpsSvc
                    << " AvgDelay_s=" << avgDelaySvc
                    << "\n";
        }

        net_stats::globalAssembled += burstsAssembled;
        net_stats::globalBlocked += burstsBlocked;
        net_stats::globalRx += burstsRx;
        net_stats::edgesFinished++;

        if (net_stats::edgesFinished == net_stats::totalEdges) {
            EV_INFO << "\n======================================================\n";
            EV_INFO << "      [GLOBAL NETWORK SUMMARY - OBS JET MODE]         \n";
            EV_INFO << "======================================================\n";
            EV_INFO << " Total Ráfagas Creadas (Enviadas) : " << net_stats::globalAssembled << "\n";
            EV_INFO << " Total Ráfagas Exitosas (Llegaron) : " << net_stats::globalRx << "\n";
            EV_INFO << " Total Ráfagas Bloqueadas (Caídas) : " << net_stats::globalBlocked << "\n";
            if (net_stats::globalAssembled > 0) {
                EV_INFO << " Probabilidad de Bloqueo (Global) : " << (double)net_stats::globalBlocked / net_stats::globalAssembled << "\n";
            }
            EV_INFO << "======================================================\n\n";
        }
    }
};

Define_Module(Edge);
