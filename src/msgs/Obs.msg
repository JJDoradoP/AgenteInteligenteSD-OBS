cplusplus {{
#include <string>
}}

packet DataPkt {
    int svc;
    int dscp;
}

packet BCP {
    int burstId;
    int bytes;
    double offset;

    int srcEdge;
    int dstEdge;

    int pathLen;
    int path[];
    int hopIndex;
}

packet Burst {
    int burstId;
    int bytes;

    int srcEdge;
    int dstEdge;

    int svc;
    double assemblyTime;
    double sendTime;

    int nVoice;
    int nData;
    int nVideo;
}

message BurstNack {
    int burstId;
    int svc;
    int srcEdge;
    int coreId = -1;   // Nodo Core que generó el bloqueo (para estado global del agente)
}

message TrafficReport {
    int edgeId;
    int svc;
    simtime_t interval;

    long pktsIn;
    long bytesIn;

    long burstsAssembled;
    long burstsBlocked;

    long queuePkts;
    long queueBytes;

    double avgDelayWindow; // <-- AÑADE ESTA LÍNEA

    int currLmax;
    simtime_t currTmax;
}

message BurstifierConfig {
    int edgeId;
    int svc;
    int newLmax;
    simtime_t newTmax;
}
