#include <omnetpp.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <chrono>
#include <thread>

#include "../msgs/Obs_m.h"

using namespace omnetpp;

class SDController : public cSimpleModule {
  private:
    static const int NUM_SVC = 3;

    struct AppliedCfg {
        bool valid = false;
        int L = 0;
        simtime_t T = 0;
    };

    struct SvcObs {
        bool seen = false;
        double rateBpsEWMA = 0.0;
        double blkEWMA = 0.0;
        double delayEWMA = 0.0;
        long queueBytes = 0;
        int currL = 0;
        simtime_t currT = 0;
    };

    // P1-A: Estado global de bloqueo por nodo Core
    struct CoreLinkState {
        int blocksThisWindow = 0;
        double blockRateEWMA = 0.0;  // fracción de bloqueos suavizada
    };

    std::map<int, std::array<AppliedCfg, NUM_SVC>> lastApplied;
    std::map<int, std::array<SvcObs, NUM_SVC>> obs;
    std::map<int, CoreLinkState> coreState; // P1-A: coreId -> estado de bloqueo

    cMessage *agentTimer = nullptr;
    // P1-C: Timer separado para leer respuesta del agente (modela latencia SDN)
    cMessage *agentResponseTimer = nullptr;
    simtime_t decisionInterval = 0.05;
    // P1-C: Latencia real del controlador SDN (escritura -> lectura)
    simtime_t controllerDelay = 0.005;  // 5ms por defecto

    double alpha = 0.3;
    bool enableAgent = true;

    int Lmin = 2000, Lmax = 96000;
    simtime_t Tmin = 0.005, Tmax = 0.1;

    std::string agentInFile = "agent_in.json";
    std::string agentOutFile = "agent_out.json";

    void handleTrafficReport(TrafficReport *rep) {
        int e = rep->getEdgeId();
        int svc = rep->getSvc();
        if (svc < 0 || svc >= NUM_SVC) return;

        auto &st = obs[e][svc];
        st.seen = true;

        double dt = rep->getInterval().dbl();
        double rateBps = (dt > 0) ? (double)rep->getBytesIn() * 8.0 / dt : 0.0;

        double blk = 0.0;
        if (rep->getBurstsAssembled() > 0)
            blk = (double)rep->getBurstsBlocked() / (double)rep->getBurstsAssembled();

        st.rateBpsEWMA = alpha * rateBps + (1.0 - alpha) * st.rateBpsEWMA;
        st.blkEWMA     = alpha * blk     + (1.0 - alpha) * st.blkEWMA;
        double currentDelay = rep->getAvgDelayWindow();
            if (currentDelay > 0) {
                st.delayEWMA = alpha * currentDelay + (1.0 - alpha) * st.delayEWMA;
            }
        st.queueBytes = rep->getQueueBytes();
        st.currL = rep->getCurrLmax();
        st.currT = rep->getCurrTmax();
    }

    void writeAgentInput(const std::string& jsonText) {
        std::ofstream out(agentInFile, std::ios::trunc);
        if (!out.is_open()) {
            EV_WARN << "[SDController] No pude escribir " << agentInFile << "\n";
            return;
        }
        out << jsonText;
        out.close();
    }

    bool readAgentOutput(std::string& content) {
        std::ifstream in(agentOutFile);
        if (!in.is_open())
            return false;

        std::ostringstream ss;
        ss << in.rdbuf();
        content = ss.str();
        in.close();

        return !content.empty();
    }

    void clearAgentOutput() {
        std::remove(agentOutFile.c_str());
    }

