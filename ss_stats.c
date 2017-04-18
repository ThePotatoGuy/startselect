/**
 * Implementation of stats functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include <windows.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <inttypes.h>

#include "ss_constants.h"
#include "ss_gamecontroller.h"

#include "ss_stats.h"

//  CONSTANTS   ===============================================================

// how large to make the square grid for joystick
// MUST BE AN EVEN VALUE
// UNDEFINED FOR ODD
#define JOYSTICK_GRIDSIZE 100

// head strings
static const char HEAD_BUTTON_STR[]     = "\nBUTTON STATS:\n";
static const char HEAD_JOYSTICK_STR[]   = "\nJOYSTICK STATS:\n";
static const char HEAD_TRIGGER_STR[]    = "\nTRIGGER STATS:\n";

// stat strings
static const char BUTTON_STAT_STR[]     = "Button: %-20s : %8"PRIu64" presses \
: %8"PRIu64" ms\n";
static const char JOYSTICK_STAT_STR[]   = "Joystick: %-20s : ms grid:\n";
static const char JOYSTICK_GRID_STR[]   = "%8"PRIu64;
static const char TRIGGER_STAT_STR[]    = "Trigger: %-20s : %8"PRIu64" presses \
: %8"PRIu64" ms\n";

// largest strings
static const char LARGEST_TIME_STR[]    = "Largest time: %s : %"PRIu64"\n";
static const char LARGEST_PRESS_STR[]   = "Largest press: %s : %"PRIu64"\n";

// error message
static const char ERROR_GRIDSIZE[]      = "The gridsize is not even. Your \
program may be corrupted\n";

//  STATIC FUNCTIONS    =======================================================

/*
 * Calculates the time difference (in milliseconds) between the ending and 
 * start time, (assuming QueryPerformnanceCounter and 
 * QueryPerformanceFrequency calls are being used)
 *
 * IN:
 *  @param start - the start time
 *  @param end -  the end imte
 *  @param freq - time frequency according to QueryPerformanceFrequency
 *
 * OUT:
 *  @return the time difference in milliseconds between end and start
 */
static ss_statnum calculate_time_diff(
        LARGE_INTEGER start,
        LARGE_INTEGER end,
        LARGE_INTEGER freq
);

/*
 * Converts the given joystick coordinate such that it is within the 
 * valid ranges of the joystick grid
 * Assumes coord is within valid joystick range
 *
 * IN:
 *  @param grid - the ss_joystick_grid struct
 *  @param coord - the coordinate to convert
 *
 * OUT:
 *  @returns index that is within the joystick grid
 */
static int convert_joystick_coordinate(ss_joystick_grid *grid, SHORT coord);

/*
 * Deletes the given button stats struct
 *
 * IN:
 *  @param buttons - the ss_button_Stats struct to delete
 */
static void destroy_button_stats(ss_button_stats *buttons);

/*
 * Deletes the given joystick grid
 *
 * IN:
 *  @param grid - the ss_joystick grid struct to delete
 */
static void destroy_joystick_grid(ss_joystick_grid *grid);

/*
 * Deletse teh given joystick_stast struct
 *
 * IN:
 *  @param stas - the ss_joystick_stats struct to delete
 */
static void destroy_joystick_stats(ss_joystick_stats *stats);

/*
 * Initializes the given button stats struct
 *
 * IN:
 *  @param buttons - the ss_button_stats struct to tinitalizes
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_ERROR if not
 */
static int init_button_stats(ss_button_stats *buttons);

/*
 * Initalizes the given joystick grid
 *
 * IN:
 *  @param grid - the ss_joystick_grid struct to initalize
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_ERROR if not
 */
static int init_joystick_grid(ss_joystick_grid *grid);

/*
 * Initialies the left joystick stats struct
 *
 * IN:
 *  @param joystick - the ss_joystick_stats to initalize
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_ERROR if not
 */
static int init_joystick_left_stats(ss_joystick_stats *joystick);

/*
 * Initializes the right joystick stats struct
 *
 * IN:
 *  @param joystick - the ss_joystick_stats struct to initizlie
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_ERROR if not
 */
static int init_joystick_right_stats(ss_joystick_stats *joystick);

/*
 * Initializes the joystick stast struct for a generic joystick
 *
 * IN:
 *  @param joystick - the ss_joystick_stats struct to initalize
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS if successful, SS_RETURN_ERROR if not
 */
static int init_joystick_stats(ss_joystick_stats *joystick);

/*
 * Initializes the ss_Trigger_stats struct for the left trigger
 *
 * IN:
 *  @param trigger - the ss_trigger_stats struct to tinitalize
 */
