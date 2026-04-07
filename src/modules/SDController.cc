/*
 * SDController.cc
 *
 *  Created on: 22/10/2025
 *      Author: admin
 */

/*
 * SDController.cc
 *
 /*
 * SDController.cc
 *
 * Integracion online con agente Python por archivos
 */

/*
 * SDController.cc
 *
 * Integracion online con agente Python por archivos
 * Version afinada con querySeq
 */
/*
 * SDController.cc
 *
 * Integracion online con agente Python por archivos
 * Version afinada con querySeq y doble timer
 */

#include <omnetpp.h>
#include <array>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "../msgs/Obs_m.h"

using namespace omnetpp;

class SDController : public cSimpleModule {
  private:
    static const int NUM_CORES = 3;
    static const int NUM_EDGES = 3;

    std::array<double, NUM_CORES> coreLoad {0.0, 0.0, 0.0};
    std::array<double, NUM_EDGES> edgeLoad {0.0, 0.0, 0.0};

    simtime_t queryPeriod = 0.002;
    simtime_t replyDelay = 0.0005;

    cMessage *queryTimer = nullptr;
    cMessage *replyTimer = nullptr;

    std::string reqFile = "../../python-agent/runtime/state.txt";
    std::string respFile = "../../python-agent/runtime/reply.txt";

    long querySeq = 0;
    long pendingSeq = 0;

    std::array<int, NUM_EDGES> lastLmax {{-1, -1, -1}};
    std::array<double, NUM_EDGES> lastTmax {{-1.0, -1.0, -1.0}};

  private:
    std::vector<int> choosePathForFlow(int srcEdge, int dstEdge) {
        std::vector<int> bestPath;
        const double TH = 5.0;

        if (srcEdge == 0 && dstEdge == 1) {
            std::vector<int> pathShort = {2, 0};
            std::vector<int> pathLong  = {2, 1, 0};
            double loadEnds = coreLoad[2] + coreLoad[0];
            bestPath = (loadEnds > TH && coreLoad[1] <= std::min(coreLoad[2], coreLoad[0])) ? pathLong : pathShort;
        }
        else if (srcEdge == 1 && dstEdge == 0) {
            std::vector<int> pathShort = {0, 2};
            std::vector<int> pathLong  = {0, 1, 2};
            double loadEnds = coreLoad[0] + coreLoad[2];
            bestPath = (loadEnds > TH && coreLoad[1] <= std::min(coreLoad[0], coreLoad[2])) ? pathLong : pathShort;
        }
        else if (srcEdge == 0 && dstEdge == 2) {
            std::vector<int> pathShort = {2, 1};
            std::vector<int> pathLong  = {2, 0, 1};
            double loadEnds = coreLoad[2] + coreLoad[1];
            bestPath = (loadEnds > TH && coreLoad[0] <= std::min(coreLoad[2], coreLoad[1])) ? pathLong : pathShort;
        }
        else if (srcEdge == 2 && dstEdge == 0) {
            std::vector<int> pathShort = {1, 2};
            std::vector<int> pathLong  = {1, 0, 2};
            double loadEnds = coreLoad[1] + coreLoad[2];
            bestPath = (loadEnds > TH && coreLoad[0] <= std::min(coreLoad[1], coreLoad[2])) ? pathLong : pathShort;
        }
        else if (srcEdge == 1 && dstEdge == 2) {
            std::vector<int> pathShort = {0, 1};
            std::vector<int> pathLong  = {0, 2, 1};
            double loadEnds = coreLoad[0] + coreLoad[1];
            bestPath = (loadEnds > TH && coreLoad[2] <= std::min(coreLoad[0], coreLoad[1])) ? pathLong : pathShort;
        }
        else if (srcEdge == 2 && dstEdge == 1) {
            std::vector<int> pathShort = {1, 0};
            std::vector<int> pathLong  = {1, 2, 0};
            double loadEnds = coreLoad[1] + coreLoad[0];
            bestPath = (loadEnds > TH && coreLoad[2] <= std::min(coreLoad[1], coreLoad[0])) ? pathLong : pathShort;
        }
        else {
            int lastCore = (dstEdge == 0 ? 2 : (dstEdge == 1 ? 0 : 1));
            bestPath = { lastCore };
        }

        return bestPath;
    }

