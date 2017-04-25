/**
 * Implementation of PS3 canvas drawing functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include "SDL2/SDL.h"

#include "ss_canvas.h"
#include "ss_constants.h"
#include "ss_shape.h"

#include "ss_ps3_constants.h"

#include "ss_ps3_canvas.h"


//  FUNCTIONS   ===============================================================

int ss_ps3_cvs_drawdpad(
        SDL_Renderer *renderer,
        const ss_ps3_dpad *dpad,
        const ss_canvas_color *color,
        bool add_aa
){

    return ss_canvas_drawpoly(renderer, dpad->vx, dpad->vy, SS_PS3_DPAD_SIZE,
            color, add_aa);
} // ss_ps3_cvs_drawdpad

int ss_ps3_cvs_drawjoy(
        SDL_Renderer *renderer,
        const ss_ps3_joystick *joystick,
        const ss_canvas_color *color,
        bool add_aa,
        bool alt_color,
        const ss_canvas_color *extra,
        unsigned int alt_clr_ct
){

    if ( (alt_color && (extra == NULL || alt_clr_ct < 2))
        || (!alt_color && color == NULL)){
        return SS_RETURN_FAILURE;
    }

    for (int index = 0; index < SS_PS3_JOY_SIZE; index++){
        if (alt_color){
            if (ss_canvas_drawslice(renderer, &(joystick->slices[index]), 
                    &(extra[index % alt_clr_ct]), add_aa) == SS_RETURN_FAILURE){
                return SS_RETURN_FAILURE;
            }
        }else{
            if (ss_canvas_drawslice(renderer, &(joystick->slices[index]),
                    color, add_aa) == SS_RETURN_FAILURE){
                return SS_RETURN_FAILURE;
            }
        }
    }

    return SS_RETURN_SUCCESS;
} // ss_ps3_cvs_drawjoy

int ss_ps3_cvs_drawtrig(
        SDL_Renderer *renderer,
        const ss_ps3_trigger *trigger,
        const ss_canvas_color *color,
        bool add_aa
){

    if (ss_canvas_drawellip(renderer, &(trigger->tip), color, 
                add_aa) == SS_RETURN_FAILURE
            || ss_canvas_drawrect(renderer, &(trigger->body), 
                color) == SS_RETURN_FAILURE){
        return SS_RETURN_FAILURE;
    }

    return SS_RETURN_SUCCESS;
} // ss_ps3_cvs_drawtrig