static void init_trigger_left_stats(ss_trigger_stats *trigger);

/*
 * Initaizlies the ss_Trigger_stats struct fot eh right trigger
 *
 * IN:
 *  @Param trigger - the ss_trigger_stats struct to tinilize
 */
static void init_trigger_right_stats(ss_trigger_stats *trigger);

/*
 * Initlaizes the ss_trigger_stats struct for a generic trigger
 *
 * IN:
 *  @param trigger - the ss_trigger_stats struct to intialize
 */
static void init_trigger_stats(ss_trigger_stats *trigger);

/*
 * Checks if the given locations are different
 *
 * IN:
 *  @param preX - the previous x to check
 *  @param preY - the previous y to check
 *  @param newX - the new x to check
 *  @param newY - the new y to check
 *
 * OUT:
 *  @returns true if the given locations are different, false otherwise
 */
static bool is_location_different(int preX, int preY, int newX, int newY);

/*
 * Prints the ss_button_stats struct nicely
 *
 * IN:
 *  @param stats - the ss_button_stats struct to print
 */
static void print_button_stats(ss_button_stats *stats);

/*
 * Prints the ss_joystick_grid struct nicely
 *
 * IN:
 *  @param grid - the ss_joystick_grid struct to print
 */
static void print_joystick_grid(ss_joystick_grid *grid);

/*
 * Prints the ss_joystick stats struct nicely
 *
 * IN:
 *  @param stats - the ss_joystick_stats struct to print
 */
static void print_joystick_stats(ss_joystick_stats *stats);

/*
 * Prints the ss_trigger_stats struct nicely
 *
 * IN:
 *  @param stast - the ss_trigger_stast struct to print
 */
static void print_trigger_stats(ss_trigger_stats *stats);

/*
 * Process the ss_button_data input struct into the ss_button_stast struct
 *
 * IN:
 *  @param input - the ss_button_data struct to process
 *  @param poll - the poll time of the input
 *  @param freq - the frequency of the timer
 *
 * OUT:
 *  @param stast - the ss_button_stats struct to save data to
 */
static void process_button_stats(
        ss_button_stats *stats, 
        ss_button_data *input,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
);

/*
 * Processes the ss_joystick_data input struct into the ss_joystick_stats 
 * struct
 *
 * IN:
 *  @param input - the ss_joystick_data struct to process
 *  @param poll - the poll time of the input
 *  @param freq - the frequency of the timer
 *
 * OUT:
 *  @param stats - the ss_joystick_stast struct to save data to
 */
static void process_joystick_stats(
        ss_joystick_stats *stats,
        ss_joystick_data *joystick,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
);

/*
 * Processes the ss_trigger_data input struct into the ss_trigger_stats struct
 *
 * IN:
 *  @param input - the ss_trigger_data struct to process
 *  @param poll - the poll time of the input
 *  @param freq - the frequency of the timer
 *
 * OUT:
 *  @param stats - the ss_trigger_stats struct to save data to
 */
static void process_trigger_stats(
        ss_trigger_stats *stats,
        ss_trigger_data *trigger,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
);

/*
 * Sets the largest to the candidate if the candidate is larger than largest
 *
 * IN:
 *  @param candidate - the time value to check if largest
 *
 * OUT:
 *  @param largest - the largest value to check against 
 */
static void set_largest_if_largest(ss_statnum *largest, ss_statnum candidate);

//  IMPLEMENTATION  ===========================================================

void ss_destroy_generic_controller_stats(ss_generic_controller_stats *stats){
    destroy_button_stats(&(stats->buttons));
    destroy_joystick_stats(&(stats->joystick_left));
    destroy_joystick_stats(&(stats->joystick_right));
} // ss_destroy_generic_controller_stats

int ss_indexof_most_pressed_button(ss_button_stats *stats){
    int large_dex = 0;

    for (int index = 1; index < stats->size; index++){
        if (stats->press_counts[index] > stats->press_counts[large_dex]){
            large_dex = index;
        }
    }

    return large_dex;
} // ss_indexof_most_pressed_button

int ss_init_generic_controller_stats(ss_generic_controller_stats *stats){
    
    if(init_button_stats(&(stats->buttons)) == SS_RETURN_ERROR
            || init_joystick_left_stats(&(stats->joystick_left)) 
                == SS_RETURN_ERROR
            || init_joystick_right_stats(&(stats->joystick_right)) 
                == SS_RETURN_ERROR){

        return SS_RETURN_ERROR;
    }

    init_trigger_left_stats(&(stats->trigger_left));
    init_trigger_right_stats(&(stats->trigger_right));

    stats->largest = 0;

    return SS_RETURN_SUCCESS;
} // ss_init_generic_controller_stats

