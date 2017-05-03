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

#include <inttypes.h>

#include "ss_gamecontroller.h"

//  TYPES   ===================================================================

// the large number type we are using for all timing stats
typedef uint64_t ss_statnum;

// slices indexes
typedef enum{
    SS_SLICE_EAST = 0,
    SS_SLICE_NORTHEAST = 1,
    SS_SLICE_NORTH = 2,
    SS_SLICE_NORTHWEST = 3,
    SS_SLICE_WEST = 4,
    SS_SLICE_SOUTHWEST = 5,
    SS_SLICE_SOUTH = 6,
    SS_SLICE_SOUTHEAST = 7,
    SS_SLICE_NONE = -1
} SS_JOYSTICK_SLICE;

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
// > divide a joystick's motion area into 8 pizza slices (non adjustable)
// > a joystick's x and y is translated into a slice, and its value in that box
// > is incremented appropriately
typedef struct{

    // array of the slices, starting from east going counter clockwise
    ss_statnum *pizza;

    // largest time value in this pizza
    ss_statnum largest;

    // offset value to adjust degree
    double offset;

    // spacing value of the slices (in degrees)
    double spacing;

    // saved previous slice
    SS_JOYSTICK_SLICE prev;

    // number of slices
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

    // saved time
    LARGE_INTEGER start_time;

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

//  CONSTANTS   ===============================================================

// the number of joystick slices 
extern const int SS_JOYSTICK_SLICE_COUNT;

//  FUNCTIONS   ===============================================================

/*
 * Deeltes the generic controller stats struct
 *
 * IN:
 *  @param stats - the generic controoler stats struct to delete
 */
void ss_destroy_generic_controller_stats(ss_generic_controller_stats *stats);

/*
 * Retrieves the index of the most pressed button
 * (The button with the most presses)
 *
 * IN:
 *  @param stats - the ss_button_stats struct to check 
 *
 * OUT:
 *  @returns the index of the most pressed button
 */
int ss_indexof_most_pressed_button(ss_button_stats *stats);

/*
 * Retrieves the index of the most timed button
 * (The button with the most press tmie)
 *
 * IN:
 *  @param stats - the ss_button_stats struct to check
 *
 * OUT:
 *  @returns the index of the most timed button
 */
int ss_indexof_most_timed_button(ss_button_stats *stats);

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
 * Initializes internal stats data
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_ERROr if not
 */
int ss_init_stats();

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

/*
 * Performs a memcpy of the data (full copy) from src and stores it in dest
 *
 * Assumes dest is intialized
 *
 * IN:
 *  @param src - the ss_generic_controller struct to copy
 *  @param drawonly - only copies data necessary for drawing
 *
 * OUT:
 *  @param dest - the copied ss_generic_controller struct
 */
void ss_stats_copy(
        ss_generic_controller_stats *dest,
        const ss_generic_controller_stats *src,
        bool drawonly
);

/*
 * Reads the given file into the given stats struct. 
 *
 * Assumes stats is intialized
 *
 * IN:
 *  @param stats - intailzied stast struct
 *  @param filepath - the path and name of file to read
 *
 * OUT:
 *  @param stats - stats struct filled with data
 *  @returns AES_RETURN_SUCCESS on success, AES_RETURN_FAILURE otherwise
 */
int ss_stats_read(
        ss_generic_controller_stats *stats,
        const char *filepath
);

/*
 * Writes the stats data to a file using the given filepath 
 *
 * IN
 *  @param stats - the stats data to save
 *  @param filepath - the path and name of file to save
 *
 * OUT:
 *  @returns AES_RETURN_SUCCESS on success, AES_RETURN_FAILURE otherwise
 */
int ss_stats_write(
        const ss_generic_controller_stats *stats, 
        const char *filepath
);

#endif
