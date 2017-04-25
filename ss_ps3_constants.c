/**
 * Implementation of PS3 constants file
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include "ss_ps3_constants.h"

//  PS3 CANVAS  ---------------------------------------------------------------
// circle button radius
const unsigned int SS_PS3_CB_R  = 25;

// button X (A)
const unsigned int SS_PS3_A_X   = 632;
const unsigned int SS_PS3_A_Y   = 556;

// button Circle (B)
const unsigned int SS_PS3_B_X   = 696;
const unsigned int SS_PS3_B_Y   = 498;

// button Square (X)
const unsigned int SS_PS3_X_X   = 570;
const unsigned int SS_PS3_X_Y   = 498;

// button Triangle (Y)
const unsigned int SS_PS3_Y_X   = 632;
const unsigned int SS_PS3_Y_Y   = 439;

// button Select (Back)
const unsigned int SS_PS3_BCK_X = 324;
const unsigned int SS_PS3_BCK_Y = 489;
const unsigned int SS_PS3_BCK_W = 31;
const unsigned int SS_PS3_BCK_H = 16;

// button Start (Start)
//const Sint16 SS_PS3_STRT_VX[]   = {489, 453, 453};
//const Sint16 SS_PS3_STRT_VY[]   = {433, 443, 423};
const unsigned int SS_PS3_STRT_X    = 496;
const unsigned int SS_PS3_STRT_Y    = 497;
const int SS_PS3_STRT_VX_OFF[]      = {0, -36, -36};
const int SS_PS3_STRT_VY_OFF[]      = {0, +10, -10};

// button Left Shoulder
const unsigned int SS_PS3_LSH_X = 139;
const unsigned int SS_PS3_LSH_Y = 191;
const unsigned int SS_PS3_LSH_W = 89;
const unsigned int SS_PS3_LSH_H = 45;

// button Right Shoulder
const unsigned int SS_PS3_RSH_X = 587;
const unsigned int SS_PS3_RSH_Y = 191;
const unsigned int SS_PS3_RSH_W = 89;
const unsigned int SS_PS3_RSH_H = 45;

// all dpads have clockwise rotation of verticies (starting from point
// button DPAD UP
//const Sint16 SS_PS3_UDP_VX[]    = {175, 156, 156, 194, 194};
//const Sint16 SS_PS3_UDP_VY[]    = {483, 465, 440, 440, 465};
const unsigned int SS_PS3_UDP_X = 181;
const unsigned int SS_PS3_UDP_Y = 484;
const int SS_PS3_UDP_VX_OFF[]   = {0, -18, -18,  18,  18};
const int SS_PS3_UDP_VY_OFF[]   = {0, -17, -42, -42, -17};

// button DPAD DOWN
//const Sint16 SS_PS3_DDP_VX[]    = {175, 194, 194, 156, 156};
//const Sint16 SS_PS3_DDP_VY[]    = {513, 531, 556, 556, 531};
const unsigned int SS_PS3_DDP_X = 181;
const unsigned int SS_PS3_DDP_Y = 510;
const int SS_PS3_DDP_VX_OFF[]   = {0,  18,  18, -18, -18};
const int SS_PS3_DDP_VY_OFF[]   = {0,  17,  42,  42,  17}; 

// button DPAD LFET
//const Sint16 SS_PS3_LDP_VX[]    = {160, 142, 117, 117, 142};
//const Sint16 SS_PS3_LDP_VY[]    = {498, 517, 517, 479, 479};
const unsigned int SS_PS3_LDP_X = 168;
const unsigned int SS_PS3_LDP_Y = 497;
const int SS_PS3_LDP_VX_OFF[]   = {0, -17, -42, -42, -17};
const int SS_PS3_LDP_VY_OFF[]   = {0,  18,  18, -18, -18}; 

// button DPAD RIGHT
//const Sint16 SS_PS3_RDP_VX[]    = {190, 208, 233, 233, 208};
//const Sint16 SS_PS3_RDP_VY[]    = {498, 479, 479, 517, 517};
const unsigned int SS_PS3_RDP_X = 194;
const unsigned int SS_PS3_RDP_Y = 497;
const int SS_PS3_RDP_VX_OFF[]   = {0,  17,  42,  42,  17};
const int SS_PS3_RDP_VY_OFF[]   = {0, -18, -18,  18,  18};

// trigger LEFT
// TODO make it offests
const unsigned int SS_PS3_LTR_B_X   = 128;
const unsigned int SS_PS3_LTR_B_Y   = 31;
const unsigned int SS_PS3_LTR_B_W   = 94;
const unsigned int SS_PS3_LTR_B_H   = 60;
const unsigned int SS_PS3_LTR_T_X   = 175;
const unsigned int SS_PS3_LTR_T_Y   = 31;
const unsigned int SS_PS3_LTR_T_RX  = 47;
const unsigned int SS_PS3_LTR_T_RY  = 16;

// trigger RIGHT
const unsigned int SS_PS3_RTR_B_X   = 580;
const unsigned int SS_PS3_RTR_B_Y   = 31;
const unsigned int SS_PS3_RTR_B_W   = 94;
const unsigned int SS_PS3_RTR_B_H   = 60;
const unsigned int SS_PS3_RTR_T_X   = 627;
const unsigned int SS_PS3_RTR_T_Y   = 31;
const unsigned int SS_PS3_RTR_T_RX  = 47;
const unsigned int SS_PS3_RTR_T_RY  = 16;
