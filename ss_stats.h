/**
 * Stats Functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains functions related to stats about game controller
 * This includes stats on buttons 
 * TODO: button combinations maybe?
 */

#ifndef SS_STATS_H
#define SS_STATS_H

#include <windows.h>

#include "ss_gamecontroller.h"

//  TYPES   ===================================================================

// the large number type we are using for all timing stats
typedef unsigned long long ss_statnum;

// struct that contains stats about buttons
// indexes correspond to the ones defined in ss_gamecontroller.h
typedef struct{

    // array of amount of time a button hsa been pressed, in miliseconds
    ss_statnum *press_times_ms;

    // array of number of times a button has been pressed
    // press = press + release
    ss_statnum *press_counts;
    
    // array of the states of a button
    SS_INPUT_STATE *states;

    // array of button pressed start times
    LARGE_INTEGER *start_times;

    // largest time value of these buttons
    ss_statnum largest;

    // size of the above arrays
    int size;
} ss_button_stats;

// special struct that contains the represntation of joystick data in a grid
// > divide a joystick's motion area into a square grid of size x (adjustable)
// > a joystick's x and y is translated into a box onto this grid, and the 
//  value in that box is incremented appropriately
typedef struct{

    // 2Ds square array of the grid
    ss_statnum **grid;

    // largest time value in this grid
    ss_statnum largest;

    // size of the square array
    int size;
} ss_joystick_grid;

// struct that contains stats about a joystick
typedef struct{

    // type of joystick this stats represents 
    SS_AXIS type;

    // the stats about the joystick
    ss_joystick_grid data;

    // the saved state pf this joystick
    SS_INPUT_STATE state;
} ss_joystick_stats;

// struct that contains stats about a trigger
typedef struct{

    // type of trigger this stats represntes
    SS_AXIS type;

    // the amount of press time this trigger has
    ss_statnum press_time;

    // the amoun tof presses this trigger has (same def as button)
    ss_statnum press_count;

    // the saved state pof this trigger
    SS_INPUT_STATE state;

    // trigger pressed start time
    LARGE_INTEGER start_time;

} ss_trigger_stats;

// struct that contains stats about a generic controller
typedef struct{

    // stats about the buttons 
    ss_button_stats buttons;

    // stats about teh joysticks of this controller
    ss_joystick_stats joystick_left;
    ss_joystick_stats joystick_right;

    // stats about the triggers of this controller
    ss_trigger_stats trigger_left;
    ss_trigger_stats trigger_right;

    // largest time value
    ss_statnum largest;
} ss_generic_controller_stats;

//  FUNCTIONS   ===============================================================

/*
 * Deeltes the generic controller stats struct
 *
 * IN:
 *  @param stats - the generic controoler stats struct to delete
 */
void ss_destroy_generic_controller_stats(ss_generic_controller_stats *stats);

/*
 * Initializes the generic controller stats struct
 *
 * IN:
 *  @param stats - the generic controller stast struct to initialize
 *
 * OUT:
 *  @returns SS_REUTRN_SUCCESS if successful, SS_REUTRN_ERROR if not
 */
int ss_init_generic_controller_stats(ss_generic_controller_stats *stats);

/*
 * Prints the given ss_generic_controller_stats struct nicely
 *
 * IN:
 *  @param stats - the ss_generic_controller_stats struct to print
 */
void ss_print_stats(ss_generic_controller_stats *stats);

/*
 * Processes the stats from the input into the given stats struct
 *
 * IN:
 *  @param input - the ss_generic_controller struct to process
 *
 * OUT:
 *  @param stats - the ss_generic_controller_stats struct to write data to
 */
void ss_process_stats(
        ss_generic_controller_stats *stats,
        ss_generic_controller *input
);

#endif
