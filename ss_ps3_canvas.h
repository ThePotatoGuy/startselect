/**
 * Canvas draw functions specific to PS3 controller
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains functions specific to drawing elements of a PS3 controller
 */

#ifndef SS_PS3_CANVAS_H
#define SS_PS3_CANVAS_H

#include "SDL2/SDL.h"

#include "ss_canvas.h"
#include "ss_canvas_color.h"
#include "ss_shape.h"

#include "ss_ps3_statcolors.h"

//  CONSTANTS   ===============================================================

// 14 buttons, 2 triggers and 2 joysticks
#define SS_PS3_SHAPE_SIZE 18

//  FUNCTIONS   ===============================================================

/*
 * Draws a PS3 DPAD using the given dpad onto the given renderer
 *
 * IN:
 *  @param renderer - the SDL_Renderer to draw on
 *  @param dpad - the dimensions of the dpad to draw
 *  @param color - the color of the dpad to draw
 *  @param add_aa - true draws the dpad with AA, false does not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_ps3_cvs_drawdpad(
        SDL_Renderer *renderer,
        const ss_ps3_dpad *dpad,
        const ss_canvas_color *color,
        bool add_aa
);

/*
 * Draws a PS3 joystick using the given joystick onto the given renderer
 *
 * IN:
 *  @param renderer - the SDL_Renderer to draw on
 *  @param joystick - the dimensions of teh joystick to draw
 *  @param color - the colors of the joystick slices to draw
 *  @param add_aa - true draws the joystick with AA, false does not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURe otherwise
 */
int ss_ps3_cvs_drawjoy(
        SDL_Renderer *renderer,
        const ss_ps3_joystick *joystick,
        const ss_canvas_color (*color)[SS_PS3_JOY_SIZE],
        bool add_aa
);

/*
 * Draws all the shapes of a PS3 using the given colors array
 *
 * IN:
 *  @param renderer - the SDL_Rendeer to draw on
 *  @param colors - the colors to draw with
 *  @param add_aa - true draws the ps3 shapes with AA, false does not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_ps3_cvs_drawps3(
        SDL_Renderer *renderer,
        const ss_ps3_colors *colors,
        bool add_aa
);

/*
 * Draws a PS3 trigger using the given trigger onto the given renderer
 *
 * IN:
 *  @param renderer - the SDL_Renderer to draw on
 *  @param trigger - the dimensions of the trigger to draw
 *  @param color - teh color of the trigger to draw
 *  @param add_aa - true draws the trigger with AA, false does not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_ps3_cvs_drawtrig(
        SDL_Renderer *renderer,
        const ss_ps3_trigger *trigger,
        const ss_canvas_color *color,
        bool add_aa
);

/*
 * Initializes this PS3 canvas
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_ps3_cvs_init();

#endif
