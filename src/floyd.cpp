#include "floyd.h"
extern "C"
    {
#include "LArray.h"
#include "LMemory.h"
#include "_LGraph.h"
    }

#define FLOYD_INFINITY LType_MAX_UI4

ui4* APSP_Floyd (LGraph* pGraph, LEdgeInfo* pEdgeWeights)
    {
    if (pGraph == nullptr || pEdgeWeights == nullptr)
        return nullptr;

    ui4 const nodeCount = pGraph->mNodesCount;
    ui4 const matrixCellCount = nodeCount * nodeCount;
    ui4* distanceMat = (ui4*)LMemory_Malloc (matrixCellCount * sizeof (ui4));

    for (ui4 i = 0; i < matrixCellCount; ++i)
        distanceMat[i] = FLOYD_INFINITY;

    LGraph_TEdge* pEdge;
    LGraph_ForAllEdges (pGraph, pEdge)
        {
        ui4 const i = pEdge->mSource->mIndex;
        ui4 const j = pEdge->mTarget->mIndex;
        ui4 const weight = LEdgeInfo_UI4At (pEdgeWeights, pEdge);
        distanceMat[i * nodeCount + j] = weight;
        }

    for (ui4 i = 0; i < nodeCount; ++i)
        distanceMat[i * nodeCount + i] = 0;

    LGraph_TNode *pNode_k, *pNode_i, *pNode_j;
    LGraph_ForAllNodes (pGraph, pNode_k)
        {
        LGraph_ForAllNodes (pGraph, pNode_i)
            {
            LGraph_ForAllNodes (pGraph, pNode_j)
                {
                ui4 const ij = pNode_i->mIndex * nodeCount + pNode_j->mIndex;
                ui4 const ik = pNode_i->mIndex * nodeCount + pNode_k->mIndex;
                ui4 const kj = pNode_k->mIndex * nodeCount + pNode_j->mIndex;
                ui4 const dist_ik = distanceMat[ik];
                ui4 const dist_kj = distanceMat[kj];

                if (dist_ik != FLOYD_INFINITY && dist_kj != FLOYD_INFINITY && distanceMat[ij] > dist_ik + dist_kj)
                    distanceMat[ij] = dist_ik + dist_kj;
                }
            }
        }
    return distanceMat;
    }
