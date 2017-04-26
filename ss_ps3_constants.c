/**
 * Implementation of PS3 constants file
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include "ss_ps3_constants.h"

//  DEFINES ===================================================================

//  PS3 CANVAS  ---------------------------------------------------------------

// circular buttons
#define PS3_BTN_RADIUS  25
#define PS3_BTN_VERT_X  632 // A and Y
#define PS3_BTN_HORZ_Y  498 // B and X
#define PS3_BTN_A_Y     556
#define PS3_BTN_Y_Y     439
#define PS3_BTN_B_X     696
#define PS3_BTN_X_X     570

// select
#define PS3_BTN_BCK_X   324
#define PS3_BTN_BCK_Y   489
#define PS3_BTN_BCK_W   31
#define PS3_BTN_BCK_H   16

// start
// clockwise vertices starting fromthe point
#define PS3_BTN_STRT_X          496
#define PS3_BTN_STRT_Y          497
#define PS3_BTN_STRT_X0_OFF     0
#define PS3_BTN_STRT_X1_OFF     -36
#define PS3_BTN_STRT_Y0_OFF     0
#define PS3_BTN_STRT_Y1_OFF     10
#define PS3_BTN_STRT_Y2_OFF     -10

// start button values
#define PS3_BTN_STRT_X0 PS3_BTN_STRT_X + PS3_BTN_STRT_X0_OFF
#define PS3_BTN_STRT_X1 PS3_BTN_STRT_X + PS3_BTN_STRT_X1_OFF
#define PS3_BTN_STRT_X2 PS3_BTN_STRT_X1
#define PS3_BTN_STRT_Y0 PS3_BTN_STRT_Y + PS3_BTN_STRT_Y0_OFF
#define PS3_BTN_STRT_Y1 PS3_BTN_STRT_Y + PS3_BTN_STRT_Y1_OFF
#define PS3_BTN_STRT_Y2 PS3_BTN_STRT_Y + PS3_BTN_STRT_Y2_OFF

// thumbs
#define PS3_BTN_THM_Y   608
#define PS3_BTN_THM_R   45
#define PS3_BTN_LTHM_X  292
#define PS3_BTN_RTHM_X  522

// shoulders
#define PS3_BTN_SH_W    89
#define PS3_BTN_SH_H    45
#define PS3_BTN_SH_Y    191
#define PS3_BTN_LSH_X   139
#define PS3_BTN_RSH_X   587

// dpads
#define PS3_BTN_DPC_X       181 // dpad center x
#define PS3_BTN_DPC_Y       497 // dpad center y
#define PS3_BTN_DP_SP       26  // spacing value between dpad pts
#define PS3_BTN_DP_TR_OFF   17  // triangular offset
#define PS3_BTN_DP_BX_L_OFF 42  // length box offset
#define PS3_BTN_DP_BX_W_OFF 18  // width box offset

// dpads negative varients
#define PS3_BTN_DP_TR_OFF_N     (-1 * PS3_BTN_DP_TR_OFF) 
#define PS3_BTN_DP_BX_L_OFF_N   (-1 * PS3_BTN_DP_BX_L_OFF)
#define PS3_BTN_DP_BX_W_OFF_N   (-1 * PS3_BTN_DP_BX_W_OFF)

// dpads spacing varients
#define PS3_BTN_DP_SP_HALF      (PS3_BTN_DP_SP / 2)
#define PS3_BTN_DP_SP_HALF_N    (-1 * PS3_BTN_DP_SP_HALF)

// dpads specific points
// dpad up
#define PS3_BTN_UDP_X0  PS3_BTN_DPC_X
#define PS3_BTN_UDP_X1  PS3_BTN_UDP_X0 + PS3_BTN_DP_BX_W_OFF_N
#define PS3_BTN_UDP_X2  PS3_BTN_UDP_X1
#define PS3_BTN_UDP_X3  PS3_BTN_UDP_X0 + PS3_BTN_DP_BX_W_OFF
#define PS3_BTN_UDP_X4  PS3_BTN_UDP_X3
#define PS3_BTN_UDP_Y0  PS3_BTN_DPC_Y + PS3_BTN_DP_SP_HALF_N
#define PS3_BTN_UDP_Y1  PS3_BTN_UDP_Y0 + PS3_BTN_DP_TR_OFF_N
#define PS3_BTN_UDP_Y2  PS3_BTN_UDP_Y0 + PS3_BTN_DP_BX_L_OFF_N
#define PS3_BTN_UDP_Y3  PS3_BTN_UDP_Y2
#define PS3_BTN_UDP_Y4  PS3_BTN_UDP_Y1

// dpad down
#define PS3_BTN_DDP_X0  PS3_BTN_DPC_X
#define PS3_BTN_DDP_X1  PS3_BTN_UDP_X3
#define PS3_BTN_DDP_X2  PS3_BTN_DDP_X1
#define PS3_BTN_DDP_X3  PS3_BTN_UDP_X1
#define PS3_BTN_DDP_X4  PS3_BTN_DDP_X3
#define PS3_BTN_DDP_Y0  PS3_BTN_DPC_Y + PS3_BTN_DP_SP_HALF
#define PS3_BTN_DDP_Y1  PS3_BTN_DDP_Y0 + PS3_BTN_DP_TR_OFF
#define PS3_BTN_DDP_Y2  PS3_BTN_DDP_Y0 + PS3_BTN_DP_BX_L_OFF
#define PS3_BTN_DDP_Y3  PS3_BTN_DDP_Y2
#define PS3_BTN_DDP_Y4  PS3_BTN_DDP_Y1

// dpad left
#define PS3_BTN_LDP_X0  PS3_BTN_DPC_X + PS3_BTN_DP_SP_HALF_N
#define PS3_BTN_LDP_X1  PS3_BTN_LDP_X0 + PS3_BTN_DP_TR_OFF_N
#define PS3_BTN_LDP_X2  PS3_BTN_LDP_X0 + PS3_BTN_DP_BX_L_OFF_N
#define PS3_BTN_LDP_X3  PS3_BTN_LDP_X2
#define PS3_BTN_LDP_X4  PS3_BTN_LDP_X1
#define PS3_BTN_LDP_Y0  PS3_BTN_DPC_Y
#define PS3_BTN_LDP_Y1  PS3_BTN_LDP_Y0 + PS3_BTN_DP_BX_W_OFF
#define PS3_BTN_LDP_Y2  PS3_BTN_LDP_Y1
#define PS3_BTN_LDP_Y3  PS3_BTN_LDP_Y0 + PS3_BTN_DP_BX_W_OFF_N
#define PS3_BTN_LDP_Y4  PS3_BTN_LDP_Y3

// dpad right
#define PS3_BTN_RDP_X0  PS3_BTN_DPC_X + PS3_BTN_DP_SP_HALF
#define PS3_BTN_RDP_X1  PS3_BTN_RDP_X0 + PS3_BTN_DP_TR_OFF
#define PS3_BTN_RDP_X2  PS3_BTN_RDP_X0 + PS3_BTN_DP_BX_L_OFF
#define PS3_BTN_RDP_X3  PS3_BTN_RDP_X2
#define PS3_BTN_RDP_X4  PS3_BTN_RDP_X1
#define PS3_BTN_RDP_Y0  PS3_BTN_DPC_Y
#define PS3_BTN_RDP_Y1  PS3_BTN_LDP_Y3
#define PS3_BTN_RDP_Y2  PS3_BTN_RDP_Y1
#define PS3_BTN_RDP_Y3  PS3_BTN_LDP_Y1
#define PS3_BTN_RDP_Y4  PS3_BTN_RDP_Y3

// triggers
#define PS3_TRG_Y           76 // y coordinate of tip
#define PS3_TRG_W           PS3_BTN_SH_W
#define PS3_TRG_H           60
#define PS3_TRG_RX          (PS3_TRG_W / 2)
#define PS3_TRG_RY          16
#define PS3_TRG_B_Y         PS3_TRG_Y + PS3_TRG_RY
#define PS3_TRG_T_Y         PS3_TRG_B_Y

// trigger left
#define PS3_TRG_L_B_X   PS3_BTN_LSH_X
#define PS3_TRG_L_T_X   PS3_TRG_L_B_X + PS3_TRG_RX

// trigger right
#define PS3_TRG_R_B_X   PS3_BTN_RSH_X
#define PS3_TRG_R_T_X   PS3_TRG_R_B_X + PS3_TRG_RX

// joysticks (unit circle px)
// the following two values are the unit circle x and y coordinates
// these coordinates are considered the start of slice 0
// all remaing coordinates manipulations of these coordinates
#define PS3_JOY_UC_X    71
#define PS3_JOY_UC_Y    29

// joystick unit circle variants
#define PS3_JOY_UC_X_N  (-1 * PS3_JOY_UC_X)
#define PS3_JOY_UC_Y_N  (-1 * PS3_JOY_UC_Y)

// joystick slice pts (coordinate offsets where slices begin and end)
// they start above the x axis and move in a clockwise direction
#define PS3_JOY_SL0_X_OFF   PS3_JOY_UC_X
#define PS3_JOY_SL1_X_OFF   PS3_JOY_UC_X
#define PS3_JOY_SL2_X_OFF   PS3_JOY_UC_Y
#define PS3_JOY_SL3_X_OFF   PS3_JOY_UC_Y_N
#define PS3_JOY_SL4_X_OFF   PS3_JOY_UC_X_N
#define PS3_JOY_SL5_X_OFF   PS3_JOY_UC_X_N
#define PS3_JOY_SL6_X_OFF   PS3_JOY_UC_Y_N
#define PS3_JOY_SL7_X_OFF   PS3_JOY_UC_Y
#define PS3_JOY_SL0_Y_OFF   PS3_JOY_UC_Y
#define PS3_JOY_SL1_Y_OFF   PS3_JOY_UC_Y_N
#define PS3_JOY_SL2_Y_OFF   PS3_JOY_UC_X_N
#define PS3_JOY_SL3_Y_OFF   PS3_JOY_UC_X_N
#define PS3_JOY_SL4_Y_OFF   PS3_JOY_UC_Y_N
#define PS3_JOY_SL5_Y_OFF   PS3_JOY_UC_Y
#define PS3_JOY_SL6_Y_OFF   PS3_JOY_UC_X
#define PS3_JOY_SL7_Y_OFF   PS3_JOY_UC_X

// joystick centers
#define PS3_JOY_Y   PS3_BTN_THM_Y
#define PS3_JOY_L_X PS3_BTN_LTHM_X
#define PS3_JOY_R_X PS3_BTN_RTHM_X

// joystick slices left
#define PS3_LJOY_SL0_X  PS3_JOY_L_X + PS3_JOY_SL0_X_OFF
#define PS3_LJOY_SL1_X  PS3_JOY_L_X + PS3_JOY_SL1_X_OFF
#define PS3_LJOY_SL2_X  PS3_JOY_L_X + PS3_JOY_SL2_X_OFF
#define PS3_LJOY_SL3_X  PS3_JOY_L_X + PS3_JOY_SL3_X_OFF
#define PS3_LJOY_SL4_X  PS3_JOY_L_X + PS3_JOY_SL4_X_OFF
#define PS3_LJOY_SL5_X  PS3_JOY_L_X + PS3_JOY_SL5_X_OFF
#define PS3_LJOY_SL6_X  PS3_JOY_L_X + PS3_JOY_SL6_X_OFF
#define PS3_LJOY_SL7_X  PS3_JOY_L_X + PS3_JOY_SL7_X_OFF
#define PS3_LJOY_SL0_Y  PS3_JOY_Y + PS3_JOY_SL0_Y_OFF
#define PS3_LJOY_SL1_Y  PS3_JOY_Y + PS3_JOY_SL1_Y_OFF
#define PS3_LJOY_SL2_Y  PS3_JOY_Y + PS3_JOY_SL2_Y_OFF
#define PS3_LJOY_SL3_Y  PS3_JOY_Y + PS3_JOY_SL3_Y_OFF
#define PS3_LJOY_SL4_Y  PS3_JOY_Y + PS3_JOY_SL4_Y_OFF
#define PS3_LJOY_SL5_Y  PS3_JOY_Y + PS3_JOY_SL5_Y_OFF
#define PS3_LJOY_SL6_Y  PS3_JOY_Y + PS3_JOY_SL6_Y_OFF
#define PS3_LJOY_SL7_Y  PS3_JOY_Y + PS3_JOY_SL7_Y_OFF

// joystick slices right
#define PS3_RJOY_SL0_X  PS3_JOY_R_X + PS3_JOY_SL0_X_OFF
#define PS3_RJOY_SL1_X  PS3_JOY_R_X + PS3_JOY_SL1_X_OFF
#define PS3_RJOY_SL2_X  PS3_JOY_R_X + PS3_JOY_SL2_X_OFF
#define PS3_RJOY_SL3_X  PS3_JOY_R_X + PS3_JOY_SL3_X_OFF
#define PS3_RJOY_SL4_X  PS3_JOY_R_X + PS3_JOY_SL4_X_OFF
#define PS3_RJOY_SL5_X  PS3_JOY_R_X + PS3_JOY_SL5_X_OFF
#define PS3_RJOY_SL6_X  PS3_JOY_R_X + PS3_JOY_SL6_X_OFF
#define PS3_RJOY_SL7_X  PS3_JOY_R_X + PS3_JOY_SL7_X_OFF
#define PS3_RJOY_SL0_Y  PS3_JOY_Y + PS3_JOY_SL0_Y_OFF
#define PS3_RJOY_SL1_Y  PS3_JOY_Y + PS3_JOY_SL1_Y_OFF
#define PS3_RJOY_SL2_Y  PS3_JOY_Y + PS3_JOY_SL2_Y_OFF
#define PS3_RJOY_SL3_Y  PS3_JOY_Y + PS3_JOY_SL3_Y_OFF
#define PS3_RJOY_SL4_Y  PS3_JOY_Y + PS3_JOY_SL4_Y_OFF
#define PS3_RJOY_SL5_Y  PS3_JOY_Y + PS3_JOY_SL5_Y_OFF
#define PS3_RJOY_SL6_Y  PS3_JOY_Y + PS3_JOY_SL6_Y_OFF
#define PS3_RJOY_SL7_Y  PS3_JOY_Y + PS3_JOY_SL7_Y_OFF

//  CONSTS  ===================================================================

//  PS3 CANVAS  ---------------------------------------------------------------

// button X (A)
const unsigned int SS_PS3_A_X   = PS3_BTN_VERT_X;
const unsigned int SS_PS3_A_Y   = PS3_BTN_A_Y;
const unsigned int SS_PS3_A_R   = PS3_BTN_RADIUS;

// button Circle (B)
const unsigned int SS_PS3_B_X   = PS3_BTN_B_X;
const unsigned int SS_PS3_B_Y   = PS3_BTN_HORZ_Y;
const unsigned int SS_PS3_B_R   = PS3_BTN_RADIUS;

// button Square (X)
const unsigned int SS_PS3_X_X   = PS3_BTN_X_X;
const unsigned int SS_PS3_X_Y   = PS3_BTN_HORZ_Y;
const unsigned int SS_PS3_X_R   = PS3_BTN_RADIUS;

// button Triangle (Y)
const unsigned int SS_PS3_Y_X   = PS3_BTN_VERT_X;
const unsigned int SS_PS3_Y_Y   = PS3_BTN_Y_Y;
const unsigned int SS_PS3_Y_R   = PS3_BTN_RADIUS;

// button Select (Back)
const unsigned int SS_PS3_BCK_X = PS3_BTN_BCK_X;
const unsigned int SS_PS3_BCK_Y = PS3_BTN_BCK_Y;
const unsigned int SS_PS3_BCK_W = PS3_BTN_BCK_W;
const unsigned int SS_PS3_BCK_H = PS3_BTN_BCK_H;

// button Start (Start)
const Sint16 SS_PS3_STRT_VX[]   = {
    PS3_BTN_STRT_X0,
    PS3_BTN_STRT_X1,
    PS3_BTN_STRT_X2
};
const Sint16 SS_PS3_STRT_VY[]   = {
    PS3_BTN_STRT_Y0,
    PS3_BTN_STRT_Y1,
    PS3_BTN_STRT_Y2
};
//const unsigned int SS_PS3_STRT_X    = 496;
//const unsigned int SS_PS3_STRT_Y    = 497;
//const int SS_PS3_STRT_VX_OFF[]      = {0, -36, -36};
//const int SS_PS3_STRT_VY_OFF[]      = {0, +10, -10};

// button Left Thumb
const unsigned int SS_PS3_LTHM_X    = PS3_BTN_LTHM_X;
const unsigned int SS_PS3_LTHM_Y    = PS3_BTN_THM_Y;
const unsigned int SS_PS3_LTHM_R    = PS3_BTN_THM_R;

// button Right Thumb
const unsigned int SS_PS3_RTHM_X    = PS3_BTN_RTHM_X;
const unsigned int SS_PS3_RTHM_Y    = PS3_BTN_THM_Y;
const unsigned int SS_PS3_RTHM_R    = PS3_BTN_THM_R;

// button Left Shoulder
const unsigned int SS_PS3_LSH_X = PS3_BTN_LSH_X;
const unsigned int SS_PS3_LSH_Y = PS3_BTN_SH_Y;
const unsigned int SS_PS3_LSH_W = PS3_BTN_SH_W;
const unsigned int SS_PS3_LSH_H = PS3_BTN_SH_H;

// button Right Shoulder
const unsigned int SS_PS3_RSH_X = PS3_BTN_RSH_X;
const unsigned int SS_PS3_RSH_Y = PS3_BTN_SH_Y;
const unsigned int SS_PS3_RSH_W = PS3_BTN_SH_W;
const unsigned int SS_PS3_RSH_H = PS3_BTN_SH_H;

// all dpads have clockwise rotation of verticies (starting from point
// button DPAD UP
const Sint16 SS_PS3_UDP_VX[]    = {
    PS3_BTN_UDP_X0,
    PS3_BTN_UDP_X1,
    PS3_BTN_UDP_X2,
    PS3_BTN_UDP_X3,
    PS3_BTN_UDP_X4
};
const Sint16 SS_PS3_UDP_VY[]    = {
    PS3_BTN_UDP_Y0,
    PS3_BTN_UDP_Y1,
    PS3_BTN_UDP_Y2,
    PS3_BTN_UDP_Y3,
    PS3_BTN_UDP_Y4
};
//const unsigned int SS_PS3_UDP_X = 181;
//const unsigned int SS_PS3_UDP_Y = 484;
//const int SS_PS3_UDP_VX_OFF[]   = {0, -18, -18,  18,  18};
//const int SS_PS3_UDP_VY_OFF[]   = {0, -17, -42, -42, -17};

// button DPAD DOWN
const Sint16 SS_PS3_DDP_VX[]    = {
    PS3_BTN_DDP_X0,
    PS3_BTN_DDP_X1,
    PS3_BTN_DDP_X2,
    PS3_BTN_DDP_X3,
    PS3_BTN_DDP_X4
};
const Sint16 SS_PS3_DDP_VY[]    = {
    PS3_BTN_DDP_Y0,
    PS3_BTN_DDP_Y1,
    PS3_BTN_DDP_Y2,
    PS3_BTN_DDP_Y3,
    PS3_BTN_DDP_Y4
};
//const unsigned int SS_PS3_DDP_X = 181;
//const unsigned int SS_PS3_DDP_Y = 510;
//const int SS_PS3_DDP_VX_OFF[]   = {0,  18,  18, -18, -18};
//const int SS_PS3_DDP_VY_OFF[]   = {0,  17,  42,  42,  17}; 

// button DPAD LFET
const Sint16 SS_PS3_LDP_VX[]    = {
    PS3_BTN_LDP_X0,
    PS3_BTN_LDP_X1,
    PS3_BTN_LDP_X2,
    PS3_BTN_LDP_X3,
    PS3_BTN_LDP_X4
};
const Sint16 SS_PS3_LDP_VY[]    = {
    PS3_BTN_LDP_Y0,
    PS3_BTN_LDP_Y1,
    PS3_BTN_LDP_Y2,
    PS3_BTN_LDP_Y3,
    PS3_BTN_LDP_Y4
};
//const unsigned int SS_PS3_LDP_X = 168;
//const unsigned int SS_PS3_LDP_Y = 497;
//const int SS_PS3_LDP_VX_OFF[]   = {0, -17, -42, -42, -17};
//const int SS_PS3_LDP_VY_OFF[]   = {0,  18,  18, -18, -18}; 

// button DPAD RIGHT
const Sint16 SS_PS3_RDP_VX[]    = {
    PS3_BTN_RDP_X0,
    PS3_BTN_RDP_X1,
    PS3_BTN_RDP_X2,
    PS3_BTN_RDP_X3,
    PS3_BTN_RDP_X4
};
const Sint16 SS_PS3_RDP_VY[]    = {
    PS3_BTN_RDP_Y0,
    PS3_BTN_RDP_Y1,
    PS3_BTN_RDP_Y2,
    PS3_BTN_RDP_Y3,
    PS3_BTN_RDP_Y4
};
//const unsigned int SS_PS3_RDP_X = 194;
//const unsigned int SS_PS3_RDP_Y = 497;
//const int SS_PS3_RDP_VX_OFF[]   = {0,  17,  42,  42,  17};
//const int SS_PS3_RDP_VY_OFF[]   = {0, -18, -18,  18,  18};

// trigger LEFT
const unsigned int SS_PS3_LTR_B_X   = PS3_TRG_L_B_X;
const unsigned int SS_PS3_LTR_B_Y   = PS3_TRG_B_Y;
const unsigned int SS_PS3_LTR_B_W   = PS3_TRG_W;
const unsigned int SS_PS3_LTR_B_H   = PS3_TRG_H;
const unsigned int SS_PS3_LTR_T_X   = PS3_TRG_L_T_X;
const unsigned int SS_PS3_LTR_T_Y   = PS3_TRG_T_Y;
const unsigned int SS_PS3_LTR_T_RX  = PS3_TRG_RX;
const unsigned int SS_PS3_LTR_T_RY  = PS3_TRG_RY;

// trigger RIGHT
const unsigned int SS_PS3_RTR_B_X   = PS3_TRG_R_B_X;
const unsigned int SS_PS3_RTR_B_Y   = PS3_TRG_B_Y;
const unsigned int SS_PS3_RTR_B_W   = PS3_TRG_W;
const unsigned int SS_PS3_RTR_B_H   = PS3_TRG_H;
const unsigned int SS_PS3_RTR_T_X   = PS3_TRG_R_T_X;
const unsigned int SS_PS3_RTR_T_Y   = PS3_TRG_T_Y;
const unsigned int SS_PS3_RTR_T_RX  = PS3_TRG_RX;
const unsigned int SS_PS3_RTR_T_RY  = PS3_TRG_RY;

// joystick lfet
// slices start from east and move clockwise
// each slice contains 3 coordinates, start from center of joystick and move
// clockwise
const Sint16 SS_PS3_LJY_SLS_VX[SS_PS3_JOY_SIZE][SS_PS3_JOY_SLICE_SIZE] =
{ 
    { // slice 0 
        PS3_JOY_L_X,
        PS3_LJOY_SL0_X,
        PS3_LJOY_SL1_X,
    },
    { // slice 1
        PS3_JOY_L_X,
        PS3_LJOY_SL1_X,
        PS3_LJOY_SL2_X,
    },
    { // slice 2
        PS3_JOY_L_X,
        PS3_LJOY_SL2_X,
        PS3_LJOY_SL3_X,
    },
    { // slice 3
        PS3_JOY_L_X,
        PS3_LJOY_SL3_X,
        PS3_LJOY_SL4_X,
    },
    { // slice 4
        PS3_JOY_L_X,
        PS3_LJOY_SL4_X,
        PS3_LJOY_SL5_X,
    },
    { // slice 5
        PS3_JOY_L_X,
        PS3_LJOY_SL5_X,
        PS3_LJOY_SL6_X,
    },
    { // slice 6
        PS3_JOY_L_X,
        PS3_LJOY_SL6_X,
        PS3_LJOY_SL7_X,
    },
    { // slice 7
        PS3_JOY_L_X,
        PS3_LJOY_SL7_X,
        PS3_LJOY_SL0_X,
    }
}; // x coordinates
const Sint16 SS_PS3_LJY_SLS_VY[SS_PS3_JOY_SIZE][SS_PS3_JOY_SLICE_SIZE] =  
{
    { // slice 0 
        PS3_JOY_Y,
        PS3_LJOY_SL0_Y,
        PS3_LJOY_SL1_Y,
    },
    { // slice 1
        PS3_JOY_Y,
        PS3_LJOY_SL1_Y,
        PS3_LJOY_SL2_Y,
    },
    { // slice 2
        PS3_JOY_Y,
        PS3_LJOY_SL2_Y,
        PS3_LJOY_SL3_Y,
    },
    { // slice 3
        PS3_JOY_Y,
        PS3_LJOY_SL3_Y,
        PS3_LJOY_SL4_Y,
    },
    { // slice 4
        PS3_JOY_Y,
        PS3_LJOY_SL4_Y,
        PS3_LJOY_SL5_Y,
    },
    { // slice 5
        PS3_JOY_Y,
        PS3_LJOY_SL5_Y,
        PS3_LJOY_SL6_Y,
    },
    { // slice 6
        PS3_JOY_Y,
        PS3_LJOY_SL6_Y,
        PS3_LJOY_SL7_Y,
    },
    { // slice 7
        PS3_JOY_Y,
        PS3_LJOY_SL7_Y,
        PS3_LJOY_SL0_Y,
    }
}; // y coordinates

// joystick right
// slices start from east and move clockwise
// each slice contains 3 coordinates, start from center of joystick and move
// clockwise
const Sint16 SS_PS3_RJY_SLS_VX[SS_PS3_JOY_SIZE][SS_PS3_JOY_SLICE_SIZE] =  
{ 
    { // slice 0 
        PS3_JOY_R_X,
        PS3_RJOY_SL0_X,
        PS3_RJOY_SL1_X,
    },
    { // slice 1
        PS3_JOY_R_X,
        PS3_RJOY_SL1_X,
        PS3_RJOY_SL2_X,
    },
    { // slice 2
        PS3_JOY_R_X,
        PS3_RJOY_SL2_X,
        PS3_RJOY_SL3_X,
    },
    { // slice 3
        PS3_JOY_R_X,
        PS3_RJOY_SL3_X,
        PS3_RJOY_SL4_X,
    },
    { // slice 4
        PS3_JOY_R_X,
        PS3_RJOY_SL4_X,
        PS3_RJOY_SL5_X,
    },
    { // slice 5
        PS3_JOY_R_X,
        PS3_RJOY_SL5_X,
        PS3_RJOY_SL6_X,
    },
    { // slice 6
        PS3_JOY_R_X,
        PS3_RJOY_SL6_X,
        PS3_RJOY_SL7_X,
    },
    { // slice 7
        PS3_JOY_R_X,
        PS3_RJOY_SL7_X,
        PS3_RJOY_SL0_X,
    }
}; // x coordinates
const Sint16 SS_PS3_RJY_SLS_VY[SS_PS3_JOY_SIZE][SS_PS3_JOY_SLICE_SIZE] =  
{
    { // slice 0 
        PS3_JOY_Y,
        PS3_RJOY_SL0_Y,
        PS3_RJOY_SL1_Y,
    },
    { // slice 1
        PS3_JOY_Y,
        PS3_RJOY_SL1_Y,
        PS3_RJOY_SL2_Y,
    },
    { // slice 2
        PS3_JOY_Y,
        PS3_RJOY_SL2_Y,
        PS3_RJOY_SL3_Y,
    },
    { // slice 3
        PS3_JOY_Y,
        PS3_RJOY_SL3_Y,
        PS3_RJOY_SL4_Y,
    },
    { // slice 4
        PS3_JOY_Y,
        PS3_RJOY_SL4_Y,
        PS3_RJOY_SL5_Y,
    },
    { // slice 5
        PS3_JOY_Y,
        PS3_RJOY_SL5_Y,
        PS3_RJOY_SL6_Y,
    },
    { // slice 6
        PS3_JOY_Y,
        PS3_RJOY_SL6_Y,
        PS3_RJOY_SL7_Y,
    },
    { // slice 7
        PS3_JOY_Y,
        PS3_RJOY_SL7_Y,
        PS3_RJOY_SL0_Y,
    }
}; // y coordinates
