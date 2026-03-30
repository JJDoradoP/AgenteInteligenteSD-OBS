cplusplus {{
#include <string>
}}

packet DataPkt {
    int svc;          // 0=voz,1=datos,2=video
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

    int svc;                // <-- NUEVO
    double assemblyTime;    // <-- NUEVO
    double sendTime;        // <-- NUEVO

    int nVoice;
    int nData;
    int nVideo;
}

message BurstAck {
    int burstId;
}