int ss_init_stats(){
    if (JOYSTICK_GRIDSIZE % 2 != 0){
        printf(ERROR_GRIDSIZE);
        return SS_RETURN_ERROR; // joystick gridsize must be even
    }

    return SS_RETURN_SUCCESS;
}

int ss_indexof_most_timed_button(ss_button_stats *stats){
    int large_dex = 0;

    for (int index = 1; index < stats->size; index++){
        if (stats->press_times_ms[index] > stats->press_times_ms[large_dex]){
            large_dex = index;
        }
    }

    return large_dex;
} // ss_indexof_most_timed_button

void ss_print_stats(ss_generic_controller_stats *stats){
    print_button_stats(&(stats->buttons));

    /*
    printf(HEAD_JOYSTICK_STR);
    print_joystick_stats(&(stats->joystick_left));
    print_joystick_stats(&(stats->joystick_right));
    */

    printf(HEAD_TRIGGER_STR);
    print_trigger_stats(&(stats->trigger_left));
    print_trigger_stats(&(stats->trigger_right));
} // ss_print_stats 

void ss_process_stats(
        ss_generic_controller_stats *stats,
        ss_generic_controller *input
){
    process_button_stats(&(stats->buttons), &(input->buttons), input->poll,
            input->freq);

    /*
    process_joystick_stats(&(stats->joystick_left), &(input->joystick_left),
            input->poll, input->freq);
    process_joystick_stats(&(stats->joystick_right), &(input->joystick_right),
            input->poll, input->freq);
    */

    process_trigger_stats(&(stats->trigger_left), &(input->trigger_left),
            input->poll, input->freq);
    process_trigger_stats(&(stats->trigger_right), &(input->trigger_right),
            input->poll, input->freq);
} // ss_process_stats

//  STATIC IMPLEMENTATION   ===================================================

static ss_statnum calculate_time_diff(
        LARGE_INTEGER start,
        LARGE_INTEGER end,
        LARGE_INTEGER freq
){

    return ((double)(1000 * (end.QuadPart - start.QuadPart))) / freq.QuadPart;
} // calcaulte_time_diff

static int convert_joystick_coordinate(ss_joystick_grid *grid, SHORT coord){
    return (int) ( (coord / grid->spacing) + (grid->size / 2.0) );
} // convert_joystick_coordinate

static void destroy_button_stats(ss_button_stats *buttons){
    if (buttons->press_times_ms != NULL){
        free(buttons->press_times_ms);
        buttons->press_times_ms = NULL;
    }

    if (buttons->press_counts != NULL){
        free(buttons->press_counts);
        buttons->press_counts = NULL;
    }

    if (buttons->states != NULL){
        free(buttons->states);
        buttons->states = NULL;
    }

    if (buttons->start_times != NULL){
        free(buttons->start_times);
        buttons->start_times = NULL;
    }
} // destroy_button_stats

static void destroy_joystick_grid(ss_joystick_grid *grid){
    if (grid->grid != NULL){
        for (int row = 0; row < grid->size; row++){
            if (grid->grid[row] != NULL){
                free(grid->grid[row]);
            }
        }
        free(grid->grid);
        grid->grid = NULL;
        grid->size = 0;
    }
} // destroy_joystick_grid

static void destroy_joystick_stats(ss_joystick_stats *stats){
    destroy_joystick_grid(&(stats->data));
} // destroy_joystick_stats

static int init_button_stats(ss_button_stats *buttons){
    buttons->press_times_ms = malloc(SS_BUTTON_COUNT*sizeof(ss_statnum));
    buttons->press_counts = malloc(SS_BUTTON_COUNT*sizeof(ss_statnum));
    buttons->states = malloc(SS_BUTTON_COUNT*sizeof(SS_INPUT_STATE));
    buttons->start_times = malloc(SS_BUTTON_COUNT*sizeof(LARGE_INTEGER));
   
    if (buttons->press_times_ms == NULL
            || buttons->press_counts == NULL
            || buttons->states == NULL
            || buttons->start_times == NULL){
        return SS_RETURN_ERROR;
    }

    for (int index = 0; index < SS_BUTTON_COUNT; index++){
        buttons->press_times_ms[index] = 0;
        buttons->press_counts[index] = 0;
        buttons->states[index] = SS_INPUT_INACTIVE;
    }

    buttons->largest = 0;
    buttons->size = SS_BUTTON_COUNT;

    return SS_RETURN_SUCCESS;
} // init_button_stats

