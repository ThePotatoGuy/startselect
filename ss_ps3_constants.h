/**
 * Constants for PS3 controllers
 * @author Andre Allan Ponce
 * @email andreponce@lavabit.com
 *
 * Contains all the constants used for PS3 controllers
 *
 * TODO: move all ps3 constants that arent in here yet to the file
 */

#ifndef SS_PS3_CONSTANTS_H
#define SS_PS3_CONSTANTS_H

#include "SDL2/SDL.h"

//  PS3 DEFINES ---------------------------------------------------------------

// size of ps3 dpad in vertexes
#define SS_PS3_DPAD_SIZE 5

// number of slices in a ps3 joystick
#define SS_PS3_JOY_SIZE 8

// number of vertices in a ps3 joystick slice
#define SS_PS3_JOY_SLICE_SIZE 3

// number of verticies in start button
#define SS_PS3_STRT_SIZE 3

//  PS3 INDEXES ---------------------------------------------------------------

//  PS3 CANVAS  ---------------------------------------------------------------

// parens denote xinput equivalent
// all constants are named using their XInput equiavlent
// button X (A)
extern const unsigned int SS_PS3_A_X;   // x coordinate
extern const unsigned int SS_PS3_A_Y;   // y coordinate
extern const unsigned int SS_PS3_A_R;   // radius

// button Circle (B)
extern const unsigned int SS_PS3_B_X;   // x coordinate
extern const unsigned int SS_PS3_B_Y;   // y coordinate
extern const unsigned int SS_PS3_B_R;   // radius

// button Square (X)
extern const unsigned int SS_PS3_X_X;   // x coordinate
extern const unsigned int SS_PS3_X_Y;   // y coordinate
extern const unsigned int SS_PS3_X_R;   // radius

// button Triangle (Y)
extern const unsigned int SS_PS3_Y_X;   // x coordinate
extern const unsigned int SS_PS3_Y_Y;   // coordinate
extern const unsigned int SS_PS3_Y_R;   // radius

// button Select (Back)
extern const unsigned int SS_PS3_BCK_X; // x coordinate
extern const unsigned int SS_PS3_BCK_Y; // y coordinate
extern const unsigned int SS_PS3_BCK_W; // width
extern const unsigned int SS_PS3_BCK_H; // height

// button Start (Start)
extern const Sint16 SS_PS3_STRT_VX[SS_PS3_STRT_SIZE];   // x coords
extern const Sint16 SS_PS3_STRT_VY[SS_PS3_STRT_SIZE];   // y coords
//extern const unsigned int SS_PS3_STRT_X;    // x coordinate ofp oint
//extern const unsigned int SS_PS3_STRT_Y;    // y coordinate of point
//extern const int SS_PS3_STRT_VX_OFF[];      // x coordinate offsetes
//extern const int SS_PS3_STRT_VY_OFF[];      // y coordinate offsetes

// button Left Thumb
extern const unsigned int SS_PS3_LTHM_X;    // x coordinate
extern const unsigned int SS_PS3_LTHM_Y;    // y coordinate
extern const unsigned int SS_PS3_LTHM_R;    // radius

// button Right Thumb
extern const unsigned int SS_PS3_RTHM_X;    // x coordinate
extern const unsigned int SS_PS3_RTHM_Y;    // y coordinate
extern const unsigned int SS_PS3_RTHM_R;    // radiu

// button Left Shoulder
extern const unsigned int SS_PS3_LSH_X; // x coordinate
extern const unsigned int SS_PS3_LSH_Y; // y coordinate
extern const unsigned int SS_PS3_LSH_W; // width
extern const unsigned int SS_PS3_LSH_H; // height

// button Right Shoulder
extern const unsigned int SS_PS3_RSH_X; // x coordinate
extern const unsigned int SS_PS3_RSH_Y; // y coordinate
extern const unsigned int SS_PS3_RSH_W; // width
extern const unsigned int SS_PS3_RSH_H; // height

// button DPAD UP
extern const Sint16 SS_PS3_UDP_VX[SS_PS3_DPAD_SIZE];    // x coords
extern const Sint16 SS_PS3_UDP_VY[SS_PS3_DPAD_SIZE];    // y coords
//extern const unsigned int SS_PS3_UDP_X; // x coordinate of point
//extern const unsigned int SS_PS3_UDP_Y; // y coordinate of point
//extern const int SS_PS3_UDP_VX_OFF[];   // x coordinate offsets
//extern const int SS_PS3_UDP_VY_OFF[];   // y coordinate offsets

