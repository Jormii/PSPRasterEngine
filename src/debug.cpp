#include "debug.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

#ifdef DEBUG
const char *debugIDsName[]{
    "CLEAR_COLOR_BUFFER",
    "CLEAR_DEPTH_BUFFER",
    "SWAP_BUFFERS",
    "DRAW",
    "BUFFER_ALLOCATION",
    "VERTEX_SHADING",
    "RASTERIZATION",
    "RASTERIZATION::SCREEN_SPACE",
    "RASTERIZATION::TRIANGLE_VISIBILITY",
    "RASTERIZATION::RASTERIZE_TRIANGLE",
    "RASTERIZATION::TRIANGLE_VISIBILITY::EDGE_FUNCTIONS",
    "RASTERIZATION::TRIANGLE_VISIBILITY::EDGE_MASKS",
    "RASTERIZATION::TRIANGLE_VISIBILITY::BBOX",
    "RASTERIZATION::TRIANGLE_VISIBILITY::WITHIN_TRIANGLE",
    "RASTERIZATION::TRIANGLE_VISIBILITY::BARY_COORDS",
    "RASTERIZATION::TRIANGLE_VISIBILITY::INTERPOLATION",
    "FRAGMENT_SHADING",
    "BUFFER_FREE"};

bool debugInit{false};
DebugData debugData[DEBUG_SLOTS];
#endif

void DebugInit()
{
#ifdef DEBUG
    debugInit = true;
    for (size_t i{0}; i < DEBUG_SLOTS; ++i)
    {
        debugData[i].slot = i;
    }
#endif
}

void DebugStart(size_t id)
{
#ifdef DEBUG
    if (!debugInit)
    {
        DebugInit();
    }
    debugData[id].startTime = clock();
#endif
}

void DebugEnd(size_t id)
{
#ifdef DEBUG
    debugData[id].totalTime += clock() - debugData[id].startTime;
    debugData[id].callCount += 1;
#endif
}

#ifdef DEBUG
struct DebugDataCompare
{
    inline bool operator()(const DebugData *db1, const DebugData *db2)
    {
        return db1->totalTime > db2->totalTime;
    }
};
#endif

void PrintDebugDataAndReset()
{
#ifdef DEBUG
#define CLOCK_SEC (float(CLOCKS_PER_SEC))
    std::vector<DebugData *> debugVec;
    for (size_t i{0}; i < DEBUG_SLOTS; ++i)
    {
        if (debugData[i].callCount != 0)
        {
            debugVec.push_back(debugData + i);
        }
    }

    std::sort(debugVec.begin(), debugVec.end(), DebugDataCompare());

    std::cout << "\n***DEBUG_DATA***:\n";

    clock_t totalCycles{0};
    for (size_t i{0}; i < debugVec.size(); ++i)
    {
        DebugData *d{debugVec[i]};
        std::cout << debugIDsName[d->slot] << "\n";
        std::cout
            << "Time: " << d->totalTime
            << " ; Calls: " << d->callCount
            << " ; Time/Call: " << d->totalTime / d->callCount << "\n";

        totalCycles += d->totalTime;

        d->startTime = 0;
        d->totalTime = 0;
        d->callCount = 0;
    }

    std::cout << "TOTAL TIME: " << totalCycles << "\n";

#endif
}