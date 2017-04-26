/**
 * Implementation of PS3 canvas drawing functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include <stdlib.h>
#include <string.h>

#include "SDL2/SDL.h"

#include "ss_canvas.h"
#include "ss_constants.h"
#include "ss_gc_constants.h"
#include "ss_shape.h"

#include "ss_ps3_constants.h"

#include "ss_ps3_canvas.h"

//  CONSTANTS   ===============================================================

// 14 buttons, 2 triggers and 2 joysticks
#define PS3_SHAPE_SIZE 18

//  VARIABLES   ===============================================================

// shape data 
static ss_canvas_shape ps3_shapes[PS3_SHAPE_SIZE];

//  STATIC FUNCTIONS    =======================================================

/*
 * Setups the ps3 circle-shaped button shapes
 */
static void setup_ps3_circles();

/*
 * Setups the ps3 dpad shapes
 */
static void setup_ps3_dpad();

/*
 * Setups the ps3 joystick shapes
 */
static void setup_ps3_joysticks();

/*
 * Setups the ps3 select button shape
 */
static void setup_ps3_select();

/*
 * Setups the ps3 shapes array
 */
static void setup_ps3_shapes();

/*
 * Setups the ps3 shoulder button shapes
 */
static void setup_ps3_shoulders();

/*
 * Setups the ps3 start button shape
 */
static void setup_ps3_start();

/*
 * Setups the ps3 thumb stick button shapes
 */
static void setup_ps3_thumbs();

/*
 * Setups the ps3 trigger shapes
 */
static void setup_ps3_triggers();

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

/*
int ss_ps3_cvs_drawjoy(
        SDL_Renderer *renderer,
        const ss_ps3_joystick *joystick,
        const ss_canvas_color *color,
        bool add_aa,
){

    for (int index = 0; index < SS_PS3_JOY_SIZE; index++){
        if (ss_canvas
        if (ss_canvas_drawslice(renderer, &(joystick->slices[index]),
                    color, add_aa) == SS_RETURN_FAILURE){
                return SS_RETURN_FAILURE;
        }
    }

    return SS_RETURN_SUCCESS;
} // ss_ps3_cvs_drawjoy
*/

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

int ss_ps3_cvs_init(){
    
    setup_ps3_shapes();
    return SS_RETURN_SUCCESS;
} // ss_ps3_cvs_init

//  STATIC IMPLEMENTATION   ===================================================

static void setup_ps3_circles(){

    // button A
    ps3_shapes[SS_BUTTON_A].circle.x = SS_PS3_A_X;
    ps3_shapes[SS_BUTTON_A].circle.y = SS_PS3_A_Y;
    ps3_shapes[SS_BUTTON_A].circle.r = SS_PS3_A_R;

    // button B
    ps3_shapes[SS_BUTTON_B].circle.x = SS_PS3_B_X;
    ps3_shapes[SS_BUTTON_B].circle.y = SS_PS3_B_Y;
    ps3_shapes[SS_BUTTON_B].circle.r = SS_PS3_B_R;

    // button X
    ps3_shapes[SS_BUTTON_X].circle.x = SS_PS3_X_X;
    ps3_shapes[SS_BUTTON_X].circle.y = SS_PS3_X_Y;
    ps3_shapes[SS_BUTTON_X].circle.r = SS_PS3_X_R;

    // button Y
    ps3_shapes[SS_BUTTON_Y].circle.x = SS_PS3_Y_X;
    ps3_shapes[SS_BUTTON_Y].circle.y = SS_PS3_Y_Y;
    ps3_shapes[SS_BUTTON_Y].circle.r = SS_PS3_Y_R;

} // setup_ps3_circles

