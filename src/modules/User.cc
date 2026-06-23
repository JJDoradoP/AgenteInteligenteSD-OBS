#include <omnetpp.h>
#include "../msgs/Obs_m.h"

using namespace omnetpp;

class User : public cSimpleModule {
    private:
    cMessage *sendTimer = nullptr;
    int svc;
    int pktBytes;
    double rate;
    bool isVariableTraffic;
    double varPeriod;
    double varAmplitude;
    
    int dscpForSvc(int s) const {
        if (s == 0) return 46; // EF
        if (s == 2) return 34; // AF
        return 0;              // BE
    }

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
        isVariableTraffic = par("isVariableTraffic");
        varPeriod = par("varPeriod").doubleValue();
        varAmplitude = par("varAmplitude").doubleValue();

        EV_INFO << "[USER] init svc=" << svc
                << " pktBytes=" << pktBytes
                << " rate=" << rate 
                << " isVar=" << isVariableTraffic << "\n";

        sendTimer = new cMessage("sendTimer");
        scheduleAt(simTime() + exponential(1.0 / rate), sendTimer);
    }

    virtual void handleMessage(cMessage *msg) override {
        if (msg == sendTimer) {
            cGate *outGate = gate("out");
            if (!outGate) {
                throw cRuntimeError("Gate 'out' no existe");
            }

            cChannel *baseCh = outGate->getTransmissionChannel();
            cDatarateChannel *txCh = dynamic_cast<cDatarateChannel *>(baseCh);

            if (txCh && txCh->isBusy()) {
                simtime_t freeAt = txCh->getTransmissionFinishTime();

                if (freeAt <= simTime())
                    freeAt = simTime() + SimTime(1, SIMTIME_US);

                scheduleAt(freeAt, sendTimer);
                return;
            }

            int randSvc = intuniform(0, 2);
            auto *pkt = new DataPkt("DataPkt");
            pkt->setSvc(randSvc);
            pkt->setDscp(dscpForSvc(randSvc));
            pkt->setByteLength(pktBytes);

            send(pkt, "out");

            double currentRate = rate;
            if (isVariableTraffic && varPeriod > 0) {
                // Onda senoidal: currentRate = rate * (1 + amplitud * sen(2*pi*t / periodo))
                double t = simTime().dbl();
                double factor = 1.0 + varAmplitude * sin(2.0 * M_PI * t / varPeriod);
                currentRate = rate * factor;
                if (currentRate < 10.0) currentRate = 10.0; // Evitar tasas negativas o cero
            }
            
            simtime_t nextTime = simTime() + exponential(1.0 / currentRate);
            if (nextTime <= simTime())
                nextTime = simTime() + SimTime(1, SIMTIME_US);

            scheduleAt(nextTime, sendTimer);
            return;
        }

        if (auto *p = dynamic_cast<DataPkt*>(msg)) {
            // Quitamos el EV_INFO aquí para no saturar la consola en tráfico alto
            delete p;
            return;
        }

        delete msg;
    }

    virtual void finish() override {
        if (sendTimer && sendTimer->isScheduled()) cancelEvent(sendTimer);
    }
};

Define_Module(User);
