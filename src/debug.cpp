#include "debug.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

#ifdef DEBUG
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

    debugData[DebugIDs::CLEAR_COLOR_BUFFER].text = "CLEAR COLOR BUFFER";
    debugData[DebugIDs::CLEAR_DEPTH_BUFFER].text = "CLEAR DEPTH BUFFER";
    debugData[DebugIDs::SWAP_BUFFERS].text = "SWAP BUFFERS";
    debugData[DebugIDs::DRAW].text = "DRAW CALL";
    debugData[DebugIDs::BUFFER_ALLOCATION].text = "BUFFER ALLOCATION";
    debugData[DebugIDs::VERTEX_SHADING].text = "VERTEX SHADING";
    debugData[DebugIDs::RASTERIZATION].text = "RASTERIZATION";
    debugData[DebugIDs::FRAGMENT_SHADING].text = "FRAGMENT_SHADING";
    debugData[DebugIDs::BUFFER_FREE].text = "FREE BUFFER";

    debugData[DebugIDs::UPLOAD_COORDINATES].text = "RASTER::Upload coords";
    debugData[DebugIDs::INIT_EDGES].text = "RASTER::Init edges";
    debugData[DebugIDs::BBOX].text = "RASTER::BBOX";
    debugData[DebugIDs::VFPU_RASTERIZATION_INIT].text = "RASTER::VFPU_RASTERIZATION_INIT";
    debugData[DebugIDs::TRIANGLE_VISIBILITY].text = "RASTER::TRIANGLE_VISIBILITY";
    debugData[DebugIDs::RASTERIZE_TRIANGLE].text = "RASTER::RASTERIZE_TRIANGLE";
    debugData[DebugIDs::TRIANGLE_TRAVERSAL].text = "RASTER::TRIANGLE_TRAVERSAL";
    debugData[DebugIDs::EVALUATE_EDGE_FUNCTION].text = "RASTER::EVALUATE_EDGE_FUNCTION";
    debugData[DebugIDs::INSIDE_TRIANGLE].text = "RASTER::INSIDE_TRIANGLE";
    debugData[DebugIDs::BARY_COORDS].text = "RASTER::BARY_COORDS";
    debugData[DebugIDs::INTERPOLATE].text = "RASTER::INTERPOLATE";
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
    if (!debugInit)
    {
        return;
    }

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
    if (!debugInit)
    {
        return;
    }

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
        std::cout << d->text << "\n";
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