static void setup_ps3_dpad(){

    // up
    memcpy(ps3_shapes[SS_BUTTON_DPAD_UP].ps3_dpad.vx, SS_PS3_UDP_VX,
            SS_PS3_DPAD_SIZE * sizeof(Sint16));
    memcpy(ps3_shapes[SS_BUTTON_DPAD_UP].ps3_dpad.vy, SS_PS3_UDP_VY,
            SS_PS3_DPAD_SIZE * sizeof(Sint16));

    // down
    memcpy(ps3_shapes[SS_BUTTON_DPAD_DOWN].ps3_dpad.vx, SS_PS3_DDP_VX,
            SS_PS3_DPAD_SIZE * sizeof(Sint16));
    memcpy(ps3_shapes[SS_BUTTON_DPAD_DOWN].ps3_dpad.vy, SS_PS3_DDP_VY,
            SS_PS3_DPAD_SIZE * sizeof(Sint16));

    // left
    memcpy(ps3_shapes[SS_BUTTON_DPAD_LEFT].ps3_dpad.vx, SS_PS3_LDP_VX,
            SS_PS3_DPAD_SIZE * sizeof(Sint16));
    memcpy(ps3_shapes[SS_BUTTON_DPAD_LEFT].ps3_dpad.vy, SS_PS3_LDP_VY,
            SS_PS3_DPAD_SIZE * sizeof(Sint16));

    // right
    memcpy(ps3_shapes[SS_BUTTON_DPAD_RIGHT].ps3_dpad.vx, SS_PS3_RDP_VX,
            SS_PS3_DPAD_SIZE * sizeof(Sint16));
    memcpy(ps3_shapes[SS_BUTTON_DPAD_RIGHT].ps3_dpad.vy, SS_PS3_RDP_VY,
            SS_PS3_DPAD_SIZE * sizeof(Sint16));

} // setup_ps3_dpad

static void setup_ps3_joysticks(){

    for (int index = 0; index < SS_PS3_JOY_SIZE; index++){

        // left
        memcpy(ps3_shapes[SS_JOYSTICK_LEFT].ps3_joystick.slices[index].vx,
                SS_PS3_LJY_SLS_VX[index], 
                SS_PS3_JOY_SLICE_SIZE * sizeof(Sint16));
        memcpy(ps3_shapes[SS_JOYSTICK_LEFT].ps3_joystick.slices[index].vy,
                SS_PS3_LJY_SLS_VY[index],
                SS_PS3_JOY_SLICE_SIZE * sizeof(Sint16));

        // right
        memcpy(ps3_shapes[SS_JOYSTICK_RIGHT].ps3_joystick.slices[index].vx,
                SS_PS3_RJY_SLS_VX[index], 
                SS_PS3_JOY_SLICE_SIZE * sizeof(Sint16));
        memcpy(ps3_shapes[SS_JOYSTICK_RIGHT].ps3_joystick.slices[index].vy,
                SS_PS3_RJY_SLS_VY[index],
                SS_PS3_JOY_SLICE_SIZE * sizeof(Sint16));
    }

} // setup_ps3_joysticks

static void setup_ps3_select(){

    ps3_shapes[SS_BUTTON_BACK].rect.x = SS_PS3_BCK_X;
    ps3_shapes[SS_BUTTON_BACK].rect.y = SS_PS3_BCK_Y;
    ps3_shapes[SS_BUTTON_BACK].rect.w = SS_PS3_BCK_W;
    ps3_shapes[SS_BUTTON_BACK].rect.h = SS_PS3_BCK_H;

} // setup_ps3_select

static void setup_ps3_shapes(){

    setup_ps3_circles();
    setup_ps3_select();
    setup_ps3_start();
    setup_ps3_thumbs();
    setup_ps3_shoulders();
    setup_ps3_dpad();
    setup_ps3_triggers();
    setup_ps3_joysticks();

} // setup_ps3_shapes

static void setup_ps3_shoulders(){

    // left
    ps3_shapes[SS_BUTTON_LEFT_SHOULDER].rect.x = SS_PS3_LSH_X;
    ps3_shapes[SS_BUTTON_LEFT_SHOULDER].rect.y = SS_PS3_LSH_Y;
    ps3_shapes[SS_BUTTON_LEFT_SHOULDER].rect.w = SS_PS3_LSH_W;
    ps3_shapes[SS_BUTTON_LEFT_SHOULDER].rect.h = SS_PS3_LSH_H;

    // right
    ps3_shapes[SS_BUTTON_RIGHT_SHOULDER].rect.x = SS_PS3_RSH_X;
    ps3_shapes[SS_BUTTON_RIGHT_SHOULDER].rect.y = SS_PS3_RSH_Y;
    ps3_shapes[SS_BUTTON_RIGHT_SHOULDER].rect.w = SS_PS3_RSH_W;
    ps3_shapes[SS_BUTTON_RIGHT_SHOULDER].rect.h = SS_PS3_RSH_H;

} // setup_ps3_shoulders

