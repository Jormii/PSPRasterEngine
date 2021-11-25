#ifndef VFPU_HPP

#define VFPU_ALIGN __attribute__((aligned(16)))

/***
 * OVERALL NOTES
 * M, R, C stand for Matrix, Row and Column
 * PTR is a pointer
 * 
 * VFPU's registers format is MCR
 */

/***
 * SCALAR OPERATIONS
 */

#define LOAD_SCALAR(M, R, C, PTR)        \
    {                                    \
        asm(                             \
            "lv.s S" #M #C #R ", 0(%0);" \
            :                            \
            : "r"(PTR)                   \
            :);                          \
    }

#define STORE_SCALAR(M, R, C, PTR)       \
    {                                    \
        asm(                             \
            "sv.s S" #M #C #R ", 0(%0);" \
            :                            \
            : "r"(PTR)                   \
            :);                          \
    }

/***
 * VEC2 OPERATIONS
 */

// L/R - T/B stand for Left/Right - Top/Bottom
//  - L/R: When writing rows
//  - T/B: When writing columns
// TH/BH stand for TopHalf and BottomHalf

#define ZERO_VEC2_ROW_L(M, R)       \
    {                               \
        asm("vzero.p R" #M "0" #R); \
    }

#define ZERO_VEC2_ROW_R(M, R)       \
    {                               \
        asm("vzero.p R" #M "2" #R); \
    }

#define LOAD_VEC2_COL_U(M, C, PTR)     \
    {                                  \
        asm(                           \
            "lv.s S" #M #C "0, 0(%0);" \
            "lv.s S" #M #C "1, 4(%0);" \
            :                          \
            : "r"(PTR)                 \
            :);                        \
    }

#define STORE_VEC2_COL_U(M, C, PTR)    \
    {                                  \
        asm(                           \
            "sv.s S" #M #C "0, 0(%0);" \
            "sv.s S" #M #C "1, 4(%0);" \
            :                          \
            : "r"(PTR)                 \
            :);                        \
    }

#define LOAD_VEC2_ROW_L(M, R, PTR)        \
    {                                     \
        asm(                              \
            "lv.s S" #M "0" #R ", 0(%0);" \
            "lv.s S" #M "1" #R ", 4(%0);" \
            :                             \
            : "r"(PTR)                    \
            :);                           \
    }

#define LOAD_VEC2_ROW_R(M, R, PTR)        \
    {                                     \
        asm(                              \
            "lv.s S" #M "2" #R ", 0(%0);" \
            "lv.s S" #M "3" #R ", 4(%0);" \
            :                             \
            : "r"(PTR)                    \
            :);                           \
    }

/***
 * VEC3 OPERATIONS
 */

#define LOAD_VEC3_COL(M, C, PTR)       \
    {                                  \
        asm(                           \
            "lv.s S" #M #C "0, 0(%0);" \
            "lv.s S" #M #C "1, 4(%0);" \
            "lv.s S" #M #C "2, 8(%0);" \
            :                          \
            : "r"(PTR)                 \
            :);                        \
    }

#define STORE_VEC3_COL(M, C, PTR)      \
    {                                  \
        asm(                           \
            "sv.s S" #M #C "0, 0(%0);" \
            "sv.s S" #M #C "1, 4(%0);" \
            "sv.s S" #M #C "2, 8(%0);" \
            :                          \
            : "r"(PTR)                 \
            :);                        \
    }

/***
 * VEC4 OPERATIONS
 */

#define LOAD_VEC4_COL(M, C, PTR)       \
    {                                  \
        asm(                           \
            "lv.q C" #M #C "0, 0(%0);" \
            :                          \
            : "r"(PTR)                 \
            :);                        \
    }

#define STORE_VEC4_ROW(M, R, PTR)         \
    {                                     \
        asm(                              \
            "sv.q R" #M "0" #R ", 0(%0);" \
            :                             \
            : "r"(PTR)                    \
            :);                           \
    }

#define STORE_VEC4_COL(M, C, PTR)      \
    {                                  \
        asm(                           \
            "sv.q C" #M #C "0, 0(%0);" \
            :                          \
            : "r"(PTR)                 \
            :);                        \
    }

#endif