static int init_joystick_grid(ss_joystick_grid *grid){
    int row; // control used to back out gracefully

    grid->grid = malloc(JOYSTICK_GRIDSIZE*sizeof(ss_statnum*));

    if (grid->grid == NULL){
        return SS_RETURN_ERROR;
    }

    for (row = 0; row < JOYSTICK_GRIDSIZE; row++){
        grid->grid[row] = malloc(JOYSTICK_GRIDSIZE*sizeof(ss_statnum));

        if (grid->grid[row] == NULL){
            // error occured while making this 2D array
            
            // this loop does graceful backout by freeing what we have already
            // made
            while (row > 0){
                free(grid->grid[--row]);
            }

            free(grid->grid);
            grid->grid = NULL;
            
            return SS_RETURN_ERROR;
        }

        // otherwise we can fill out the grid with 0s

        for (int col = 0; col < JOYSTICK_GRIDSIZE; col++){
            grid->grid[row][col] = 0;
        }
    } // for each row of the grid

    grid->x = 0;
    grid->y = 0;
    grid->largest = 0;
    grid->size = JOYSTICK_GRIDSIZE;
    grid->spacing = (SS_JOYSTICK_MAX - SS_JOYSTICK_MIN) / 
        (double) JOYSTICK_GRIDSIZE;

    return SS_RETURN_SUCCESS;
} // init_joystick_grid

static int init_joystick_left_stats(ss_joystick_stats *joystick){
    joystick->type = SS_GAMECONTROLLER_LEFT;
    return init_joystick_stats(joystick);
} // init_joystick_left_stats

static int init_joystick_right_stats(ss_joystick_stats *joystick){
    joystick->type = SS_GAMECONTROLLER_RIGHT;
    return init_joystick_stats(joystick);
} // init_joystick_right_stats

static int init_joystick_stats(ss_joystick_stats *joystick){
    //joystick->x = 0;
//    joystick->y = 0;
    joystick->state = SS_INPUT_INACTIVE;
    return init_joystick_grid(&(joystick->data));
} // init_joystick_stats

static void init_trigger_left_stats(ss_trigger_stats *trigger){
    trigger->type = SS_GAMECONTROLLER_LEFT;
    init_trigger_stats(trigger);
} // init_trigger_left_stats

static void init_trigger_right_stats(ss_trigger_stats *trigger){
    trigger->type = SS_GAMECONTROLLER_RIGHT;
    init_trigger_stats(trigger);
} // init_trigger_right_stats

static void init_trigger_stats(ss_trigger_stats *trigger){
    trigger->press_time = 0;
    trigger->press_count = 0;
    trigger->state = SS_INPUT_INACTIVE;
} // init_trigger_stats

static bool is_location_different(int preX, int preY, int newX, int newY){
    return (preX != newX) || (preY != newY);
} // is_location_different

static void print_button_stats(ss_button_stats *stats){
    int most_index;
    
    printf(HEAD_BUTTON_STR);

    for (int index = 0; index < stats->size; index++){
        printf(BUTTON_STAT_STR, ss_get_button_str(index), 
                stats->press_counts[index], stats->press_times_ms[index]);
    }

    most_index = ss_indexof_most_pressed_button(stats);
    printf(LARGEST_PRESS_STR,ss_get_button_str(most_index),
            stats->press_counts[most_index]);

    most_index = ss_indexof_most_timed_button(stats);
    printf(LARGEST_TIME_STR, ss_get_button_str(most_index),
            stats->press_times_ms[most_index]);
} // print_button_stats

static void print_joystick_grid(ss_joystick_grid *grid){
    for (int row = 0; row < JOYSTICK_GRIDSIZE; row++){
        for (int col = 0; col < JOYSTICK_GRIDSIZE; col++){
            printf(JOYSTICK_GRID_STR, grid->grid[row][col]);
        }
    }
} // print_joystick_grid

static void print_joystick_stats(ss_joystick_stats *stats){
    printf(JOYSTICK_STAT_STR, ss_get_direction_str(stats->type));
    print_joystick_grid(&(stats->data));
} // print_joystick_states

static void print_trigger_stats(ss_trigger_stats *stats){
    printf(TRIGGER_STAT_STR, ss_get_direction_str(stats->type),
            stats->press_count, stats->press_time);
} // print_trigger_stats

