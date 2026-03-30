package src.modules;

channel CtrlLink extends ned.DelayChannel { delay = 0.2ms; }
channel DataLink extends ned.DatarateChannel { delay = 1ms; datarate = 1Gbps; }

simple User
{
    parameters:
        int svc;
        int pktBytes;
        double rate;
    gates:
        output out;
        input  in;     // <-- NUEVO para recibir
}

simple Edge
{
    parameters:
        int edgeId;
        int dstEdgeId;

        int LmaxTotal = default(24000);
        double TmaxTotal @unit(s) = default(0.05s);
        double offsetBase @unit(s) = default(0.003s);

        double ackTimeout @unit(s) = default(0.02s);
     gates:
        input  dataIn;
        input  burstIn;
        output dataOut;

        output appOut;     // <-- NUEVO para entregar pkts al User local

        input  ctrlIn;
        output ctrlOut;
}

simple Core
{
    parameters:
        int coreId;
        @display("i=abstract/router");
    gates:
        input  dataIn[3];
        output dataOut[3];

        input  ctrlIn;
        output ctrlOut;
}

simple SDController
{
    gates:
        input  ctrlIn[];
        output ctrlOut[];
}
network Obs6OBS
{
    @display("bgb=696,380");

    submodules:
        userA: User { svc=0; pktBytes=500; rate=50; @display("p=138,225"); }
        userB: User { svc=1; pktBytes=800; rate=40; @display("p=578,45"); }
        userC: User { svc=2; pktBytes=1000; rate=30; @display("p=147,51"); }

        edgeA: Edge { edgeId=0; dstEdgeId=1; @display("p=220,225"); }
        edgeB: Edge { edgeId=1; dstEdgeId=2; @display("p=481,45"); }
        edgeC: Edge { edgeId=2; dstEdgeId=0; @display("p=270,51"); }

        core0: Core { coreId=0; @display("p=220,122"); }
        core1: Core { coreId=1; @display("p=400,122"); }
        core2: Core { coreId=2; @display("p=323,225"); }

        ctrl: SDController { @display("p=323,122"); }

    connections allowunconnected:

        // Users -> Edges (generación)
        userA.out --> DataLink --> edgeA.dataIn;
        userB.out --> DataLink --> edgeB.dataIn;
        userC.out --> DataLink --> edgeC.dataIn;

        // Edges -> Users (entrega tras desensamble)
        edgeA.appOut --> userA.in;
        edgeB.appOut --> userB.in;
        edgeC.appOut --> userC.in;

        // Edge -> Controller (BCP + stats)
        edgeA.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;
        edgeB.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;
        edgeC.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;

        // Controller -> Cores (0..2)
        ctrl.ctrlOut++ --> CtrlLink --> core0.ctrlIn;
        ctrl.ctrlOut++ --> CtrlLink --> core1.ctrlIn;
        ctrl.ctrlOut++ --> CtrlLink --> core2.ctrlIn;

        // Controller -> Edges (ACK/CFG) (3..5)
        ctrl.ctrlOut++ --> CtrlLink --> edgeA.ctrlIn;
        ctrl.ctrlOut++ --> CtrlLink --> edgeB.ctrlIn;
        ctrl.ctrlOut++ --> CtrlLink --> edgeC.ctrlIn;

        // Cores -> Controller (feedback BCP)
        core0.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;
        core1.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;
        core2.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;

        // Edge -> Core (inyección burst)
        edgeA.dataOut --> DataLink --> core2.dataIn[0];
        edgeB.dataOut --> DataLink --> core1.dataIn[0];
        edgeC.dataOut --> DataLink --> core0.dataIn[0];

        // Inter-core
        core0.dataOut[0] --> DataLink --> core1.dataIn[1];
        core1.dataOut[0] --> DataLink --> core0.dataIn[1];

        core0.dataOut[1] --> DataLink --> core2.dataIn[1];
        core2.dataOut[0] --> DataLink --> core0.dataIn[2];

        core1.dataOut[1] --> DataLink --> core2.dataIn[2];
        core2.dataOut[1] --> DataLink --> core1.dataIn[2];

        // Core -> Edge (entrega burst)
        core2.dataOut[2] --> DataLink --> edgeA.burstIn;
        core1.dataOut[2] --> DataLink --> edgeB.burstIn;
        core0.dataOut[2] --> DataLink --> edgeC.burstIn;
}
