/**
 * Implementation of Game Controller functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 */

//#include "SDL2/SDL.h"
#include <windows.h>
#include <xinput.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "ss_constants.h"

#include "ss_gamecontroller.h"

//  CONSTANTS   ===============================================================

//static const int BUTTON_COUNT = 14;
#define BUTTON_COUNT 14
#define BUTTON_STR_SIZE 20

// button descriptions
static const char BUTTON_A[]                = "A";
static const char BUTTON_B[]                = "B";
static const char BUTTON_X[]                = "X";
static const char BUTTON_Y[]                = "Y";
static const char BUTTON_BACK[]             = "Back";
static const char BUTTON_GUIDE[]            = "Guide"; // unused
static const char BUTTON_START[]            = "Start";
static const char BUTTON_LEFT_THUMB[]       = "Left thumb";
static const char BUTTON_RIGHT_THUMB[]      = "Right thumb";
static const char BUTTON_LEFT_SHOULDER[]    = "Left shoulder";
static const char BUTTON_RIGHT_SHOULDER[]   = "Right shoulder";
static const char BUTTON_DPAD_UP[]          = "D up";
static const char BUTTON_DPAD_DOWN[]        = "D down";
static const char BUTTON_DPAD_LEFT[]        = "D left";
static const char BUTTON_DPAD_RIGHT[]       = "D right";
static const char BUTTON_MAX[]              = "Max"; // unused
static const char BUTTON_INVALID[]          = "Invalid"; // unusedi


// button indexes (based on hardcoded button array)
const int SS_BUTTON_A               = 0;
const int SS_BUTTON_B               = 1;
const int SS_BUTTON_X               = 2;
const int SS_BUTTON_Y               = 3;
const int SS_BUTTON_BACK            = 4;
const int SS_BUTTON_START           = 5;
const int SS_BUTTON_LEFT_THUMB      = 6;
const int SS_BUTTON_RIGHT_THUMB     = 7;
const int SS_BUTTON_LEFT_SHOULDER   = 8;
const int SS_BUTTON_RIGHT_SHOULDER  = 9;
const int SS_BUTTON_DPAD_UP         = 10;
const int SS_BUTTON_DPAD_DOWN       = 11;
const int SS_BUTTON_DPAD_LEFT       = 12;
const int SS_BUTTON_DPAD_RIGHT      = 13;

// joystick values
const int SS_JOYSTICK_MAX           = 32767;
const int SS_JOYSTICK_MIN           = -32768;

// trigger values
const int SS_TRIGGER_MAX            = 255;
const int SS_TRIGGER_MIN            = 0;

// hardcoded button array
static char BUTTON_STRS[BUTTON_COUNT][BUTTON_STR_SIZE];
/*static const char BUTTON_STRS[BUTTON_COUNT][BUTTON_STR_SIZE] = {
    "A",
    "B",
    "X",
    "Y",
    "Back",
    "Start",
    "Left thumb",
    "Right thumb",
    "Left shoulder",
    "Right shoulder",
    "D up",
    "D down",
    "D left",
    "D right"
};*/

// pressed released description
static const char BUTTON_PRESSED[]          = "Pressed";
static const char BUTTON_RELEASED[]         = "Released";

// the fmt string we use
static const char BUTTON_FMT_STR[] = "Button %s Pressed\n";

//  VARIABLES   ===============================================================

// global that contains the array of button presses
static ss_button_presses buttons = {0, 0};

//  STATIC FUNCTIONS    =======================================================

/*
 * Deletes the pressed buttons struct
 */
static void destroy_pressed_buttons();

/*
 * Initialzes the ss_joystick_data for a generic joystick
 *
 * OUT:
 *  @param joystick - ss_joystick_data intialized for a generic joystick
 */
static void init_joystick(ss_joystick_data *joystick);

/*
 * Initializes the ss_joystick_data for a left joystick
 *
 * OUT
 *  @param joystick - ss_joystick_data initalized for a left joystick
 */
static void init_joystick_left(ss_joystick_data *joystick);

