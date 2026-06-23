/*
 * Receiver.cc
 *
 *  Created on: 9/12/2025
 *      Author: aleja
 */

/*
 * Receiver.cc
 *
 *  Created on: 22/10/2025
 *      Author: admin
 */


#include <omnetpp.h>
using namespace omnetpp;

class Receiver : public cSimpleModule {
  protected:
    simsignal_t rxSig;
    virtual void initialize() override { rxSig = registerSignal("rxPkts"); }
    virtual void handleMessage(cMessage* msg) override {
        emit(rxSig, 1);
        delete msg;
    }
};
Define_Module(Receiver);





