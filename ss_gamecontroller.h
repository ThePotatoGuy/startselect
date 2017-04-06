/**
 * Game Controller Functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains functions related to game controller input
 * This includes functions for printing values input.
 * TODO: include functions for handling data
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

// state of a button
typedef enum {
    SS_BUTTON_RELEASED = 0,
    SS_BUTTON_PRESSED = 1
} SS_BUTTON_STATE;

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
