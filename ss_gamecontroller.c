/**
 * Implementation of Game Controller functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 */

//#include "SDL2/SDL.h"
#include <windows.h>
#include <xinput.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <inttypes.h>
#include <math.h>

#include "ss_constants.h"

#include "ss_gamecontroller.h"

//  CONSTANTS   ===============================================================

//static const int BUTTON_COUNT = 14;
#define BUTTON_COUNT 14
#define BUTTON_STR_SIZE 20
const int SS_BUTTON_COUNT = BUTTON_COUNT;

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

// left, right descriptions
static const char AXIS_LEFT[]       = "Left";
static const char AXIS_RIGHT[]      = "Right";
static const char AXIS_UNKNOWN[]    = "nani?";

// the fmt string we use
// button <button name> pressed
static const char BUTTON_FMT_STR[]      = "Button %s Pressed\n";

// Joystick <left | right> (<x>, <y>), (<normalized x>, <normalized y>)
static const char JOYSTICK_FMT_STR[]    = "Joystick %s: (%i, %i), (%f, %f)\n";

// Trigger <left | right> (<value>), (<magnitude>), (<normalized magnitude>)
static const char TRIGGER_FMT_STR[]     = "Trigger %s: (%i), (%f), (%f)\n";

static const char POLL_FMT_STR[]        = "Poll time: %"PRIi64"\n";

//  STATIC FUNCTIONS    =======================================================

/*
 * Deletes the button presses struct
 *
 * IN:
 *  @param buttons - pointer to the button presses struct to delete
 */
static void destroy_button_data(ss_button_data *buttons);

/*
 * Returns teh appropriate direction string for the given type
 *
 * IN:
 *  @param type - the SS_AXIS enum to get direction string for
 *
 * OUT:
 *  @returns the appropriate direction string 
 */
static const char* get_direction_str(SS_AXIS type);

/*
 * Initializes the given button data struct
 *
 * IN:
 *  @param buttons - the button presses struct to initializes
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_ERROR if not
 */
static int init_button_data(ss_button_data *buttons);

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
 * Returns true if this joystick is currently active (not in deadzone)
 *
 * IN:
 *  @param joystick - ss_joystick_data struct
 *
 * OUT:
 *  @returns true if joystick is active, false if not
 */
static bool is_joystick_active(ss_joystick_data *joystick);

/*
 * Returns true if this trigger is currently active (not in deadzone)
 *
 * IN:
 *  @param trigger - ss_Trigger_data struct
 *
 * OUT:
 *  @returns true if trigger is active, false if not
 */
static bool is_trigger_active(ss_trigger_data *trigger);

/*
 * Prints the buttons that are pressed
 *
 * IN:
 *  @param buttons - the ss_buttons_data struct
 */
static void print_button_data(ss_button_data *buttons);

/*
 * Prints the given joystick data nicely
 *
 * IN:
 *  @param joystick - the ss_joystick_data struct
 */
static void print_joystick_data(ss_joystick_data *joystick);

/*
 * Prints the given trigger data nicely
 *
 * IN:
 *  @param trigger - the ss_Trigger_data struct
 */
static void print_trigger_data(ss_trigger_data *trigger);

/*
 * Processes the input given from the gamepad into button struct
 *
 * IN:
 *  @param gamepad - XINPUT_GAMEPAD struct
 *
 * OUT:
 *  @param buttons - ss_button_presses struct filled out with input data
 */
static void process_button_input(
        XINPUT_GAMEPAD *gamepad,
        ss_button_data *buttons
);

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

void ss_destroy_gamecontroller(){
    // nothing here for now
} // ss_destroy_gamecontroller

void ss_destroy_generic_controller(ss_generic_controller *controller){
    destroy_button_data(&(controller->buttons));
} // ss_destroy_generic_controller

int ss_init_gamecontroller(){

    setup_button_strs();

    return SS_RETURN_SUCCESS;
} // ss_init_gamecontroller

int ss_init_generic_controller(ss_generic_controller *controller){
    if (init_button_data(&(controller->buttons)) == SS_RETURN_ERROR){
        return SS_RETURN_ERROR;
    }

    init_joystick_left(&(controller->joystick_left));
    init_joystick_right(&(controller->joystick_right));

    init_trigger_left(&(controller->trigger_left));
    init_trigger_right(&(controller->trigger_right));

    return SS_RETURN_SUCCESS;
} // ss_init_generic_controller

void ss_print_generic_controller(ss_generic_controller *controller){
    print_button_data(&(controller->buttons));
    print_joystick_data(&(controller->joystick_left));
    print_joystick_data(&(controller->joystick_right));
    print_trigger_data(&(controller->trigger_left));
    print_trigger_data(&(controller->trigger_right));

    printf(POLL_FMT_STR, controller->poll.QuadPart);
} // ss_print_generic_controller

void ss_process_input(
        ss_generic_controller *controller, 
        XINPUT_GAMEPAD *gamepad
){
    process_button_input(gamepad, &(controller->buttons));
    process_joystick_input(gamepad, &(controller->joystick_left));
    process_joystick_input(gamepad, &(controller->joystick_right));
    process_trigger_input(gamepad, &(controller->trigger_left));
    process_trigger_input(gamepad, &(controller->trigger_right));

    QueryPerformanceCounter(&(controller->poll));
} // ss_process_input

//  STATIC IMPLEMENTATION   ===================================================

