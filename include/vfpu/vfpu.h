#ifndef VFPU_H
#define VFPU_H

#define VFPU_ALIGN __attribute__((aligned(16)))

#define VFPU_SCALAR(MATRIX, ROW, COLUMN) "S" #MATRIX #COLUMN #ROW
#define VFPU_COLUMN(MATRIX, ROW, COLUMN) "C" #MATRIX #COLUMN #ROW
#define VFPU_ROW(MATRIX, ROW, COLUMN) "R" #MATRIX #COLUMN #ROW
#define VFPU_MATRIX(MATRIX, ROW, COLUMN) "M" #MATRIX #COLUMN #ROW
#define VFPU_MATRIX_T(MATRIX, ROW, COLUMN) "E" #MATRIX #COLUMN #ROW

#define VFPU_V1 "s"
#define VFPU_V2 "p"
#define VFPU_V3 "t"
#define VFPU_V4 "q"

/**
 *  TODO: PSPSDK used "m" instead of "r"
 */
#define VFPU_INST_MEMORY(OP_CODE, SIZE, REG, PTR, OFFSET)              \
    {                                                                  \
        asm(OP_CODE "." SIZE " " REG ", " #OFFSET "(%0);" ::"r"(PTR)); \
    }

#define VFPU_INST_NO_OPERANDS(OP_CODE, SIZE, DST) \
    {                                                  \
        asm(OP_CODE "." SIZE " " DST ";");             \
    }

#define VFPU_INST_UNARY(OP_CODE, SIZE, DST, SRC)    \
    {                                               \
        asm(OP_CODE "." SIZE " " DST ", " SRC ";"); \
    }

#define VFPU_INST_BINARY(OP_CODE, SIZE, DST, SRC1, SRC2)       \
    {                                                          \
        asm(OP_CODE "." SIZE " " DST ", " SRC1 ", " SRC2 ";"); \
    }

#endif