/*
 * Initialies the ss_joystick_data for a right joystick
 *
 * OUT:
 *  @param joystick - ss_joystick_data intialized for a right joystick
 */
static void init_joystick_right(ss_joystick_data *joystick);

/*
 * Initlaizes the button string array
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS upon success, SS_RETURN_ERROR if error occured
 */
//static int init_button_array();

/*
 * Initializes the buttons preesses struct
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS upon success, SS_RETURN_ERROR if error occured
 */
static int init_pressed_buttons();

/*
 * Initializes the ss_trigger_data for a generic trigger
 *
 * OUT:
 *  @param trigger - ss_trigger_data initalized for a generic trigger
 */
static void init_trigger(ss_trigger_data *trigger);

/*
 * Initializes the ss_trigger_data for a left trigger
 *
 * OUT:
 *  @param trigger - ss_Trigger_data intialized for a left trigger
 */
static void init_trigger_left(ss_trigger_data *trigger);

/*
 * Initializes the ss_trigger_data for a right trigger
 *
 * OUT:
 *  @param trigger - ss_trigger_data initalized for a right trigger
 */
static void init_trigger_right(ss_trigger_data *trigger);

/*
 * Processes the joystick input given a struct and the gamepad
 *
 * IN:
 *  @param gamepad - XINPUT_GAMEPAD struct
 *
 * OUT:
 *  @param joystick - ss_joystick_data struct filled out with input data
 */
static void process_joystick_input(
        XINPUT_GAMEPAD *gamepad, 
        ss_joystick_data *joystick
);

/*
 * PRocesses the trigger input given a struct and teh gamepad
 *
 * IN:
 *  @param gamepad - XINPUT_GAMEPAD struct
 * OUT:
 *  @param trigger - ss_trigger_data struct filled out with input data
 */
static void process_trigger_input(
        XINPUT_GAMEPAD *gamepad,
        ss_trigger_data *trigger
);

/*
 * Setups the BUTTON_STRS array
 */
static void setup_button_strs();

//  IMPLEMENTATION  ===========================================================

void ss_destroy_button_presses(ss_button_presses *buttons){
    if (buttons->pressed != NULL){
        free(buttons->pressed);
        buttons->pressed = NULL;
    }
}

void ss_destroy_gamecontroller(){
    destroy_pressed_buttons();
}

void destroy_generic_controller(ss_generic_controller *controller){
    ss_destroy_button_presses(&(controller->buttons));
}

int ss_init_button_presses(ss_button_presses *buttons){
    buttons->pressed = malloc(BUTTON_COUNT*sizeof(int));

    if (buttons->pressed == NULL){
        return SS_RETURN_ERROR;
    }

    for (int index = 0; index < BUTTON_COUNT; index++){
        buttons->pressed[index] = 0;
    }

    return SS_RETURN_SUCCESS;
}

int ss_init_gamecontroller(){
    if (init_pressed_buttons() == SS_RETURN_ERROR){
        return SS_RETURN_ERROR;
    }

    setup_button_strs();

    return SS_RETURN_SUCCESS;
}

int ss_init_generic_controller(ss_generic_controller *controller){
    if (ss_init_button_presses(&(controller->buttons)) == SS_RETURN_ERROR){
        return SS_RETURN_ERROR;
    }

    init_joystick_left(controller->joystick_left);
    init_joystick_right(controller->joystick_right);

    init_trigger_left(controller->trigger_left);
    init_trigger_right(controller->trigger_right);

    return SS_RETURN_SUCCESS;
}

