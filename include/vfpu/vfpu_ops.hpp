#ifndef VFPU_OP_H
#define VFPU_OP_H

/**
 * LOAD/STORE
 */
#define VFPU_OP_LOAD "lv"
#define VFPU_OP_STORE "sv"

/**
 * VECTOR
 */
// Zero
#define VFPU_OP_V_ZERO "vzero"
#define VFPU_OP_V_ONE "vone"

// Unary
#define VFPU_OP_V_NEG "vneg"
#define VFPU_OP_V_ABS "vabs"
#define VFPU_OP_V_SIGN "vsgn"

// Binary
#define VFPU_OP_V_ADD "vadd"
#define VFPU_OP_V_SUB "vsub"
#define VFPU_OP_V_MULT "vmul"
#define VFPU_OP_V_DIV "vdiv"

#define VFPU_OP_V_MIN "vmin"
#define VFPU_OP_V_MAX "vmax"

/**
 * MATRIX
 */
// Zero
#define VFPU_OP_M_ZERO "vmzero"

// Binary
#define VFPU_OP_M_MULT "vmmul"

#endif