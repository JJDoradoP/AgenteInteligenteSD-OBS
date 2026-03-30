#include <omnetpp.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#include "../msgs/Obs_m.h"

using namespace omnetpp;

class SDController : public cSimpleModule {
  private:
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

    // edgeId -> coreId al que está conectado su dataOut
    int coreForEdge(int edgeId) const {
        cModule *net = getParentModule();
        for (cModule::SubmoduleIterator it(net); !it.end(); ++it) {
            cModule *m = *it;
            if (!m->hasPar("edgeId")) continue;
            if (m->par("edgeId").intValue() != edgeId) continue;

            cGate *g = m->gate("dataOut");
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

    std::vector<int> computeCorePath(int srcEdge, int dstEdge) {
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
        return path;
    }

  protected:
    virtual void initialize() override {
        EV_INFO << "[SDController] Inicializado (SIN AGENTE)\n";
    }

    virtual void handleMessage(cMessage *msg) override {
        // ===== BCP routing + ACK =====
        if (auto *bcp = dynamic_cast<BCP*>(msg)) {
            int len = bcp->getPathLen();
            int hop = bcp->getHopIndex();

            // BCP nuevo: calcular path y enviar al primer core
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

            // BCP completado => ACK al edge origen
            if (hop >= len) {
                int srcEdge = bcp->getSrcEdge();
                int outEdge = findCtrlOutToEdge(srcEdge);

                auto *ack = new BurstAck("BurstAck");
                ack->setBurstId(bcp->getBurstId());

                if (outEdge >= 0) {
                    EV_INFO << "[SDController] BCP completado burstId=" << bcp->getBurstId()
                            << " -> ACK a edge" << srcEdge << "\n";
                    send(ack, "ctrlOut", outEdge);
                } else {
                    delete ack;
                }

                delete bcp;
                return;
            }

            // Reenvío al próximo core
            int nextCore = bcp->getPath(hop);
            int out = findCtrlOutToCore(nextCore);
            if (out < 0) { delete bcp; return; }

            EV_INFO << "[SDController] Reenvío BCP burstId=" << bcp->getBurstId()
                    << " hop=" << hop << " -> core" << nextCore << "\n";

            send(bcp, "ctrlOut", out);
            return;
        }

        EV_WARN << "[SDController] Mensaje desconocido '" << msg->getName() << "'\n";
        delete msg;
    }
};

Define_Module(SDController);
