#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <cstddef>

#ifdef DEBUG

#include "time.h"

#define DEBUG_SLOTS 64

struct DebugData
{
    clock_t startTime;
    clock_t totalTime{0};
    size_t callCount{0};
};

#endif

enum DebugIDs
{
    CLEAR_COLOR_BUFFER,
    CLEAR_DEPTH_BUFFER,
    DRAW,
    BUFFER_ALLOCATION,
    VERTEX_SHADING,
    RASTERIZATION,
    FRAGMENT_SHADING,
    BUFFER_FREE
};

void DebugStart(size_t id);
void DebugEnd(size_t id);
void PrintDebugDataAndReset();

#endif