void ss_print_input(XINPUT_GAMEPAD *gamepad){
// A
// B
// X
// Y
// Back
// Start
// l t
// r t
// l s
// r s
// d u
// d d
// d l
// d r
    
    // check which values are pressed
    buttons.pressed[0] = gamepad->wButtons & XINPUT_GAMEPAD_A;
    buttons.pressed[1] = gamepad->wButtons & XINPUT_GAMEPAD_B;
    buttons.pressed[2] = gamepad->wButtons & XINPUT_GAMEPAD_X;
    buttons.pressed[3] = gamepad->wButtons & XINPUT_GAMEPAD_Y;
    buttons.pressed[4] = gamepad->wButtons & XINPUT_GAMEPAD_BACK;
    buttons.pressed[5] = gamepad->wButtons & XINPUT_GAMEPAD_START;
    buttons.pressed[6] = gamepad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
    buttons.pressed[7] = gamepad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
    buttons.pressed[8] = gamepad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    buttons.pressed[9] = gamepad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
    buttons.pressed[10] = gamepad->wButtons & XINPUT_GAMEPAD_DPAD_UP;
    buttons.pressed[11] = gamepad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
    buttons.pressed[12] = gamepad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
    buttons.pressed[13] = gamepad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

    // now go thru the pressed array and print out appropriate press statements
    for (int index = 0; index < BUTTON_COUNT; index++){
        if (buttons.pressed[index]){
            printf(BUTTON_FMT_STR, BUTTON_STRS[index]);
        }
    }

    float LX = gamepad->sThumbLX;
    float LY = gamepad->sThumbLY;

    float magnitude = sqrt(LX*LX + LY*LY);
    float mag2 = LX*LX + LY*LY;

    float normLX = LX / magnitude;
    float normLY = LY / magnitude;
    float normLX2 = (LX*LX) / mag2;
    float normLY2 = (LY*LY) / mag2;

    float norm_mag = 0;

    int deadzone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

    if (magnitude > deadzone){
        if (magnitude > SS_JOYSTICK_MAX){
            magnitude = SS_JOYSTICK_MAX;
        }

        magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

        norm_mag = magnitude / (SS_JOYSTICK_MAX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

        printf("norm (%f , %f );norm2 ( %f, %f);norm_mag %f %f \n",
            normLX, normLY, normLX2, normLY2, magnitude, norm_mag);
    }
}

//  STATIC IMPLEMENTATION   ===================================================

static void destroy_pressed_buttons(){
    if (buttons.pressed != NULL){
        free(buttons.pressed);
    }
    buttons.pressed = NULL;
}

static void init_joystick(ss_joystick_data *joystick){
    joystick->x = 0;
    joystick->y = 0;
    joystick->norm_x = 0;
    joystick->norm_y = 0;
}

static void init_joystick_left(ss_joystick_data *joystick){
    init_joystick(joystick);
    joystick->type = SS_GAMECONTROLLER_LEFT;
}

static void init_joystick_right(ss_joystick_data *joystick){
    init_joystick(joystick);
    joystick->type = SS_GAMECONTROLLER_RIGHT;
}

static int init_pressed_buttons(){
    buttons.size = BUTTON_COUNT;
    buttons.pressed = malloc(BUTTON_COUNT*sizeof(int));

    if (buttons.pressed == NULL){
        // error occured
        return SS_RETURN_ERROR;
    }

    for (int index = 0; index < BUTTON_COUNT; index++){
        buttons.pressed[index] = 0;
    }
    
    return SS_RETURN_SUCCESS;
}

static void init_trigger(ss_trigger_data *trigger){
    trigger->value = 0;
    trigger->magnitude = 0;
    trigger->norm_magnitude = 0;
}

static void init_trigger_left(ss_trigger_data *trigger){
    init_trigger(trigger);
    trigger->type = SS_GAMECONTROLLER_LEFT;
}

static void init_trigger_right(ss_trigger_data *trigger){
    init_trigger(trigger);
    trigger->type = SS_GAMECONTROLLER_RIGHT;
}

static void process_joystick_input(
        XINPUT_GAMEPAD *gamepad, 
        ss_joystick_data *joystick
){
    float x,y, magnitude;
    int deadzone; // deadzone of this joystick

    // first determine if this is a right or left joystick 
    // and retrieve the x and y values
    switch(joystick->type){
        case SS_GAMECONTROLLER_LEFT:
        {
            joystick->x = gamepad->sThumbLX;
            joystick->y = gamepad->sThumbLY;
            deadzone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            break;
        }
        case SS_GAMECONTROLLER_RIGHT:
        {
            joystick->x = gamepad->sThumbRX;
            joystick->y = gamepad->sThumbRY;
            deadzone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
            break;
        }
        default:
        {
            // error case here just return
            return;
        }
    }

    x = joystick->x;
    y = joystick->y;

    // distance from (0,0)
    magnitude = sqrt(x*x + y*y);

    if (magnitude > deadzone){
        
        // joystick is considered out of deadzone (player is using stick)
        joystick->norm_x = x / magnitude;
        joystick->norm_y = y / magnitude;

    }else{

        // joystick is in deadzone
        joystick->x = 0;
        joystick->y = 0;
        joystick->norm_x = 0;
        joystick->norm_y = 0;
    }
}

static void process_trigger_input(
        XINPUT_GAMEPAD *gamepad,
        ss_trigger_data *trigger
){
    float value, magnitude;

    // firt determine if this is a right or left trigger
    // and retrieve the value
    switch (trigger->type){
        case SS_GAMECONTROLLER_LEFT:
        {
            trigger->value = gamepad->bLeftTrigger;
            break;
        }
        case SS_GAMECONTROLLER_RIGHT:
        {
            trigger->value = gamepad->bRightTrigger;
            break;
        }
        default:
        {
            // error case here just return
            return;
        }
    }

    value = trigger->value;

    // distance from (0,0)
    magnitude = sqrt(value*value);

    // past deadzone
    if (magnitude > XINPUT_GAMEPAD_TRIGGER_THRESHOLD){

        // limit to max
        if (magnitude > SS_TRIGGER_MAX){
            magnitude = SS_TRIGGER_MAX;
        }

        magnitude -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

        trigger->magnitude = magnitude;
        trigger->norm_magnitude = magnitude / (SS_TRIGGER_MAX - 
                XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
    }else{
        trigger->magnitude = 0;
        trigger->norm_magnitude = 0;
    }
}

static void setup_button_strs(){
    strncpy(BUTTON_STRS[SS_BUTTON_A], BUTTON_A, sizeof(BUTTON_A));
    strncpy(BUTTON_STRS[SS_BUTTON_B], BUTTON_B, sizeof(BUTTON_B));
    strncpy(BUTTON_STRS[SS_BUTTON_X], BUTTON_X, sizeof(BUTTON_X));
    strncpy(BUTTON_STRS[SS_BUTTON_Y], BUTTON_Y, sizeof(BUTTON_Y));
    strncpy(BUTTON_STRS[SS_BUTTON_BACK], BUTTON_BACK, sizeof(BUTTON_BACK));
    strncpy(BUTTON_STRS[SS_BUTTON_START], BUTTON_START, sizeof(BUTTON_START));
    strncpy(BUTTON_STRS[SS_BUTTON_LEFT_THUMB], BUTTON_LEFT_THUMB, 
            sizeof(BUTTON_LEFT_THUMB));
    strncpy(BUTTON_STRS[SS_BUTTON_RIGHT_THUMB], BUTTON_RIGHT_THUMB, 
            sizeof(BUTTON_RIGHT_THUMB));
    strncpy(BUTTON_STRS[SS_BUTTON_LEFT_SHOULDER], BUTTON_LEFT_SHOULDER, 
            sizeof(BUTTON_LEFT_SHOULDER));
    strncpy(BUTTON_STRS[SS_BUTTON_RIGHT_SHOULDER], BUTTON_RIGHT_SHOULDER, 
            sizeof(BUTTON_RIGHT_SHOULDER));
    strncpy(BUTTON_STRS[SS_BUTTON_DPAD_UP], BUTTON_DPAD_UP, 
            sizeof(BUTTON_DPAD_UP));
    strncpy(BUTTON_STRS[SS_BUTTON_DPAD_DOWN], BUTTON_DPAD_DOWN, 
            sizeof(BUTTON_DPAD_DOWN));
    strncpy(BUTTON_STRS[SS_BUTTON_DPAD_LEFT], BUTTON_DPAD_LEFT, 
            sizeof(BUTTON_DPAD_LEFT));
    strncpy(BUTTON_STRS[SS_BUTTON_DPAD_RIGHT], BUTTON_DPAD_RIGHT,
            sizeof(BUTTON_DPAD_RIGHT));
}