// button DPAD DOWN
extern const Sint16 SS_PS3_DDP_VX[SS_PS3_DPAD_SIZE];    // x coords
extern const Sint16 SS_PS3_DDP_VY[SS_PS3_DPAD_SIZE];    // y coords
//extern const unsigned int SS_PS3_DDP_X; // x coordinate of point
//extern const unsigned int SS_PS3_DDP_Y; // y coordinate of point
//extern const int SS_PS3_DDP_VX_OFF[];   // x coordinate offsets
//extern const int SS_PS3_DDP_VY_OFF[];   // y coordinate offsets

// button DPAD LEFT
extern const Sint16 SS_PS3_LDP_VX[SS_PS3_DPAD_SIZE];    // x coords
extern const Sint16 SS_PS3_LDP_VY[SS_PS3_DPAD_SIZE];    // y coords
//extern const unsigned int SS_PS3_LDP_X; // x coordinate of point
//extern const unsigned int SS_PS3_LDP_Y; // y coordinate of point
//extern const int SS_PS3_LDP_VX_OFF[];   // x coordinaet offsets
//extern const int SS_PS3_LDP_VY_OFF[];   // y coordinate offsetes

// button DPAD RIGHT
extern const Sint16 SS_PS3_RDP_VX[SS_PS3_DPAD_SIZE];    // x coords
extern const Sint16 SS_PS3_RDP_VY[SS_PS3_DPAD_SIZE];    // y coords
//extern const unsigned int SS_PS3_RDP_X; // x coordinate of point
//extern const unsigned int SS_PS3_RDP_Y; // y coordinate of point
//extern const int SS_PS3_RDP_VX_OFF[];   // x coordinate offsets
//extern const int SS_PS3_RDP_VY_OFF[];   // y coordinate offsetes

// trigger LEFT
// coordinates to the trigger are point where x = left size of trigger and
// y = top of the trigger tip
extern const unsigned int SS_PS3_LTR_B_X;   // x coordinate of body
extern const unsigned int SS_PS3_LTR_B_Y;   // y coordinate of body
extern const unsigned int SS_PS3_LTR_B_W;   // width of body
extern const unsigned int SS_PS3_LTR_B_H;   // height of body
extern const unsigned int SS_PS3_LTR_T_X;   // x coodrinate of tip
extern const unsigned int SS_PS3_LTR_T_Y;   // y coordinate of tip
extern const unsigned int SS_PS3_LTR_T_RX;  // horizontal radius of tip
extern const unsigned int SS_PS3_LTR_T_RY;  // vertical radius of tip

// trigger RIGHT
extern const unsigned int SS_PS3_RTR_B_X;   // x coordinate of body
extern const unsigned int SS_PS3_RTR_B_Y;   // y coordinate of body
extern const unsigned int SS_PS3_RTR_B_W;   // width of body
extern const unsigned int SS_PS3_RTR_B_H;   // height of body
extern const unsigned int SS_PS3_RTR_T_X;   // x coodrinate of tip
extern const unsigned int SS_PS3_RTR_T_Y;   // y coordinate of tip
extern const unsigned int SS_PS3_RTR_T_RX;  // horizontal radius of tip
extern const unsigned int SS_PS3_RTR_T_RY;  // vertical radius of tip

// joystick left (x and y coords)
extern const Sint16 SS_PS3_LJY_SLS_VX[SS_PS3_JOY_SIZE][SS_PS3_JOY_SLICE_SIZE];  
extern const Sint16 SS_PS3_LJY_SLS_VY[SS_PS3_JOY_SIZE][SS_PS3_JOY_SLICE_SIZE];

// joystick right (x and y coords)
extern const Sint16 SS_PS3_RJY_SLS_VX[SS_PS3_JOY_SIZE][SS_PS3_JOY_SLICE_SIZE];
extern const Sint16 SS_PS3_RJY_SLS_VY[SS_PS3_JOY_SIZE][SS_PS3_JOY_SLICE_SIZE];

#endif
