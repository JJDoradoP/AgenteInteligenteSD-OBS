//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 


package src.modules;

// ------------------------ CANALES ------------------------
channel CtrlLink extends ned.DelayChannel
{
    delay = 0.2ms;
}

channel DataLink extends ned.DatarateChannel
{
    delay = 1ms;
    datarate = 1Gbps;
}

// --------------------- MÓDULOS SIMPLES --------------------
simple Edge
{
    parameters:
        double period @unit(s) = default(0.01s);
        int pktBytes @unit(B) = default(200B);

        int LmaxTotal = default(24);
        double TmaxTotal @unit(s) = default(0.01s);

        double rateVoice = default(500);
        double rateData  = default(300);
        double rateVideo = default(200);

        double offsetBase @unit(s) = default(0.003s);

        int dstId  = default(0);
        int edgeId = default(0);
                // límites por servicio (para que coincida con omnetpp.ini)
        double TmaxVoice @unit(s) = default(0.008s);
        double TmaxData  @unit(s) = default(0.015s);
        double TmaxVideo @unit(s) = default(0.025s);

        int LmaxVoice = default(24);
        int LmaxData  = default(40);
        int LmaxVideo = default(100);
        
		bool agentEnabled = default(true);
        string agentHost = default("127.0.0.1");
        int agentPort = default(5555);
    gates:
        // control
        output ctrlOut;
        input  ctrlIn;

        // datos
        output dataOut;
        input  dataIn;
}

simple Core
{
    parameters:
        int coreId = default(0);
        double serviceTime @unit(s) = default(0.0005s);
        double statusPeriod @unit(s) = default(0.01s); 

    gates:
        // datos
        input  dataIn[];
        output dataOut[];

        // control
        input  ctrlIn[];
        output ctrlOut[];
}

simple SDController
{
    parameters:
        double queryPeriod @unit(s) = default(0.002s);
        double replyDelay @unit(s) = default(0.0005s);
        string reqFile = default("../../python-agent/runtime/state.txt");
        string respFile = default("../../python-agent/runtime/reply.txt");
    gates:
        input  ctrlInEdges[];
        output ctrlOutEdges[];
        input  ctrlInCores[];
        output ctrlOutCores[];
}

// ------------------------- RED ----------------------------
network Obs4U3E3C
{
    submodules:
        core0: Core { parameters: coreId = 0; @display("p=280,327"); }
        core1: Core { parameters: coreId = 1; @display("p=431,261"); }
        core2: Core { parameters: coreId = 2; @display("p=129,261"); }

        edgeA: Edge { parameters: edgeId = 0; @display("p=90,70;i=device/laptop"); }
        edgeB: Edge { parameters: edgeId = 1; @display("p=280,50;i=device/laptop"); }
        edgeC: Edge { parameters: edgeId = 2; @display("p=470,70;i=device/laptop"); }

        ctrl: SDController { @display("p=280,204;i=block/control"); }

    connections allowunconnected:
        // -------- CONTROL: Edge <-> Controller --------
        edgeA.ctrlOut --> CtrlLink --> ctrl.ctrlInEdges++;
ctrl.ctrlOutEdges++ --> CtrlLink --> edgeA.ctrlIn;

edgeB.ctrlOut --> CtrlLink --> ctrl.ctrlInEdges++;
ctrl.ctrlOutEdges++ --> CtrlLink --> edgeB.ctrlIn;

edgeC.ctrlOut --> CtrlLink --> ctrl.ctrlInEdges++;
ctrl.ctrlOutEdges++ --> CtrlLink --> edgeC.ctrlIn;


        // -------- CONTROL: Controller <-> Cores --------
        ctrl.ctrlOutCores++ --> CtrlLink --> core0.ctrlIn++;
core0.ctrlOut++ --> CtrlLink --> ctrl.ctrlInCores++;

ctrl.ctrlOutCores++ --> CtrlLink --> core1.ctrlIn++;
core1.ctrlOut++ --> CtrlLink --> ctrl.ctrlInCores++;

ctrl.ctrlOutCores++ --> CtrlLink --> core2.ctrlIn++;
core2.ctrlOut++ --> CtrlLink --> ctrl.ctrlInCores++;


        // -------- DATA: Edge <-> Core --------
        edgeA.dataOut --> DataLink --> core2.dataIn++;
        core2.dataOut++ --> DataLink --> edgeA.dataIn;

        edgeB.dataOut --> DataLink --> core0.dataIn++;
        core0.dataOut++ --> DataLink --> edgeB.dataIn;

        edgeC.dataOut --> DataLink --> core1.dataIn++;
        core1.dataOut++ --> DataLink --> edgeC.dataIn;

        // -------- DATA: Core <-> Core (malla) --------
        core0.dataOut++ --> DataLink --> core1.dataIn++;
        core1.dataOut++ --> DataLink --> core0.dataIn++;

        core0.dataOut++ --> DataLink --> core2.dataIn++;
        core2.dataOut++ --> DataLink --> core0.dataIn++;

        core1.dataOut++ --> DataLink --> core2.dataIn++;
        core2.dataOut++ --> DataLink --> core1.dataIn++;
}

