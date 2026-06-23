package src.modules;

channel CtrlLink extends ned.DelayChannel
{
    delay = 0.2ms;
}
channel DataLink extends ned.DatarateChannel
{
    delay = 2ms;
    datarate = 10Gbps;
}

simple User
{
    parameters:
        int svc;
        int pktBytes;
        double rate;
        bool isVariableTraffic = default(false);
        double varPeriod @unit(s) = default(1s);
        double varAmplitude = default(0.5);
    gates:
        output out;
        input in;
}

simple Edge
{
    parameters:

        int edgeId;
        int dstEdgeId;
        int LmaxTotal = default(24000);
        double TmaxTotal @unit(s) = default(0.05s);
        // NUEVO: Tiempo de procesamiento de un Core (Delta)
        double coreProcessingTime @unit(s) = default(10us);
        double reportInterval @unit(s) = default(0.05s);
        double ackTimeout @unit(s) = default(0.02s);

    gates:
        input dataIn;       // Entrada electrónica del Usuario (1 cable)
        output dataOut[5];  // Salida óptica hacia el Core (5 lambdas)
        input burstIn[5];  // Entrada óptica desde el Core (5 lambdas)
        output appOut;      // Salida electrónica hacia el Usuario (1 cable)
        input ctrlIn;
        output ctrlOut;
}

simple Core
{
    parameters:
        int coreId;
        @display("i=abstract/router");
    gates:
        input dataIn[15];  // 3 enlaces (Edge, Core, Core) * 5 lambdas = 15
        output dataOut[15];

        input ctrlIn;
        output ctrlOut;
}

simple SDController
{
    parameters:
        bool enableAgent = default(true);
        double decisionInterval @unit(s) = default(0.05s);
        double ewmaAlpha = default(0.3);

        int Lmin = default(2000);
        int Lmax = default(96000);

        double Tmin @unit(s) = default(0.005s);
        double Tmax @unit(s) = default(0.1s);

        // P1-C: Latencia de procesamiento del controlador SDN (modela delay real)
        double controllerDelay @unit(s) = default(0.005s);

        string agentInFile = default("agent_in.json");
        string agentOutFile = default("agent_out.json");
    gates:
        input ctrlIn[];
        output ctrlOut[];
}

network Obs6OBS
{
    @display("bgb=696,380");

    submodules:
        userA: User {
            svc = 0;
            pktBytes = 1500;
            @display("p=138,225");
        }  // Voz (EF)
        userB: User {
            svc = 1;
            pktBytes = 1500;
            @display("p=578,45");
        }   // Video (AF)
        userC: User {
            svc = 2;
            pktBytes = 1500;
            @display("p=147,51");
        }   // Datos (BE)

        edgeA: Edge {
            edgeId = 0;
            dstEdgeId = 2;
            @display("p=220,225");
        }
        edgeB: Edge {
            edgeId = 1;
            dstEdgeId = 2;
            @display("p=481,45");
        }
        edgeC: Edge {
            edgeId = 2;
            dstEdgeId = 0;
            @display("p=270,51");
        }

        core0: Core {
            coreId = 0;
            @display("p=220,122");
        }
        core1: Core {
            coreId = 1;
            @display("p=400,122");
        }
        core2: Core {
            coreId = 2;
            @display("p=323,225");
        }

        ctrl: SDController {
            @display("p=323,122");
        }

    connections allowunconnected:
        // === PLANO DE CONTROL Y ELECTRÓNICO (Se mantiene intacto) ===
        userA.out --> DataLink --> edgeA.dataIn;
        userB.out --> DataLink --> edgeB.dataIn;
        userC.out --> DataLink --> edgeC.dataIn;

        edgeA.appOut --> userA.in;
        edgeB.appOut --> userB.in;
        edgeC.appOut --> userC.in;

        edgeA.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;
        edgeB.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;
        edgeC.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;

        ctrl.ctrlOut++ --> CtrlLink --> core0.ctrlIn;
        ctrl.ctrlOut++ --> CtrlLink --> core1.ctrlIn;
        ctrl.ctrlOut++ --> CtrlLink --> core2.ctrlIn;

        ctrl.ctrlOut++ --> CtrlLink --> edgeA.ctrlIn;
        ctrl.ctrlOut++ --> CtrlLink --> edgeB.ctrlIn;
        ctrl.ctrlOut++ --> CtrlLink --> edgeC.ctrlIn;

        core0.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;
        core1.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;
        core2.ctrlOut --> CtrlLink --> ctrl.ctrlIn++;

        // === PLANO DE DATOS ÓPTICO (W=5 Lambdas por enlace) ===
        for i=0..4 {
            // Edge -> Core (Bloque de inyección: puertos 0 al 4)
            edgeA.dataOut[i] --> DataLink --> core2.dataIn[i];
            edgeB.dataOut[i] --> DataLink --> core1.dataIn[i];
            edgeC.dataOut[i] --> DataLink --> core0.dataIn[i];

            // Inter-core (Interconexión en malla)
            // core0 <--> core1
            core0.dataOut[i] --> DataLink --> core1.dataIn[5 + i];
            core1.dataOut[i] --> DataLink --> core0.dataIn[5 + i];

            // core0 <--> core2
            core0.dataOut[5 + i] --> DataLink --> core2.dataIn[5 + i];
            core2.dataOut[i] --> DataLink --> core0.dataIn[10 + i];

            // core1 <--> core2
            core1.dataOut[5 + i] --> DataLink --> core2.dataIn[10 + i];
            core2.dataOut[5 + i] --> DataLink --> core1.dataIn[10 + i];

            // Core -> Edge (Bloque de entrega: puertos 10 al 14)
            core2.dataOut[10 + i] --> DataLink --> edgeA.burstIn[i];
            core1.dataOut[10 + i] --> DataLink --> edgeB.burstIn[i];
            core0.dataOut[10 + i] --> DataLink --> edgeC.burstIn[i];
        }
}