static void destroy_button_data(ss_button_data *buttons){
    if (buttons->pressed != NULL){
        free(buttons->pressed);
        buttons->pressed = NULL;
    }

/*    if(buttons->states != NULL){
        free(buttons->states);
        buttons->states = NULL;
    }*/

    buttons->size = 0;
} // destroy_button_data

static const char* get_direction_str(SS_AXIS type){
    switch (type){
        case SS_GAMECONTROLLER_LEFT:
        {
            return AXIS_LEFT;
        }
        case SS_GAMECONTROLLER_RIGHT:
        {
            return AXIS_RIGHT;
        }
        default:
        {
            return AXIS_UNKNOWN;
        }
    }
} // get_direction_str

static int init_button_data(ss_button_data *buttons){
    buttons->pressed = malloc(BUTTON_COUNT*sizeof(int));
//    buttons->states = malloc(BUTTON_COUNT*sizeof(SS_BUTTON_STATE));

    if (buttons->pressed == NULL
            /*|| buttons->states == NULL*/){
        return SS_RETURN_ERROR;
    }

    buttons->size = BUTTON_COUNT;

    for (int index = 0; index < BUTTON_COUNT; index++){
        buttons->pressed[index] = 0;
//        buttons->states[index] = SS_BUTTON_RELEASED;
    }

    return SS_RETURN_SUCCESS;
} // init_button_data

static void init_joystick(ss_joystick_data *joystick){
    joystick->x = 0;
    joystick->y = 0;
    joystick->norm_x = 0;
    joystick->norm_y = 0;
} // init_joystick

static void init_joystick_left(ss_joystick_data *joystick){
    init_joystick(joystick);
    joystick->type = SS_GAMECONTROLLER_LEFT;
} // init_joystick_left

static void init_joystick_right(ss_joystick_data *joystick){
    init_joystick(joystick);
    joystick->type = SS_GAMECONTROLLER_RIGHT;
} // init_joystick_right

static void init_trigger(ss_trigger_data *trigger){
    trigger->value = 0;
    trigger->magnitude = 0;
    trigger->norm_magnitude = 0;
} // init_trigger

static void init_trigger_left(ss_trigger_data *trigger){
    init_trigger(trigger);
    trigger->type = SS_GAMECONTROLLER_LEFT;
} // init_trigger_left

static void init_trigger_right(ss_trigger_data *trigger){
    init_trigger(trigger);
    trigger->type = SS_GAMECONTROLLER_RIGHT;
} // init_trigger_right

static bool is_joystick_active(ss_joystick_data *joystick){
    return joystick->x != 0 || joystick->y != 0;
} // is_joystick_active

static bool is_trigger_active(ss_trigger_data *trigger){
    return trigger->value != 0;
} // is_trigger_active

static void print_button_data(ss_button_data *buttons){
    // now go thru the pressed array and print out appropriate press statements
    for (int index = 0; index < BUTTON_COUNT; index++){
        if (buttons->pressed[index]){
            printf(BUTTON_FMT_STR, BUTTON_STRS[index]);
        }
    }
} // print_button_data

static void print_joystick_data(ss_joystick_data *joystick){
    if (is_joystick_active(joystick)){

        printf(JOYSTICK_FMT_STR, get_direction_str(joystick->type), 
                joystick->x, joystick->y, joystick->norm_x, joystick->norm_y);
    }
} // print_joystick_data

static void print_trigger_data(ss_trigger_data *trigger){
    if (is_trigger_active(trigger)){

        printf(TRIGGER_FMT_STR, get_direction_str(trigger->type),
                trigger->value, trigger->magnitude, trigger->norm_magnitude);
    }
} // print_trigger_data

static void process_button_input(
        XINPUT_GAMEPAD *gamepad,
        ss_button_data *buttons
){
    // check which values are pressed
    buttons->pressed[SS_BUTTON_A] = gamepad->wButtons & XINPUT_GAMEPAD_A;
    buttons->pressed[SS_BUTTON_B] = gamepad->wButtons & XINPUT_GAMEPAD_B;
    buttons->pressed[SS_BUTTON_X] = gamepad->wButtons & XINPUT_GAMEPAD_X;
    buttons->pressed[SS_BUTTON_Y] = gamepad->wButtons & XINPUT_GAMEPAD_Y;
    buttons->pressed[SS_BUTTON_BACK] = gamepad->wButtons & XINPUT_GAMEPAD_BACK;
    buttons->pressed[SS_BUTTON_START] = 
        gamepad->wButtons & XINPUT_GAMEPAD_START;
    buttons->pressed[SS_BUTTON_LEFT_THUMB] = 
        gamepad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
    buttons->pressed[SS_BUTTON_RIGHT_THUMB] = 
        gamepad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
    buttons->pressed[SS_BUTTON_LEFT_SHOULDER] = 
        gamepad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    buttons->pressed[SS_BUTTON_RIGHT_SHOULDER] = 
        gamepad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
    buttons->pressed[SS_BUTTON_DPAD_UP] = 
        gamepad->wButtons & XINPUT_GAMEPAD_DPAD_UP;
    buttons->pressed[SS_BUTTON_DPAD_DOWN] = 
        gamepad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
    buttons->pressed[SS_BUTTON_DPAD_LEFT] = 
        gamepad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
    buttons->pressed[SS_BUTTON_DPAD_RIGHT] = 
        gamepad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
} // process_button_input

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
} // process_joystick_input

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
        trigger->value = 0;
        trigger->magnitude = 0;
        trigger->norm_magnitude = 0;
    }
} // process_trigger_input

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
} // setup_button_strs

