#include "debug.hpp"

#include <iostream>

#ifdef DEBUG
DebugData debugData[DEBUG_SLOTS];

const char *debugIDsName[]{
    "CLEAR_COLOR_BUFFER",
    "CLEAR_DEPTH_BUFFER",
    "DRAW",
    "BUFFER_ALLOCATION",
    "VERTEX_SHADING",
    "RASTERIZATION",
    "FRAGMENT_SHADING",
    "BUFFER_FREE"};
#endif

void DebugStart(size_t id)
{
#ifdef DEBUG
    debugData[id].startTime = clock();
#endif
}

void DebugEnd(size_t id)
{
#ifdef DEBUG
    debugData[id].totalTime = clock() - debugData[id].startTime;
    debugData[id].callCount += 1;
#endif
}

void PrintDebugDataAndReset()
{
#ifdef DEBUG
#define CLOCK_SEC (float(CLOCKS_PER_SEC))
    std::cout << "\n***DEBUG_DATA***:\n";

    clock_t totalCycles{0};
    for (size_t i{0}; i < DEBUG_SLOTS; ++i)
    {
        if (debugData[i].callCount == 0)
        {
            continue;
        }

        DebugData &d{debugData[i]};
        std::cout << debugIDsName[i] << "\n";
        std::cout
            << "Time: " << d.totalTime / CLOCK_SEC
            << " ; Calls: " << d.callCount
            << " ; Time/Call: " << (d.totalTime / CLOCK_SEC) / d.callCount << "\n";

        totalCycles += d.totalTime;

        d.startTime = 0;
        d.totalTime = 0;
        d.callCount = 0;
    }

    std::cout << "TOTAL TIME: " << totalCycles / CLOCK_SEC << "\n";

#endif
}