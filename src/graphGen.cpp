#include "graphGen.h"
#include <vector>
#include <stdio.h>
#include <assert.h>
extern "C"
    {
#include "LArray.h"
#include "_LGraph.h"
#include "LGraphGen.h"
#include "LArray.h"
#include "LRandSource.h"
    }


void generateGraph_random (int seed)
    {
    s_graph = LGraphGen_RandomNP (s_nodeCount, s_edgeP, seed);
    LGraphGen_RemoveSelfLoops (&s_graph);

    s_edgeWeights = LGraphGen_RndEdgeInfoUI4 (s_graph, EDGE_WEIGHT_MIN, EDGE_WEIGHT_MAX, seed);
    }

void generateGraph_randomCycle (int seed)
    {
    s_graph = LGraphGen_RandomNP (s_nodeCount, s_edgeP, seed);
    LGraphGen_RemoveSelfLoops (&s_graph);
    LGraphGen_AddCycle (&s_graph);

    s_edgeWeights = LGraphGen_RndEdgeInfoUI4 (s_graph, EDGE_WEIGHT_MIN, EDGE_WEIGHT_MAX, seed);
    }

void generateGraph_cluster (int seed)
    {
    s_graph = LGraphGen_RandomClustered (s_nodeCount, 10, 0.85, 0.05, seed, true);
    LGraphGen_RemoveSelfLoops (&s_graph);

    s_edgeWeights = LGraphGen_RndEdgeInfoUI4 (s_graph, EDGE_WEIGHT_MIN, EDGE_WEIGHT_MAX, seed);
    }

void generateGraph_real()
    {
    FILE* pRealFile = fopen (s_realFilename.c_str(), "r");
    if (pRealFile == nullptr)
        {
        printf ("Failed to open file: %s", s_realFilename.c_str());
        return;
        }

    std::vector<std::pair<int, int>> edges;
    int source, target;
    int maxIndex = 0;
    while (2 == fscanf (pRealFile, "%d %d", &source, &target))
        {
        edges.push_back (std::make_pair (source, target));
        if (source > maxIndex)
            maxIndex = source;
        if (target > maxIndex)
            maxIndex = target;
        }
    fclose (pRealFile);

    s_graph = LGraph_New (false);
    for (int i = 0; i <= maxIndex; ++i)
        LGraph_NewNode (s_graph);

    LGraph_TNode** ppNodes;
    LArray* pNodeList = LGraph_GetAllNodes (s_graph);
    LArray_InstallSyncPtr (pNodeList, (void**)&ppNodes);

    for (std::pair<int, int> const& edge : edges)
        LGraph_NewEdge (s_graph, ppNodes[edge.first], ppNodes[edge.second]);
    LArray_Delete (&pNodeList);

    s_edgeWeights = LGraphGen_RndEdgeInfoUI4 (s_graph, 1, 1, 1);
    }


// --------------------------------------------------------------------------- //
// Preferential attachment graph model
// Nidhi Parikh, "Generating Random Graphs with Tunable Clustering Coefficient"
// --------------------------------------------------------------------------- //
static void createPrefAttachGraph (int seed);
static ui4 findNodeIndex (ui4 r, std::vector<ui4> const& prefixSums);
static bool checkEdgeExists (LGraph* pGraph, ui4 source, ui4 target);

void generateGraph_power (int seed)
    {
    s_edgesPerNode = (s_nodeCount * s_nodeCount / 2) * s_edgeP / s_nodeCount;

    createPrefAttachGraph (seed);
    LGraphGen_RemoveSelfLoops (&s_graph);

    s_edgeWeights = LGraphGen_RndEdgeInfoUI4 (s_graph, EDGE_WEIGHT_MIN, EDGE_WEIGHT_MAX, seed);
    }

static void createPrefAttachGraph (int seed)
    {
    LRandSource* pRandSource = LRandSource_New (seed);
    std::vector<LGraph_TNode*> nodeArray;
    std::vector<ui4> prefixSums;

    s_graph = LGraph_New (false);

    ui4 const initialNodeCount = s_edgesPerNode + 2;
    for (ui4 i = 0; i < initialNodeCount; ++i)
        LGraph_NewNode (s_graph);

    LGraphGen_AddCycle (&s_graph);
    LGraph_TNode* pNode;
    LGraph_ForAllNodes (s_graph, pNode)
        nodeArray.push_back (pNode);

    LGraph_ForAllNodes (s_graph, pNode)
        {
        ui4 sum = pNode->mOutDeg;
        if (pNode->mPrev != nullptr)
            sum += prefixSums[prefixSums.size() - 1];
        prefixSums.push_back (sum);
        }

    for (ui4 i = initialNodeCount; i < s_nodeCount; ++i)
        {
        nodeArray.push_back (LGraph_NewNode (s_graph));
        prefixSums.push_back (prefixSums[prefixSums.size() - 1]);

        ui4 edgesAdded = 0;
        while (edgesAdded < s_edgesPerNode)
            {
            ui4 r = LRandSource_GetRandUI4 (pRandSource, 1, prefixSums[i - 1]);
            ui4 index = findNodeIndex (r, prefixSums);
            if (!checkEdgeExists (s_graph, i, index))
                {
                LGraph_NewEdge (s_graph, nodeArray[i], nodeArray[index]);
                edgesAdded++;

                for (ui4 j = index; j <= i; ++j)
                    prefixSums[j]++;
                prefixSums[i]++;
                }
            }
        }
    }

static ui4 findNodeIndex (ui4 r, std::vector<ui4> const& prefixSums)
    {
    ui4 low = 0;
    ui4 high = prefixSums.size() - 1;
    while (low < high)
        {
        ui4 mid = (low + high) / 2;
        if (r <= prefixSums[low])
            return low;
        else if (low == mid)
            return high;
        else if (r <= prefixSums[mid])
            high = mid;
        else
            low = mid;
        }
    return low;
    }

static bool checkEdgeExists (LGraph* pGraph, ui4 source, ui4 target)
    {
    // Could do this faster ..
    LGraph_TEdge* pEdge;
    LGraph_ForAllEdges (s_graph, pEdge)
        {
        ui4 _source = pEdge->mSource->mIndex;
        ui4 _target = pEdge->mTarget->mIndex;
        if ((source == _source && target == _target) ||
            (source == _target && target == _source))
            return true;
        }
    return false;
    }