static void setup_ps3_start(){

    memcpy(ps3_shapes[SS_BUTTON_START].triangle.vx, SS_PS3_STRT_VX, 
            SS_PS3_STRT_SIZE * sizeof(Sint16));
    memcpy(ps3_shapes[SS_BUTTON_START].triangle.vy, SS_PS3_STRT_VY,
            SS_PS3_STRT_SIZE * sizeof(Sint16));

} // setup_ps3_start

static void setup_ps3_thumbs(){

    // left
    ps3_shapes[SS_BUTTON_LEFT_THUMB].circle.x = SS_PS3_LTHM_X;
    ps3_shapes[SS_BUTTON_LEFT_THUMB].circle.y = SS_PS3_LTHM_Y;
    ps3_shapes[SS_BUTTON_LEFT_THUMB].circle.r = SS_PS3_LTHM_R;

    // right
    ps3_shapes[SS_BUTTON_RIGHT_THUMB].circle.x = SS_PS3_RTHM_X;
    ps3_shapes[SS_BUTTON_RIGHT_THUMB].circle.y = SS_PS3_RTHM_Y;
    ps3_shapes[SS_BUTTON_RIGHT_THUMB].circle.r = SS_PS3_RTHM_R;

} // setup_ps3_thumbs

static void setup_ps3_triggers(){

    // left
    ps3_shapes[SS_TRIGGER_LEFT].ps3_trigger.body.x  = SS_PS3_LTR_B_X;
    ps3_shapes[SS_TRIGGER_LEFT].ps3_trigger.body.y  = SS_PS3_LTR_B_Y;
    ps3_shapes[SS_TRIGGER_LEFT].ps3_trigger.body.w  = SS_PS3_LTR_B_W;
    ps3_shapes[SS_TRIGGER_LEFT].ps3_trigger.body.h  = SS_PS3_LTR_B_H;
    ps3_shapes[SS_TRIGGER_LEFT].ps3_trigger.tip.x   = SS_PS3_LTR_T_X;
    ps3_shapes[SS_TRIGGER_LEFT].ps3_trigger.tip.x   = SS_PS3_LTR_T_Y;
    ps3_shapes[SS_TRIGGER_LEFT].ps3_trigger.tip.rx  = SS_PS3_LTR_T_RX;
    ps3_shapes[SS_TRIGGER_LEFT].ps3_trigger.tip.ry  = SS_PS3_LTR_T_RY;

    // right
    ps3_shapes[SS_TRIGGER_RIGHT].ps3_trigger.body.x  = SS_PS3_RTR_B_X;
    ps3_shapes[SS_TRIGGER_RIGHT].ps3_trigger.body.y  = SS_PS3_RTR_B_Y;
    ps3_shapes[SS_TRIGGER_RIGHT].ps3_trigger.body.w  = SS_PS3_RTR_B_W;
    ps3_shapes[SS_TRIGGER_RIGHT].ps3_trigger.body.h  = SS_PS3_RTR_B_H;
    ps3_shapes[SS_TRIGGER_RIGHT].ps3_trigger.tip.x   = SS_PS3_RTR_T_X;
    ps3_shapes[SS_TRIGGER_RIGHT].ps3_trigger.tip.x   = SS_PS3_RTR_T_Y;
    ps3_shapes[SS_TRIGGER_RIGHT].ps3_trigger.tip.rx  = SS_PS3_RTR_T_RX;
    ps3_shapes[SS_TRIGGER_RIGHT].ps3_trigger.tip.ry  = SS_PS3_RTR_T_RY;

} // setup_ps3_triggerS
