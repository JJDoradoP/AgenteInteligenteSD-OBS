// Mensaje mínimo para pruebas
//packet DataPkt {
//    int svc = 0;     // 0: voz, 1: datos, 2: video
//    int bytes = 200; // solo un campo de ejemplo
//}
//packet DataPkt {
//    int svc;      // 0 voz, 1 datos, 2 video
//    int bytes;    // tamaño del paquetito
//}

//packet BCP {
//    int dst;
//    int bytes;        // tamaño total de la ráfaga
//    double offset;    // segundos hasta que llegue la ráfaga
//    int burstId;      // ID de ráfaga (para depuración)
//    int srcId;        // id del Edge emisor (opcional)
//    string srcName;   // nombre "edgeA"/"edgeB"
//}

//packet Burst {
//    int dst;
//    int bytes;
//    int burstId;
//    int srcId;
//    string srcName;

    // conteo de paquetitos por clase dentro de la ráfaga
//    int nVoice;
//    int nData;
//    int nVideo;
//}

cplusplus {{
#include <string>
}}

packet DataPkt {
    int svc;          // 0=voz,1=datos,2=video
}

packet BCP {
    int burstId;
    int srcId;
    int dst;
    int ingressCore;
    int bytes;
    double offset;
    string srcName;

    int srcEdge;
    int dstEdge;

    int pathLen;
    int path[];
    int hopIndex;
}

packet Burst {
    int burstId;
    int srcId;
    int dst;
    int bytes;

    int nVoice;
    int nData;
    int nVideo;

    string srcName;

    int srcEdge;
    int dstEdge;

    int pathLen;
    int hopIndex;
    int path[];
    
    simtime_t creationTime;
}

message CoreStatus {
    int coreId;
    int queuedBursts;
    double estDelay;
}
message EdgeStatus
{
    int edgeId;
    int totalBuffered;
    int nVoice;
    int nData;
    int nVideo;
}

message PathReply {
    int burstId;
    int srcEdge;
    int dstEdge;
    int pathLen;
    int path[];
}

//**
// * UPDATE de parámetros híbridos (TOTAL) hacia el Edge.
 //* El SDController lo envía por ctrlOutEdges[edgeId].
 //*/
message ParamUpdate {
    int edgeId;
    int LmaxTotal;
    double TmaxTotal; // segundos
}
