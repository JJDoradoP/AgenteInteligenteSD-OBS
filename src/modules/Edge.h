/*
 * Edge.h
 *
 *  Created on: 22/10/2025
 *      Author: admin
 */

#include <omnetpp.h>
using namespace omnetpp;
#include "../msgs/Obs_m.h"



class Edge : public cSimpleModule {
  private:
    cMessage* tick = nullptr;
    simtime_t period;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
    virtual void finish() override;
};