    void writeStateForAgent() {
        querySeq++;
        pendingSeq = querySeq;

        std::ofstream out(reqFile.c_str(), std::ios::trunc);
        if (!out.is_open()) {
            EV_WARN << "[SDController] No pude escribir estado en " << reqFile << "\n";
            return;
        }

        out << "STATE "
            << pendingSeq << " "
            << coreLoad[0] << " "
            << coreLoad[1] << " "
            << coreLoad[2] << " "
            << edgeLoad[0] << " "
            << edgeLoad[1] << " "
            << edgeLoad[2] << "\n";
        out.close();

        EV_INFO << "[SDController] QUERY #" << pendingSeq
                << " t=" << simTime()
                << " STATE cores="
                << coreLoad[0] << "," << coreLoad[1] << "," << coreLoad[2]
                << " edges="
                << edgeLoad[0] << "," << edgeLoad[1] << "," << edgeLoad[2]
                << "\n";
    }

    bool readAgentReplyAndUpdateEdges() {
        std::ifstream in(respFile.c_str());
        if (!in.is_open()) {
            EV_WARN << "[SDController] No pude abrir respuesta del agente: " << respFile << "\n";
            return false;
        }

        std::string line;
        std::getline(in, line);
        in.close();

        if (line.empty()) {
            EV_WARN << "[SDController] Respuesta del agente vacia.\n";
            return false;
        }

        EV_INFO << "[SDController] Respuesta agente: '" << line << "'\n";

        std::istringstream iss(line);
        std::string tag;
        long respSeq = -1;

        iss >> tag >> respSeq;
        if (tag != "UPDATE") {
            EV_WARN << "[SDController] Respuesta inesperada del agente: '" << line << "'\n";
            return false;
        }

        if (respSeq != pendingSeq) {
            EV_WARN << "[SDController] Respuesta desfasada. Esperaba seq="
                    << pendingSeq << " y llego seq=" << respSeq << "\n";
            return false;
        }

        int L[NUM_EDGES];
        double T[NUM_EDGES];

        for (int e = 0; e < NUM_EDGES; ++e) {
            if (!(iss >> L[e] >> T[e])) {
                EV_WARN << "[SDController] UPDATE incompleto: '" << line << "'\n";
                return false;
            }
        }

        bool sentAny = false;

        for (int e = 0; e < NUM_EDGES; ++e) {
            bool sameL = (L[e] == lastLmax[e]);
            bool sameT = (std::fabs(T[e] - lastTmax[e]) < 1e-12);

            if (sameL && sameT)
                continue;

            auto *up = new ParamUpdate("ParamUpdate");
            up->setEdgeId(e);
            up->setLmaxTotal(L[e]);
            up->setTmaxTotal(T[e]);

            EV_INFO << "[SDController] ParamUpdate -> edge " << e
                    << " LmaxTotal=" << L[e]
                    << " TmaxTotal=" << T[e] << "\n";

            send(up, "ctrlOutEdges", e);

            lastLmax[e] = L[e];
            lastTmax[e] = T[e];
            sentAny = true;
        }

        if (!sentAny) {
            EV_INFO << "[SDController] UPDATE sin cambios efectivos; no envio ParamUpdate.\n";
        }

        return true;
    }

  protected:
    virtual void initialize() override {
        coreLoad.fill(0.0);
        edgeLoad.fill(0.0);

        if (hasPar("queryPeriod"))
            queryPeriod = par("queryPeriod");
        if (hasPar("replyDelay"))
            replyDelay = par("replyDelay");
        if (hasPar("reqFile"))
            reqFile = par("reqFile").stdstringValue();
        if (hasPar("respFile"))
            respFile = par("respFile").stdstringValue();

        queryTimer = new cMessage("queryTimer");
        replyTimer = new cMessage("replyTimer");

        EV_INFO << "[SDController] Inicializado. Esperando CoreStatus y BCP...\n";
        EV_INFO << "[SDController] Integracion online por archivos.\n";
        EV_INFO << "[SDController] reqFile=" << reqFile
                << " respFile=" << respFile
                << " queryPeriod=" << queryPeriod
                << " replyDelay=" << replyDelay << "\n";

        scheduleAt(simTime() + queryPeriod, queryTimer);
    }

