/**
 * Game Controller Functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains functions related to game controller input
 * This includes functions for printing values input.
 * TODO: move ps3 related stuff to ps3 file
 */

#ifndef SS_GAMECONTROLLER_H
#define SS_GAMECONTROLLER_H

#include <windows.h>
#include <xinput.h>

//  TYPES   ===================================================================

// left and right types for joystick and trigger
typedef enum {
    SS_GAMECONTROLLER_LEFT,
    SS_GAMECONTROLLER_RIGHT
} SS_AXIS;

// state of input
typedef enum {
    SS_INPUT_INACTIVE = 0,
    SS_INPUT_ACTIVE = 1,
} SS_INPUT_STATE;

// struct that contains data about pressed buttons
typedef struct{

    // array of pressed buttons where each index corresponds to a particular
    // button. SEE CONSTANTS
    int *pressed;

    // size of the above arrays
    int size;
} ss_button_data;

// struct that contains data about a joystick
typedef struct{
    
    // type of joystick this data represents
    SS_AXIS type;

    // the state of this joystick (active or inactive)
    SS_INPUT_STATE state;
    
    // x and y locations of this joystick
    SHORT x;
    SHORT y;

    // normalized x and y
    float norm_x;
    float norm_y;
} ss_joystick_data;

// struct that contains data about a trigger
typedef struct{

    // type of trigger this data represents
    SS_AXIS type;

    // the state of this trigger
    SS_INPUT_STATE state;

    // value of the trigger
    int value;

    // how far this trigger is pushed
    float magnitude;
    float norm_magnitude; // above value between 0.0 and 1.0
} ss_trigger_data;

// struct that contains data about a controller (generic version)
typedef struct{

    // the time this generic controller was processed for input
    LARGE_INTEGER poll;

    // frequency of the time (for timing)
    LARGE_INTEGER freq;

    // data about the buttons of this controller
    ss_button_data buttons;

    // data about the joysticks of this controller
    ss_joystick_data joystick_left;
    ss_joystick_data joystick_right;

    // data about the triggers of this controller
    ss_trigger_data trigger_left;
    ss_trigger_data trigger_right;
} ss_generic_controller;

//  CONSTANTS   ===============================================================

// button indexes
extern const int SS_BUTTON_A;
extern const int SS_BUTTON_B;
extern const int SS_BUTTON_X;
extern const int SS_BUTTON_Y;
extern const int SS_BUTTON_BACK;
extern const int SS_BUTTON_START;
extern const int SS_BUTTON_LEFT_THUMB;
extern const int SS_BUTTON_RIGHT_THUMB;
extern const int SS_BUTTON_LEFT_SHOULDER;
extern const int SS_BUTTON_RIGHT_SHOULDER;
extern const int SS_BUTTON_DPAD_UP;
extern const int SS_BUTTON_DPAD_DOWN;
extern const int SS_BUTTON_DPAD_LEFT;
extern const int SS_BUTTON_DPAD_RIGHT;

// trigger and joystick indexes
// this is mainly for canvas usage 
// TODO move this (and the other indexes) to seperate files that handle
// controller constants (like one for ps3, one for xbone, one for steam, and
// so on
extern const int SS_TRIGGER_LEFT;
extern const int SS_TRIGGER_RIGHT;
extern const int SS_JOYSTICK_LEFT;
extern const int SS_JOYSTICK_RIGHT;

// joystick values
extern const int SS_JOYSTICK_MAX;
extern const int SS_JOYSTICK_MIN;

// trigger values
extern const int SS_TRIGGER_MAX;
extern const int SS_TRIGGER_MIN;

// number of buttons
extern const int SS_BUTTON_COUNT;

//  FUNCTIONS   ===============================================================

/*
 * Deletes game controller data
 */
void ss_destroy_gamecontroller();

/*
 * Deletes the generic controller struct
 *
 * IN:
 *  @param controller - pointer to teh generic controller struct to delete
 */
void ss_destroy_generic_controller(ss_generic_controller *controller);

/*
 * Retrievesa teh button string for the given button index
 *
 * IN:
 *  @param button_index - the index of the button string to get
 *
 * OUT:
 *  @returns the button string that you were looking for
 */
const char* ss_get_button_str(int button_index);

/*
 * Returns teh appropriate direction string for the given type
 *
 * IN:
 *  @param type - the SS_AXIS enum to get direction string for
 *
 * OUT:
 *  @returns the appropriate direction string 
 */
const char* ss_get_direction_str(SS_AXIS type);

/*
 * Initializes the gamecontroller data
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_ERROR if not
 */
int ss_init_gamecontroller();

/*
 * Initializes the generic controller struct
 *
 * IN:
 *  @param controller - the generic controller struct to initlaize
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_ERROR if not
 */
int ss_init_generic_controller(ss_generic_controller *controller);

/*
 * Prints the current state of the given generic controller struct
 *
 * IN:
 *  @param controller - the generic controller struct
 */
void ss_print_generic_controller(ss_generic_controller *controller);

/*
 * Process the input into an ss_generic controller struct
 *
 * IN:
 *  @param gamepad - the XINPUT_GAMEPAD struct
 *
 * OUT:
 *  @param controller - ss_generic_controller struct filled with input info
 */
void ss_process_input(
        ss_generic_controller *controller,
        XINPUT_GAMEPAD *gamepad
);

#endif
