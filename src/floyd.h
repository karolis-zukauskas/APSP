#pragma once
extern "C"
    {
#include "LType.h"
#include "LGraph.h"
#include "LEdgeInfo.h"
    }

ui4* APSP_Floyd (LGraph* pGraph, LEdgeInfo* pEdgeWeights);