static void process_button_stats(
        ss_button_stats *stats, 
        ss_button_data *input,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){
    for (int index = 0; index < input->size; index++){
        if (input->pressed[index] != stats->states[index]){
            // current button is different than previous saved state

            switch (stats->states[index]){
                case SS_INPUT_INACTIVE:
                {
                    // the button was prreviuos released, which means we just
                    // detected a press
                    stats->states[index] = SS_INPUT_ACTIVE;

                    // timer
                    QueryPerformanceCounter(&(stats->start_times[index])); 

                    break;
                }
                case SS_INPUT_ACTIVE:
                {
                    // the button was previously pressed, whcih means we just
                    // detected a release
                    stats->states[index] = SS_INPUT_INACTIVE;

                    // add value to press count
                    stats->press_counts[index] += 1;

                    // calculate time diff and add to press time
                    stats->press_times_ms[index] += calculate_time_diff(
                            stats->start_times[index], poll, freq);

                    // check if largest and set to largest (for drawing later)
                    set_largest_if_largest(&(stats->largest), 
                            stats->press_times_ms[index]);
                    break;
                }
                default:
                {
                    // noting happens i guess
                    break;
                }
            }
        }
    }
} // process_button_stats

static void process_joystick_stats(
        ss_joystick_stats *stats,
        ss_joystick_data *input,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){
    int newX, newY;

    switch (stats->state){
        case SS_INPUT_INACTIVE:
        {
            // the current input is inactive, check for activity and do stuff
            // accordingly
            switch (input->state){
                case SS_INPUT_ACTIVE:
                {
                    // activity occured, time to save the state of this 
                    // joystick
                    stats->state = SS_INPUT_ACTIVE;
                    stats->data.x = 
                        convert_joystick_coordinate(&(stats->data), input->x);
                    stats->data.y = 
                        convert_joystick_coordinate(&(stats->data), input->y);

                    // timer
                    QueryPerformanceCounter(&(stats->start_time));

                    break;
                }
                case SS_INPUT_INACTIVE:
                default:
                {
                    // nothing 
                    break;
                }
            }

            break;
        } // stats->state == SS_INPUT_INACTIVE
        case SS_INPUT_ACTIVE:
        {
            // the current input is active, so its time to check for 
            // location change as well as inactivity
            
            // first calculate the new indexes
            newX = convert_joystick_coordinate(&(stats->data), input->x);
            newY = convert_joystick_coordinate(&(stats->data), input->y);

            if (is_location_different(stats->data.x, stats->data.y, newX, 
                        newY)){
                // input location is different than previous

                // add the time difference to previous location
                stats->data.grid[stats->data.x][stats->data.y] +=
                    calculate_time_diff(stats->start_time, poll, freq);

                switch (input->state){
                    case SS_INPUT_ACTIVE:
                    {
                        // input is currently active, so prepare for next
                        // input

                        // save new location
                        stats->data.x = newX;
                        stats->data.y = newY;

                        // timer
                        QueryPerformanceCounter(&(stats->start_time));

                        break;
                    }
                    case SS_INPUT_INACTIVE:
                    {
                        // input is currently inactive, so change the saved
                        // state
                        stats->state = SS_INPUT_INACTIVE;
                        break;
                    }
                    default:
                    {
                        // nothing happens here
                        break;
                    }
                }
            }

            break;
        }
        default:
        {
            // nothing
            break;
        }
    }
    // current joystick state is different than previuos saved state
        //
        // okay so the way this will work is we will save previous location
        // and if the current location is different than the previous location

        
} // process_joystick_stats

static void process_trigger_stats(
        ss_trigger_stats *stats,
        ss_trigger_data *input,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){
    if (input->state != stats->state){
        // current trigger state is different than previous saved state
        
        switch (stats->state){
            case SS_INPUT_INACTIVE:
            {
                // the trigger was previously released, which means we just 
                // detected a press
                stats->state = SS_INPUT_ACTIVE;

                // timer
                QueryPerformanceCounter(&(stats->start_time));

                break;
            }
            case SS_INPUT_ACTIVE:
            {
                // the trigger was previously pressed, which means we just
                // detected a release
                stats->state = SS_INPUT_INACTIVE;

                // add value to press count
                stats->press_count += 1;

                // calculate time diff and add to press time
                stats->press_time += calculate_time_diff(
                        stats->start_time, poll, freq);

                break;
            }
            default:
            {
                // nothing happens i guess
                break;
            }
        }
    }
} // process_trigger_stats

static void set_largest_if_largest(ss_statnum *largest, ss_statnum candidate){

    if (candidate > *largest){
        *largest = candidate;
    }
} // set_largest_if_largest
