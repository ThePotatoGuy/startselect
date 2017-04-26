/**
 * Data that represents shapes
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains data types that contain info about the different shapes
 */

#ifndef SS_SHAPE_H
#define SS_SHAPE_H

#include "SDL2/SDL.h"

#include "ss_ps3_constants.h"

//  CONSTANTS   ===============================================================

// size of a triangle in vertexes
#define SS_TRI_SIZE 3

//  TYPES   ===================================================================

// components of a triangle
// (all in pixels)
typedef struct{ 
    Sint16 vx[SS_TRI_SIZE];    // x coordinates
    Sint16 vy[SS_TRI_SIZE];    // y coordinates
} ss_triangle;

// components of an ellipse
// (all in pixels)
typedef struct{
    unsigned int x;     // x coordinates of center
    unsigned int y;     // y coordinates of center
    unsigned int rx;    // horizontal radius
    unsigned int ry;    // vertical radius
} ss_ellipse;

// components of a circle
// (all in pixels)
typedef struct{
    unsigned int x;     // x coordinates of center
    unsigned int y;     // y coordinates of center
    unsigned int r;     // radius
} ss_circle;

// components of a pizza slice
typedef struct{
    ss_ellipse tip;     // the round edge of a slice
    ss_triangle body;   // the body of the a slice
} ss_slice;

// components of a ps3 dpad
// > bascially x and y coordinates of its vertices
// (all in pixels)
typedef struct{
    Sint16 vx[SS_PS3_DPAD_SIZE];    // x coordinates
    Sint16 vy[SS_PS3_DPAD_SIZE];    // y coordinates
} ss_ps3_dpad;

// components of a ps3 trigger
// > bascially the square part and the round tip
typedef struct{
    ss_ellipse tip;     // the round edge of the trigger
    SDL_Rect body;      // the square section of the trigger
} ss_ps3_trigger;

// components of a ps3 joystick
// > bascially all the slices of the joystick
typedef struct{
    // we no longer use ellipse part of slices
//    ss_slice slices[SS_PS3_JOY_SIZE];   // slices of the joystick

    ss_triangle slices[SS_PS3_JOY_SIZE];    // slice sof the joystick
} ss_ps3_joystick;

#endif
