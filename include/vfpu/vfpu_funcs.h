#ifndef VFPU_COMPLEX_H
#define VFPU_COMPLEX_H

#include "vfpu.h"
#include "vfpu_ops.h"

// zero(x) = 1 - abs(sign(x))
#define VFPU_FUN_EQ_ZERO(SIZE, SRC, DST)                            \
    {                                                               \
        VFPU_INST_UNARY(VFPU_OP_V_SIGN, SIZE, "C700", SRC);         \
        VFPU_INST_UNARY(VFPU_OP_V_ABS, SIZE, "C700", "C700");       \
        VFPU_INST_NO_OPERANDS(VFPU_OP_V_ONE, SIZE, "C730");         \
        VFPU_INST_BINARY(VFPU_OP_V_SUB, SIZE, DST, "C730", "C700"); \
    }

/**
 *  gt_zero(x) = (1 - zero(x))*u(x)
 * u(x) = (x + abs(x)) / 2x
 * 
 * C730 is a 1-filled vector. Set in the VFPU_FUN_EQ_ZERO call
 * VFPU_OP_V_MAX requiered to avoid -inf problems
 */
#define VFPU_FUN_GT_ZERO(SIZE, SRC, ZERO_DST, GT_ZERO_DST)                        \
    {                                                                             \
        VFPU_FUN_EQ_ZERO(SIZE, SRC, ZERO_DST);                                    \
                                                                                  \
        VFPU_INST_BINARY(VFPU_OP_V_SUB, SIZE, GT_ZERO_DST, "C730", ZERO_DST);     \
                                                                                  \
        VFPU_INST_UNARY(VFPU_OP_V_ABS, SIZE, "C700", SRC);                        \
        VFPU_INST_BINARY(VFPU_OP_V_ADD, SIZE, "C700", SRC, "C700");               \
        VFPU_INST_BINARY(VFPU_OP_V_ADD, SIZE, "C710", SRC, SRC);                  \
        VFPU_INST_BINARY(VFPU_OP_V_DIV, SIZE, "C720", "C700", "C710");            \
                                                                                  \
        VFPU_INST_NO_OPERANDS(VFPU_OP_V_ZERO, SIZE, "C730");                      \
        VFPU_INST_BINARY(VFPU_OP_V_MAX, SIZE, "C720", "C720", "C730");            \
                                                                                  \
        VFPU_INST_BINARY(VFPU_OP_V_MULT, SIZE, GT_ZERO_DST, GT_ZERO_DST, "C720"); \
    }

// gte_zero(x) = max{zero(x), ge(x)}
#define VFPU_FUN_GTE_ZERO(SIZE, SRC, ZERO_DST, GTE_ZERO_DST)                         \
    {                                                                                \
        VFPU_FUN_GT_ZERO(SIZE, SRC, ZERO_DST, GTE_ZERO_DST);                         \
        VFPU_INST_BINARY(VFPU_OP_V_MAX, SIZE, GTE_ZERO_DST, ZERO_DST, GTE_ZERO_DST); \
    }

/***
 * LOAD/STORES
 */

#define VFPU_FUN_LOAD_V2_ROW_C01(MATRIX, ROW, PTR)                                   \
    {                                                                                \
        VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, VFPU_SCALAR(MATRIX, ROW, 0), PTR, 0) \
        VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, VFPU_SCALAR(MATRIX, ROW, 1), PTR, 4) \
    }

#define VFPU_FUN_LOAD_V2_COL_R01(MATRIX, COLUMN, PTR)                                   \
    {                                                                                   \
        VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, VFPU_SCALAR(MATRIX, 0, COLUMN), PTR, 0) \
        VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, VFPU_SCALAR(MATRIX, 1, COLUMN), PTR, 4) \
    }

#define VFPU_FUN_STORE_V2_COL_R01(MATRIX, COLUMN, PTR)                                   \
    {                                                                                    \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, VFPU_SCALAR(MATRIX, 0, COLUMN), PTR, 0) \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, VFPU_SCALAR(MATRIX, 1, COLUMN), PTR, 4) \
    }

#define VFPU_FUN_LOAD_V3_COL(MATRIX, COLUMN, PTR)                                       \
    {                                                                                   \
        VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, VFPU_SCALAR(MATRIX, 0, COLUMN), PTR, 0) \
        VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, VFPU_SCALAR(MATRIX, 1, COLUMN), PTR, 4) \
        VFPU_INST_MEMORY(VFPU_OP_LOAD, VFPU_V1, VFPU_SCALAR(MATRIX, 2, COLUMN), PTR, 8) \
    }

#define VFPU_FUN_STORE_V3_COL(MATRIX, COLUMN, PTR)                                       \
    {                                                                                    \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, VFPU_SCALAR(MATRIX, 0, COLUMN), PTR, 0) \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, VFPU_SCALAR(MATRIX, 1, COLUMN), PTR, 4) \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, VFPU_SCALAR(MATRIX, 2, COLUMN), PTR, 8) \
    }

#define VFPU_FUN_STORE_V3_ROW(MATRIX, ROW, PTR)                                       \
    {                                                                                 \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, VFPU_SCALAR(MATRIX, ROW, 0), PTR, 0) \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, VFPU_SCALAR(MATRIX, ROW, 1), PTR, 4) \
        VFPU_INST_MEMORY(VFPU_OP_STORE, VFPU_V1, VFPU_SCALAR(MATRIX, ROW, 2), PTR, 8) \
    }

#endif