/**
 * Drawing canvas (for rendering)
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains functions related to opening a drawing canvas and drawing on it
 */

#ifndef SS_CANVAS_H
#define SS_CANVAS_H

#include <stdbool.h>

#include "SDL2/SDL.h"

#include "ss_shape.h"

//  CONSTANTS   ===============================================================

// the number of poly vertexes in a dpad
#define SS_DPAD_SIZE 5

//  TYPES   ===================================================================

// parts of a color
// 255 is max, 0 is min
typedef struct{
    unsigned char r;    // red
    unsigned char g;    // green
    unsigned char b;    // blue
    unsigned char a;    // alpha
} ss_canvas_color;

// shape data union
typedef union{
    ss_triangle     triangle;
    ss_ellipse      ellipse;
    ss_circle       circle;
    ss_slice        slice;
    ss_ps3_dpad     ps3_dpad;
    ss_ps3_trigger  ps3_trigger;
    ss_ps3_joystick ps3_joystick;
} ss_canvas_shape;

//  FUNCTIONS   ===============================================================

/*
 * Draws a circle with its center at the specificed coordinates on the given 
 * renderer
 *
 * IN:
 *  @param renderer - the SDL_Renderer to draw on
 *  @param circle - info about the circle to draw
 *  @param color - color of the circle to draw
 *  @param add_aa - true draws the circle with AA, false does not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_canvas_drawcircle(
        SDL_Renderer *renderer,
        const ss_circle *circle,
        const ss_canvas_color *color,
        bool add_aa
);

/*
 * Draws an ellipse using the given ellipse onto the given renderer
 *
 * IN:
 *  @param renderer - the SDL_Renderer to draw on
 *  @param ellipes - dimensions of the ellipse to draw
 *  @param color - color of the ellipse to draw
 *  @param add_aa - true draws the ellipse with AA, false does not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_canvas_drawellip(
        SDL_Renderer *renderer,
        const ss_ellipse *ellipse,
        const ss_canvas_color *color,
        bool add_aa
);

/*
 * Draws a polygon using the given coordinates onto the given renderer
 *
 * IN:
 *  @param renderer - the SDL_Renderer to draw on
 *  @param vx - array of the x coordinate of the vertices of this polygon
 *  @param vy - array of the y coordinates of the vertices of this polygon
 *  @param vtx_ct - number of vertices of this polygon
 *  @param color - color of the polygon to draw
 *  @param add_aa - true draws the poly with AA, false does not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_canvas_drawpoly(
        SDL_Renderer *renderer,
        const Sint16 *vx,
        const Sint16 *vy,
        unsigned int vtx_ct,
        const ss_canvas_color *color,
        bool add_aa
);

/*
 * Draws a rectangle using the given SDL_Rect onto the given renderer
 *
 * IN:
 *  @param renderer - the SDL_Renderer to draw on
 *  @param rect - dimensions of the rectangle to draw
 *  @param color - color of the rectangle to draw
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_canvas_drawrect(
        SDL_Renderer *renderer, 
        const SDL_Rect *rect, 
        const ss_canvas_color *color
);

/*
 * Draws a slice using the given ss_slice onto the given renderer
 *
 * IN:
 *  @param renderer - the SDL_Renderer to draw on
 *  @param slice - dimensions of the slice to draw
 *  @param color - color of the slice to draw
 *  @oaran add_aa - true draws the slice with AA, false does not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on suyccess, SS_RETURN_FAILURE otherwise
 */
int ss_canvas_drawslice(
        SDL_Renderer *renderer,
        const ss_slice *slice,
        const ss_canvas_color *color,
        bool add_aa
);

/*
 * Initializes the canvas
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_FAILURE if not
 */
int ss_canvas_init();

// testing method 
int ss_open_canvas();

#endif
