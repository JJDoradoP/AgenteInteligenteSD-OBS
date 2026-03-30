#include <omnetpp.h>
#include "../msgs/Obs_m.h"

using namespace omnetpp;

class User : public cSimpleModule {
  private:
    cMessage *sendTimer = nullptr;
    int svc;
    int pktBytes;
    double rate;

    const char* svcName(int s) const {
        if (s == 0) return "VOZ";
        if (s == 1) return "DATOS";
        if (s == 2) return "VIDEO";
        return "UNK";
    }

  protected:
    virtual void initialize() override {
        svc = par("svc");
        pktBytes = par("pktBytes");
        rate = par("rate");

        sendTimer = new cMessage("sendTimer");
        scheduleAt(simTime() + exponential(1.0 / rate), sendTimer);
    }

    virtual void handleMessage(cMessage *msg) override {
        if (msg == sendTimer) {
            auto *pkt = new DataPkt("DataPkt");
            pkt->setSvc(svc);
            pkt->setByteLength(pktBytes);

            send(pkt, "out");
            scheduleAt(simTime() + exponential(1.0 / rate), sendTimer);
            return;
        }

        // Recepción
        if (auto *p = dynamic_cast<DataPkt*>(msg)) {
            EV_INFO << "[USER] Recibido paquete servicio=" << svcName(p->getSvc())
                    << " bytes=" << p->getByteLength()
                    << " en t=" << simTime() << "\n";
            delete p;
            return;
        }

        delete msg;
    }

    virtual void finish() override {
        cancelAndDelete(sendTimer);
    }
};

Define_Module(User);