    // P1-A + P1-C: Separar escritura del JSON (fase 1, instantánea)
    void buildAndWriteAgentInput() {
        // P1-A: Actualizar EWMA de bloqueos por Core al inicio de cada ventana
        for (auto &cs : coreState) {
            // Decaimiento del EWMA si no hubo nuevos bloqueos en esta ventana
            cs.second.blockRateEWMA =
                alpha * (cs.second.blocksThisWindow > 0 ? 1.0 : 0.0)
                + (1.0 - alpha) * cs.second.blockRateEWMA;
            cs.second.blocksThisWindow = 0;
        }

        std::ostringstream oss;
        oss << "{\"t\":" << simTime().dbl() << ",\"edges\":[";
        bool first = true;

        for (auto &ep : obs) {
            int edgeId = ep.first;
            // P1-A: obtener la tasa de bloqueo del Core asociado a este Edge
            int ingressCore = coreForEdge(edgeId);
            double coreBlkRate = 0.0;
            if (ingressCore >= 0 && coreState.count(ingressCore)) {
                coreBlkRate = coreState[ingressCore].blockRateEWMA;
            }

            for (int svc = 0; svc < NUM_SVC; ++svc) {
                const auto &st = ep.second[svc];
                if (!st.seen) continue;

                if (!first) oss << ",";
                first = false;

                oss << "{"
                    << "\"edgeId\":"  << edgeId           << ","
                    << "\"svc\":"     << svc              << ","
                    << "\"rateBps\":" << st.rateBpsEWMA   << ","
                    << "\"blk\":"     << st.blkEWMA       << ","
                    << "\"coreBlk\":" << coreBlkRate      << ","  // P1-A: estado global
                    << "\"qBytes\":"  << st.queueBytes    << ","
                    << "\"delay\":"   << st.delayEWMA     << ","
                    << "\"L\":"       << st.currL         << ","
                    << "\"T\":"       << st.currT.dbl()
                    << "}";
            }
        }
        oss << "]}";

        clearAgentOutput();
        writeAgentInput(oss.str());

        EV_INFO << "[SDController] JSON escrito al agente en t=" << simTime() << "\n";
    }

