#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <cstddef>

#ifdef DEBUG

#include "time.h"

#define DEBUG_SLOTS 64

struct DebugData
{
    size_t slot;
    char *text;
    clock_t startTime;
    clock_t totalTime{0};
    size_t callCount{0};
};

#endif

enum DebugIDs
{
    CLEAR_COLOR_BUFFER,
    CLEAR_DEPTH_BUFFER,
    SWAP_BUFFERS,
    DRAW,
    BUFFER_ALLOCATION,
    VERTEX_SHADING,
    RASTERIZATION,
    FRAGMENT_SHADING,
    BUFFER_FREE,

    UPLOAD_COORDINATES,
    INIT_EDGES,
    BBOX
};

void DebugInit();
void DebugStart(size_t id);
void DebugEnd(size_t id);
void PrintDebugDataAndReset();

#endif