#pragma once
extern "C"
    {
#include "LGraph.h"
#include "LEdgeInfo.h"
    }
#include <string>

extern int s_nodeCount;
extern int s_edgesPerNode;
extern float s_edgeP;
extern int s_clusterCount;
extern float s_clusterPInter;

extern std::string s_realFilename;
extern LGraph* s_graph;
extern LEdgeInfo* s_edgeWeights;

#define EDGE_WIEGHT_INFINITY    LType_MAX_UI4
#define EDGE_WEIGHT_MIN         1
#define EDGE_WEIGHT_MAX         10000


void generateGraph_random (int seed);
void generateGraph_randomCycle (int seed);
void generateGraph_cluster (int seed);
void generateGraph_power (int seed);
void generateGraph_real();