    // P1-C: Leer respuesta y aplicar configuración (fase 2, tras latencia del controlador)
    void readAndApplyAgentResponse() {
        std::string resp;
        int maxRetries = 100;
        while (!readAgentOutput(resp) && maxRetries-- > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        if (resp.empty()) {
            EV_WARN << "[SDController] Agente no respondió a tiempo.\n";
            return;
        }

        size_t pos = 0;
        while (true) {
            size_t p = resp.find("\"edgeId\"", pos);
            if (p == std::string::npos) break;

            int edgeId = -1, svc = -1, L = -1;
            double T = 0.0;

            if (std::sscanf(resp.c_str() + p, "\"edgeId\":%d,\"svc\":%d,\"L\":%d,\"T\":%lf",
                            &edgeId, &svc, &L, &T) != 4) {
                pos = p + 7;
                continue;
            }

            if (L < Lmin) L = Lmin;
            if (L > Lmax) L = Lmax;

            simtime_t Tt = T;
            if (Tt < Tmin) Tt = Tmin;
            if (Tt > Tmax) Tt = Tmax;

            int outEdge = findCtrlOutToEdge(edgeId);
            auto &prev = lastApplied[edgeId][svc];
            bool changed = (!prev.valid || prev.L != L || prev.T != Tt);

            if (outEdge >= 0 && 0 <= svc && svc < NUM_SVC && changed) {
                auto *cfg = new BurstifierConfig("BurstifierConfig");
                cfg->setEdgeId(edgeId);
                cfg->setSvc(svc);
                cfg->setNewLmax(L);
                cfg->setNewTmax(Tt);

                EV_INFO << "[AGENTE-FILE] edge=" << edgeId
                        << " svc=" << svc
                        << " -> L=" << L
                        << " T=" << Tt << "\n";

                send(cfg, "ctrlOut", outEdge);

                prev.valid = true;
                prev.L = L;
                prev.T = Tt;
            }

            pos = p + 7;
        }
    }

    int findCtrlOutToCore(int coreId) const {
        for (int i = 0; i < gateSize("ctrlOut"); ++i) {
            const cGate *g = gate("ctrlOut", i);
            if (!g || !g->isConnected()) continue;
            cModule *nbr = g->getNextGate()->getOwnerModule();
            if (nbr->hasPar("coreId") && nbr->par("coreId").intValue() == coreId)
                return i;
        }
        return -1;
    }

    int findCtrlOutToEdge(int edgeId) const {
        for (int i = 0; i < gateSize("ctrlOut"); ++i) {
            const cGate *g = gate("ctrlOut", i);
            if (!g || !g->isConnected()) continue;
            cModule *nbr = g->getNextGate()->getOwnerModule();
            if (nbr->hasPar("edgeId") && nbr->par("edgeId").intValue() == edgeId)
                return i;
        }
        return -1;
    }

    int coreForEdge(int edgeId) const {
            cModule *net = getParentModule();
            for (cModule::SubmoduleIterator it(net); !it.end(); ++it) {
                cModule *m = *it;
                if (!m->hasPar("edgeId")) continue;
                if (m->par("edgeId").intValue() != edgeId) continue;

                cGate *g = m->gate("dataOut", 0);
                if (!g || !g->isConnected()) return -1;

                cModule *core = g->getNextGate()->getOwnerModule();
                if (core && core->hasPar("coreId")) return core->par("coreId").intValue();
            }
            return -1;
        }

    cModule* findCoreModule(int coreId) const {
        cModule *net = getParentModule();
        for (cModule::SubmoduleIterator it(net); !it.end(); ++it) {
            cModule *m = *it;
            if (m->hasPar("coreId") && m->par("coreId").intValue() == coreId)
                return m;
        }
        return nullptr;
    }

    std::map<std::pair<int, int>, std::vector<int>> cachedPaths;

    std::vector<int> computeCorePath(int srcEdge, int dstEdge) {
        std::pair<int, int> cacheKey(srcEdge, dstEdge);
        if (cachedPaths.count(cacheKey)) return cachedPaths[cacheKey];

        int ingress = coreForEdge(srcEdge);
        int egress  = coreForEdge(dstEdge);
        if (ingress < 0 || egress < 0) return {};

        if (ingress == egress) return { ingress };

        cTopology topo;
        topo.extractByNedTypeName(std::vector<std::string>{"src.modules.Core"});

        cModule *srcMod = findCoreModule(ingress);
        cModule *dstMod = findCoreModule(egress);
        if (!srcMod || !dstMod) return {};

        auto *destNode = topo.getNodeFor(dstMod);
        if (!destNode) return {};
        topo.calculateUnweightedSingleShortestPathsTo(destNode);

        auto *n = topo.getNodeFor(srcMod);
        if (!n) return {};

        std::vector<int> path;
        path.push_back(ingress);

        while (n != destNode) {
            if (n->getNumPaths() == 0) { path.clear(); break; }
            auto *link = n->getPath(0);
            n = link->getRemoteNode();
            cModule *m = n->getModule();
            int cid = m->par("coreId").intValue();
            path.push_back(cid);
        }
        cachedPaths[cacheKey] = path;
        return path;
    }

  protected:
    virtual void initialize() override {
        enableAgent = hasPar("enableAgent") ? (bool)par("enableAgent") : true;
        decisionInterval = hasPar("decisionInterval") ? (simtime_t)par("decisionInterval") : 0.05;
        alpha = hasPar("ewmaAlpha") ? (double)par("ewmaAlpha") : 0.3;

        Lmin = hasPar("Lmin") ? (int)par("Lmin") : 2000;
        Lmax = hasPar("Lmax") ? (int)par("Lmax") : 96000;

        Tmin = hasPar("Tmin") ? (simtime_t)par("Tmin") : 0.005;
        Tmax = hasPar("Tmax") ? (simtime_t)par("Tmax") : 0.1;

        // P1-C: Latencia del controlador SDN configurable
        controllerDelay = hasPar("controllerDelay") ? (simtime_t)par("controllerDelay") : 0.005;

        agentInFile  = hasPar("agentInFile")  ? par("agentInFile").stdstringValue()  : std::string("agent_in.json");
        agentOutFile = hasPar("agentOutFile") ? par("agentOutFile").stdstringValue() : std::string("agent_out.json");

        agentTimer = new cMessage("agentTimer");
        scheduleAt(simTime() + decisionInterval + 0.002, agentTimer);

        EV_INFO << "[SDController] Inicializado. Agente=" << (enableAgent ? "ON" : "OFF")
                << " controllerDelay=" << controllerDelay
                << " in=" << agentInFile
                << " out=" << agentOutFile << "\n";
    }

    virtual void handleMessage(cMessage *msg) override {
        // P1-C: Fase 1 — disparar escritura al agente
        if (msg->isSelfMessage() && strcmp(msg->getName(), "agentTimer") == 0) {
            if (enableAgent) {
                buildAndWriteAgentInput();
                // Programar lectura de respuesta tras la latencia del controlador
                agentResponseTimer = new cMessage("agentResponseTimer");
                scheduleAt(simTime() + controllerDelay, agentResponseTimer);
            }
            scheduleAt(simTime() + decisionInterval, agentTimer);
            return;
        }

        // P1-C: Fase 2 — leer y aplicar respuesta del agente
        if (msg->isSelfMessage() && strcmp(msg->getName(), "agentResponseTimer") == 0) {
            readAndApplyAgentResponse();
            delete msg;
            agentResponseTimer = nullptr;
            return;
        }

        if (auto *rep = dynamic_cast<TrafficReport*>(msg)) {
            handleTrafficReport(rep);
            delete rep;
            return;
        }

        if (auto *bcp = dynamic_cast<BCP*>(msg)) {
            int len = bcp->getPathLen();
            int hop = bcp->getHopIndex();

            if (len == 0) {
                int sEdge = bcp->getSrcEdge();
                int dEdge = bcp->getDstEdge();

                std::vector<int> path = computeCorePath(sEdge, dEdge);
                if (path.empty()) {
                    EV_WARN << "[SDController] No path for " << sEdge << "->" << dEdge << " DROP BCP\n";
                    delete bcp;
                    return;
                }

                bcp->setPathLen((int)path.size());
                bcp->setPathArraySize(path.size());
                for (int i = 0; i < (int)path.size(); ++i) bcp->setPath(i, path[i]);
                bcp->setHopIndex(0);

                int firstCore = path[0];
                int out = findCtrlOutToCore(firstCore);
                if (out < 0) { delete bcp; return; }

                EV_INFO << "[SDController] BCP burstId=" << bcp->getBurstId()
                        << " flow " << sEdge << "->" << dEdge << " path=";
                for (int cid : path) EV_INFO << cid << " ";
                EV_INFO << " -> send core" << firstCore << "\n";

                send(bcp, "ctrlOut", out);
                return;
            }

            if (hop >= len) {
                delete bcp;
                return;
            }

            int nextCore = bcp->getPath(hop);
            int out = findCtrlOutToCore(nextCore);
            if (out < 0) { delete bcp; return; }

            EV_INFO << "[SDController] Reenvío BCP burstId=" << bcp->getBurstId()
                    << " hop=" << hop << " -> core" << nextCore << "\n";

            send(bcp, "ctrlOut", out);
            return;
        }

        if (auto *nack = dynamic_cast<BurstNack*>(msg)) {
            // P1-A: Registrar bloqueo en el Core que lo generó
            int fromCore = nack->getCoreId();
            if (fromCore >= 0) {
                coreState[fromCore].blocksThisWindow++;
                EV_INFO << "[SDController] NACK registrado de core=" << fromCore
                        << " blkEWMA=" << coreState[fromCore].blockRateEWMA << "\n";
            }

            int outEdge = findCtrlOutToEdge(nack->getSrcEdge());
            if (outEdge >= 0) {
                send(nack, "ctrlOut", outEdge);
            } else {
                delete nack;
            }
            return;
        }

        EV_WARN << "[SDController] Mensaje desconocido '" << msg->getName() << "'\n";
        delete msg;
    }

    virtual void finish() override {
        // El engine de OMNeT++ cancelará agentTimer y agentResponseTimer automáticamente.
    }
};

Define_Module(SDController);