    virtual void handleMessage(cMessage *msg) override {
        if (msg == queryTimer) {
            writeStateForAgent();

            if (!replyTimer->isScheduled())
                scheduleAt(simTime() + replyDelay, replyTimer);

            scheduleAt(simTime() + queryPeriod, queryTimer);
            return;
        }

        if (msg == replyTimer) {
            readAgentReplyAndUpdateEdges();
            return;
        }

        if (auto *st = dynamic_cast<CoreStatus*>(msg)) {
            int id = st->getCoreId();
            if (0 <= id && id < NUM_CORES) {
                coreLoad[id] = st->getQueuedBursts();
                EV_INFO << "[SDController] CoreStatus coreId=" << id
                        << " queuedBursts=" << coreLoad[id] << "\n";
            } else {
                EV_WARN << "[SDController] CoreStatus con coreId invalido: " << id << "\n";
            }
            delete st;
            return;
        }

        if (auto *es = dynamic_cast<EdgeStatus*>(msg)) {
            int id = es->getEdgeId();
            if (0 <= id && id < NUM_EDGES) {
                edgeLoad[id] = es->getTotalBuffered();
                EV_INFO << "[SDController] EdgeStatus edgeId=" << id
                        << " totalBuffered=" << edgeLoad[id]
                        << " (V/D/Vid)="
                        << es->getNVoice() << "/"
                        << es->getNData()  << "/"
                        << es->getNVideo() << "\n";
            } else {
                EV_WARN << "[SDController] EdgeStatus con edgeId invalido: " << id << "\n";
            }
            delete es;
            return;
        }

        if (auto *bcp = dynamic_cast<BCP*>(msg)) {
            int len = bcp->getPathLen();
            int hop = bcp->getHopIndex();

            if (len == 0) {
                int sEdge = bcp->getSrcEdge();
                int dEdge = bcp->getDstEdge();

                std::vector<int> path = choosePathForFlow(sEdge, dEdge);
                if (path.empty()) {
                    EV_WARN << "[SDController] No hay ruta para flujo "
                            << sEdge << "->" << dEdge << ". Se descarta BCP.\n";
                    delete bcp;
                    return;
                }

                len = (int)path.size();

                EV_INFO << "[SDController] BCP NUEVO burstId=" << bcp->getBurstId()
                        << " flow " << sEdge << "->" << dEdge
                        << " elijo path=";
                for (int cid : path) EV_INFO << cid << " ";
                EV_INFO << "(len=" << len << ")\n";

                bcp->setPathLen(len);
                bcp->setPathArraySize(len);
                for (int i = 0; i < len; ++i)
                    bcp->setPath(i, path[i]);

                bcp->setHopIndex(0);
                int firstCore = path[0];

                auto *rep = new PathReply("PathReply");
                rep->setBurstId(bcp->getBurstId());
                rep->setSrcEdge(sEdge);
                rep->setDstEdge(dEdge);
                rep->setPathLen(len);
                rep->setPathArraySize(len);
                for (int i = 0; i < len; ++i)
                    rep->setPath(i, path[i]);

                EV_INFO << "[SDController] -> envio PathReply burstId=" << bcp->getBurstId()
                        << " a edge " << sEdge << "\n";
                send(rep, "ctrlOutEdges", sEdge);

                EV_INFO << "[SDController] -> envio BCP burstId=" << bcp->getBurstId()
                        << " a core " << firstCore << "\n";
                send(bcp, "ctrlOutCores", firstCore);
                return;
            }

            if (hop >= len) {
                EV_INFO << "[SDController] BCP burstId=" << bcp->getBurstId()
                        << " path completado. Fin de BCP.\n";
                delete bcp;
                return;
            }

            int nextCore = bcp->getPath(hop);

            EV_INFO << "[SDController] BCP transito burstId=" << bcp->getBurstId()
                    << " hop=" << hop << "/" << len
                    << " -> envio a core " << nextCore << "\n";

            send(bcp, "ctrlOutCores", nextCore);
            return;
        }

        EV_WARN << "[SDController] Mensaje desconocido '" << msg->getName()
                << "', se descarta.\n";
        delete msg;
    }

    virtual void finish() override {
        if (queryTimer)
            cancelAndDelete(queryTimer);
        if (replyTimer)
            cancelAndDelete(replyTimer);
    }
};

Define_Module(SDController);
