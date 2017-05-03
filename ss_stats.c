/**
 * Implementation of stats functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 * TODO: add error msgs specific to operation
 * TODO: also move io stuff to another file
 */

#include <windows.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <inttypes.h>
#include <math.h>
#include <string.h>

#include "ss_config.h"
#include "ss_constants.h"
#include "ss_gamecontroller.h"
#include "ss_gc_constants.h"

#include "ss_stats.h"

//  CONSTANTS   ===============================================================

// PI for atan calculation
#define PI 3.14159265

// how many slices we have for the joystick
#define SLICE_COUNT 8

// number of degrees in circle
#define CIRCLE_DEG 360

// stat file stuff
#define STAT_READ "r"
#define STAT_WRITE "w"

// buffer size
#define BUFFER_SIZE 512

// tok delimieters
#define TOK_DLM ":\n"

// number of joystick slices
const int SS_JOYSTICK_SLICE_COUNT       = SLICE_COUNT;

// head strings
static const char HEAD_BUTTON_STR[]     = "\nBUTTON STATS:\n";
static const char HEAD_JOYSTICK_STR[]   = "\nJOYSTICK STATS:\n";
static const char HEAD_TRIGGER_STR[]    = "\nTRIGGER STATS:\n";

// stat strings
static const char BUTTON_STAT_STR[]     = "Button: %-20s : %8"PRIu64" presses \
: %8"PRIu64" ms\n";
static const char JOYSTICK_STAT_STR[]   = "Joystick: %-20s : ms grid:\n";
static const char JOYSTICK_SLICE_STR[]  = " %8"PRIu64;
static const char TRIGGER_STAT_STR[]    = "Trigger: %-20s : %8"PRIu64" presses \
: %8"PRIu64" ms\n";

// largest strings
static const char LARGEST_TIME_STR[]    = "Largest time: %s : %"PRIu64"\n";
static const char LARGEST_PRESS_STR[]   = "Largest press: %s : %"PRIu64"\n";

// error message
static const char ERROR_GRIDSIZE[]      = "The gridsize is not even. Your \
program may be corrupted\n";

// joystick access pattern (generated at startup)
static SS_JOYSTICK_SLICE JOYSTICK_ACCESSPATTERN[SLICE_COUNT];

// write messages
static const char FILE_STAT_BUT_FMT[]   = "%i:%"PRIu64":%"PRIu64"\n";
static const char FILE_STAT_JOY_FMT[]   = "%i";
static const char FILE_STAT_JOY_SFMT[]  = ":%"PRIu64;

// read messages
static const char FILE_STAT_BUT_RD[]    = "%i:%"SCNu64":%"SCNu64;
static const char FILE_STAT_JOY_SRD[]   = "%"SCNu64;

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
 * Clears the given button stats struct
 *
 * OUT:
 *  @param stats - cleared button stats data
 */
static void clear_buttons(ss_button_stats *stats);

/*
 * Clears the given joystick grid struct
 *
 * OUT:
 *  @param grid - cleared joystick grid data
 */
static void clear_joygrid(ss_joystick_grid *grid);

/*
 * Clears the given joystick stats struct
 *
 * OUT:
 *  @param stats - cleared joystick stats data
 */
static void clear_joystick(ss_joystick_stats *stats);

/*
 * Clears the given trigger stats struct
 *
 * OUT:
 *  @param stats - cleared trigger stats dta
 */
static void clear_trigger(ss_trigger_stats *stats);

/*
 * Collapses the given degree into an appropriate pizza slice of the given
 * grid.
 *
 * IN:
 *  @param grid - ss_joystick_grid struct that contains the pizza to collapse
 *      to
 *  @param degree - the degree to collapse
 *
 * OUT:
 *  @returns the slice that the degree belongs in
 */
static SS_JOYSTICK_SLICE collapse_degree_to_slice(
        ss_joystick_grid *grid, 
        double degree
);

/*
 * Converts the given joystick coordinates to an appropriate slice index
 *
 * IN:
 *  @param grid - the ss_joystick_grid struct
 *  @param x - the x coord
 *  @param y - the y coord
 *
 * OUT:
 *  @returns the slice that the coordinates belong in
 */
static SS_JOYSTICK_SLICE convert_joystick_coordinate_to_slice(
        ss_joystick_grid *grid, 
        SHORT x,
        SHORT y
);

/*
 * Converts the given x and y coordinates into a degree. The returned degree
 * is in the correpsonding quadrant
 *
 * IN:
 *  @param x - x coordinate
 *  @param y - y coordinate
 *
 * OUT:
 *  @returns degree of the x and y coordinate
 */
static double convert_xy_to_degree(SHORT x, SHORT y);

/*
 * Performs a full copy of button stats data from src and stores it in dest
 *
 * Assumes dest is initialized
 *
 * IN:
 *  @param src - data to copy
 *  @param drawonly - only copies data needed for drawing
 *
 * OUT:
 *  @param dest - contains copied data
 */
static void copy_button(
        ss_button_stats *dest, 
        const ss_button_stats *src,
        bool drawonly
);

/*
 * Performs a full copy of joystick grid stats data from src and stores it in
 * dest
 *
 * Assumes dest is intiailized
 *
 * IN:
 *  @param src - data to copy
 *  @param drawonly - only copies data needed for drawing
 *
 * OUT:
 *  @param dest - contains copied data
 */
static void copy_joygrid(
        ss_joystick_grid *dest,
        const ss_joystick_grid *src,
        bool drawonly
);

/*
 * Performs a fully copy of joystick stats data from src and stores it in dest
 *
 * Assumes dest is intialized
 *
 * IN:
 *  @param src - data to copy
 *  @param drawonly - only copies data needed for drawing
 *
 * OUT:
 *  @param dest - contains copied data
 */
static void copy_joystick(
        ss_joystick_stats *dest,
        const ss_joystick_stats *src,
        bool drawonly
);

/*
 * Performs a full copy of trigger stats data from src and stores it in dest
 *
 * Assumes dest is intitalized
 *
 * IN:
 *  @param src - data to copy
 *  @param drawonly - only copies data needed for drawing
 *
 * OUT:
 *  @param dest - contains copied data
 */
static void copy_trigger(
        ss_trigger_stats *dest,
        const ss_trigger_stats *src,
        bool drawonly
);

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
 * Deactivates all active button inputs and properly updates stats
 *
 * IN:
 *  @param poll - end time
 *  @param freq - timer frequency
 *
 * OUT:
 *  @param stats - inputs are inactive 
 */
static void finish_buttons(
        ss_button_stats *stats,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
);

/*
 * Deactivates joystick and properly update stats
 *
 * IN:
 *  @param poll - end time
 *  @param freq - timer freq
 *
 * OUT:
 *  @param stats - input is inactive
 */
static void finish_joystick(
        ss_joystick_stats *stats,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
);

/*
 * Deactivates trigger and properly updates stats
 *
 * IN:
 *  @param poll - end time
 *  @param freq - timer freq
 *
 * OUT:
 *  @param stats - input is inactive
 */
static void finish_trigger(
        ss_trigger_stats *stats,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
);

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
 * Processes the selected button's input stats
 *
 * IN:
 *  @param button - index of the selected button to process
 *  @param poll - the poll time of the input
 *  @param freq - freq of timer
 *
 * OUT:
 *  @param stats - button stats struct to save datat to
 */
static void process_button(
        ss_button_stats *stats,
        int button,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
);

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
 * Processes the selected joystick slice's input stats
 *
 * IN:
 *  @param slice - index of the selected slice to process
 *  @param poll - the poll time of the input
 *  @param freq - freq of timer
 *
 * OUT:
 *  @param stats - joystick stats struct to save data to
 */
static void process_joystick(
        ss_joystick_stats *stats,
        int slice,
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
 * Processes the selected trigger's input stats
 *
 * IN:
 *  @param poll - the poll time of the input
 *  @param freq - freq of timer
 *
 * OUT:
 *  @param stats - the trigger stats struct to save data to
 */
static void process_trigger(
        ss_trigger_stats *stats,
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

/*
 * Sets up the Joystick accesspattern array 
 */
static void setup_joystick_accesspattern();

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

    setup_joystick_accesspattern();

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

    
    printf(HEAD_JOYSTICK_STR);
    print_joystick_stats(&(stats->joystick_left));
    print_joystick_stats(&(stats->joystick_right));
    //*/

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

    
    process_joystick_stats(&(stats->joystick_left), &(input->joystick_left),
            input->poll, input->freq);
    process_joystick_stats(&(stats->joystick_right), &(input->joystick_right),
            input->poll, input->freq);
    //*/

    process_trigger_stats(&(stats->trigger_left), &(input->trigger_left),
            input->poll, input->freq);
    process_trigger_stats(&(stats->trigger_right), &(input->trigger_right),
            input->poll, input->freq);
} // ss_process_stats

void ss_stats_clear(ss_generic_controller_stats *stats){

    clear_buttons(&(stats->buttons));

    clear_joystick(&(stats->joystick_left));
    clear_joystick(&(stats->joystick_right));

    clear_trigger(&(stats->trigger_left));
    clear_trigger(&(stats->trigger_right));

} // ss_stats_clear

void ss_stats_copy(
        ss_generic_controller_stats *dest,
        const ss_generic_controller_stats *src,
        bool drawonly
){
    copy_button(&(dest->buttons), &(src->buttons), drawonly);

    copy_joystick(&(dest->joystick_left), &(src->joystick_left), drawonly);
    copy_joystick(&(dest->joystick_right), &(src->joystick_right), drawonly);

    copy_trigger(&(dest->trigger_left), &(src->trigger_left), drawonly);
    copy_trigger(&(dest->trigger_right), &(src->trigger_right), drawonly);
} // ss_stats_copy

void ss_stats_finishstate(ss_generic_controller_stats *stats){

    LARGE_INTEGER poll, freq;

    QueryPerformanceCounter(&poll);
    QueryPerformanceFrequency(&freq);

    finish_buttons(&(stats->buttons), poll, freq);

    finish_joystick(&(stats->joystick_left), poll, freq);
    finish_joystick(&(stats->joystick_right), poll, freq);

    finish_trigger(&(stats->trigger_left), poll, freq);
    finish_trigger(&(stats->trigger_right), poll, freq);

} // ss_stats_finishstate

int ss_stats_read(
        ss_generic_controller_stats *stats,
        const char *filepath
){
    FILE *file;
    char buffer[BUFFER_SIZE];
    char *token;
    int rc;

    // input
    int id;
    ss_statnum prscts;
    ss_statnum prstim;

    file = fopen(filepath, STAT_READ);

    if (file == NULL){
        return SS_RETURN_FAILURE;
    }

    // ensure the read in file has ps3
    if (fgets(buffer, BUFFER_SIZE, file) == NULL){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // check for PS3 header
    if (strncmp(buffer, "PS3", 3) != 0){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // now start button reads
    for (int index = 0; index < SS_BUTTON_COUNT; index++){
        
        // grab line
        if (fgets(buffer, BUFFER_SIZE, file) == NULL){
            fclose(file);
            return SS_RETURN_FAILURE;
        }

        // parse line
        rc = sscanf(buffer, FILE_STAT_BUT_RD, &id, &prscts, &prstim);

        // expecting 3 arguments
        // also making sure index and id match
        if (rc != 3 || id != index){
            fclose(file);
            return SS_RETURN_FAILURE;
        }

        stats->buttons.press_counts[index] = prscts;
        stats->buttons.press_times_ms[index] = prstim;

        set_largest_if_largest(&(stats->buttons.largest), prstim);
    }

    // triggers
    // left trigger
    if (fgets(buffer, BUFFER_SIZE, file) == NULL){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // parse line
    rc = sscanf(buffer, FILE_STAT_BUT_RD, &id, &prscts, &prstim);

    if (rc != 3 || id != SS_TRIGGER_LEFT){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // save data
    stats->trigger_left.press_count = prscts;
    stats->trigger_left.press_time = prstim;

    // right trigger
    if (fgets(buffer, BUFFER_SIZE, file) == NULL){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // parse line
    rc = sscanf(buffer, FILE_STAT_BUT_RD, &id, &prscts, &prstim);

    if (rc != 3 || id != SS_TRIGGER_RIGHT){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // save data
    stats->trigger_right.press_count = prscts;
    stats->trigger_right.press_time = prstim;

    // joysticks
    // left joystick
    if (fgets(buffer, BUFFER_SIZE, file) == NULL){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // tokenize
    token = strtok(buffer, TOK_DLM);
    
    // check for matching id
    rc = sscanf(token, FILE_STAT_JOY_FMT, &id);

    if (rc != 1 || id != SS_JOYSTICK_LEFT){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // now begin loop for slices
    for (int index = 0; index < SLICE_COUNT; index++){

        // tokenize
        token = strtok(NULL, TOK_DLM);

        if (token == NULL){
            fclose(file);
            return SS_RETURN_FAILURE;
        }

        // parse token
        rc = sscanf(token, FILE_STAT_JOY_SRD, &prstim);

        if (rc != 1){
            fclose(file);
            return SS_RETURN_FAILURE;
        }

        stats->joystick_left.data.pizza[index] = prstim;

        set_largest_if_largest(&(stats->joystick_left.data.largest), prstim);
    }

    // right joystick
    if (fgets(buffer, BUFFER_SIZE, file) == NULL){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    token = strtok(buffer, TOK_DLM);

    // check for matching id
    rc = sscanf(token, FILE_STAT_JOY_FMT, &id);

    if (rc != 1 || id != SS_JOYSTICK_RIGHT){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // now begin loop for slices
    for (int index = 0; index < SLICE_COUNT; index++){

        // tokenize
        token = strtok(NULL, TOK_DLM);

        if (token == NULL){
            fclose(file);
            return SS_RETURN_FAILURE;
        }

        // parse token
        rc = sscanf(token, FILE_STAT_JOY_SRD, &prstim);

        if (rc != 1){
            fclose(file);
            return SS_RETURN_FAILURE;
        }

        stats->joystick_right.data.pizza[index] = prstim;

        set_largest_if_largest(&(stats->joystick_right.data.largest), prstim);
    }

    // successful reading
    fclose(file);
    return SS_RETURN_SUCCESS;

} // ss_stats_read

int ss_stats_write(
        const ss_generic_controller_stats *stats, 
        const char *filepath
){

    FILE *file;

    file = fopen(filepath, STAT_WRITE);

    if (file == NULL){
        return SS_RETURN_FAILURE;
    }

    // write identiifier
    if (fprintf(file, "PS3\n") < 0){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // now for the buttons
    for (int index = 0; index < SS_BUTTON_COUNT; index++){
        if (fprintf(file, FILE_STAT_BUT_FMT, index, 
                    stats->buttons.press_counts[index],
                    stats->buttons.press_times_ms[index]) 
                < 0){
            fclose(file);
            return SS_RETURN_FAILURE;
        }
    }

    // and triggers
    if (fprintf(file, FILE_STAT_BUT_FMT, SS_TRIGGER_LEFT, 
                stats->trigger_left.press_count, 
                stats->trigger_left.press_time) < 0 
            || fprintf(file, FILE_STAT_BUT_FMT, SS_TRIGGER_RIGHT,
                stats->trigger_right.press_count,
                stats->trigger_right.press_time) < 0
            ){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // joysticks
    if (fprintf(file, FILE_STAT_JOY_FMT, SS_JOYSTICK_LEFT) < 0){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    for (int index = 0; index < SLICE_COUNT; index++){
        if (fprintf(file, FILE_STAT_JOY_SFMT, 
                    stats->joystick_left.data.pizza[index]) < 0){
            fclose(file);
            return SS_RETURN_FAILURE;
        }
    }

    if (fprintf(file, "\n") < 0
            || fprintf(file, FILE_STAT_JOY_FMT, SS_JOYSTICK_RIGHT) < 0){
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    for (int index = 0; index < SLICE_COUNT; index++){
        if (fprintf(file, FILE_STAT_JOY_SFMT,
                    stats->joystick_right.data.pizza[index]) < 0){
            fclose(file);
            return SS_RETURN_FAILURE;
        }
    }

    // success in writing
    fclose(file);
    return SS_RETURN_SUCCESS;
} // ss_stats_write

//  STATIC IMPLEMENTATION   ===================================================

static ss_statnum calculate_time_diff(
        LARGE_INTEGER start,
        LARGE_INTEGER end,
        LARGE_INTEGER freq
){

    return ((double)(1000 * (end.QuadPart - start.QuadPart))) / freq.QuadPart;
} // calcaulte_time_diff

static void clear_buttons(ss_button_stats *stats){

    for (int index = 0; index < stats->size; index++){
        stats->press_times_ms[index] = 0;
        stats->press_counts[index] = 0;
        stats->states[index] = SS_INPUT_INACTIVE;
    }

    stats->largest = 0;
} // clear_buttons

static void clear_joygrid(ss_joystick_grid *grid){

    for (int index = 0; index < grid->size; index++){
        grid->pizza[index] = 0;
    }

    grid->largest = 0;
    grid->prev = -1;
} // clear_joygrid

static void clear_joystick(ss_joystick_stats *stats){
    stats->state = SS_INPUT_INACTIVE;
    clear_joygrid(&(stats->data));
} // clear_joystick

static void clear_trigger(ss_trigger_stats *stats){
    stats->press_time = 0;
    stats->press_count = 0;
    stats->state = SS_INPUT_INACTIVE;
} // clear_trigger

static SS_JOYSTICK_SLICE collapse_degree_to_slice(
        ss_joystick_grid *grid, 
        double degree
){
    return ((int) ((CIRCLE_DEG + degree + grid->offset) / grid->spacing)) % grid->size;
} // collapse_degree_to_slice

static SS_JOYSTICK_SLICE convert_joystick_coordinate_to_slice(
        ss_joystick_grid *grid, 
        SHORT x,
        SHORT y
){
    return collapse_degree_to_slice(grid, convert_xy_to_degree(x,y));
} // convert_joystick_coordinate_to_slice


static double convert_xy_to_degree(SHORT x, SHORT y){
    return atan2(y,x) * 180 / PI;
} // convert_xy_to_degree

static void copy_button(
        ss_button_stats *dest, 
        const ss_button_stats *src,
        bool drawonly
){

    memcpy(dest->press_times_ms, src->press_times_ms, 
            src->size * sizeof(ss_statnum));
    memcpy(dest->press_counts, src->press_counts, 
            src->size * sizeof(ss_statnum));
    dest->largest = src->largest;
    dest->size = src->size;

    // non draw data next
    if (!drawonly){
        memcpy(dest->states, src->states, src->size * sizeof(SS_INPUT_STATE));
        memcpy(dest->start_times, src->start_times,
                src->size * sizeof(LARGE_INTEGER));
    }
} // copy_button

static void copy_joygrid(
        ss_joystick_grid *dest,
        const ss_joystick_grid *src,
        bool drawonly
){

    memcpy(dest->pizza, src->pizza, src->size * sizeof(ss_statnum));
    dest->largest = src->largest;
    dest->size = src->size;

    // non draw data next
    if (!drawonly){
        dest->offset = src->offset;
        dest->spacing = src->spacing;
        dest->prev = src->prev;
    }

} // copy_joygrid

static void copy_joystick(
        ss_joystick_stats *dest,
        const ss_joystick_stats *src,
        bool drawonly
){
    copy_joygrid(&(dest->data), &(src->data), drawonly);
    dest->type = src->type;

    if (!drawonly){
        dest->state = src->state;
        dest->start_time = src->start_time;
    }

} // copy_joystick

static void copy_trigger(
        ss_trigger_stats *dest,
        const ss_trigger_stats *src,
        bool drawonly
){
    *dest = *src;
} // copy_trigger

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
    if (grid->pizza != NULL){
        free(grid->pizza);
    }
    grid->pizza = NULL;
    grid->size = 0;
} // destroy_joystick_grid

static void destroy_joystick_stats(ss_joystick_stats *stats){
    destroy_joystick_grid(&(stats->data));
} // destroy_joystick_stats

static void finish_buttons(
        ss_button_stats *stats,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){
    for (int index = 0; index < stats->size; index++){
        switch (stats->states[index]){
            case SS_INPUT_ACTIVE:
            {
                process_button(stats, index, poll, freq);
                stats->states[index] = SS_INPUT_INACTIVE;
                break;
            }
            default:
            {
                // this is good
                break;
            }
        }
    }
} // finish_buttons

static void finish_joystick(
        ss_joystick_stats *stats,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){
    switch (stats->state){
        case SS_INPUT_ACTIVE:
        {
            process_joystick(stats, stats->data.prev, poll, freq);
            stats->data.prev = -1;
            stats->state = SS_INPUT_INACTIVE;
            break;
        }
        default:
        {
            // this is good
            break;
        }
    }
} // finish_joystick

static void finish_trigger(
        ss_trigger_stats *stats,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){
    switch (stats->state){
        case SS_INPUT_ACTIVE:
        {
            process_trigger(stats, poll, freq);
            stats->state = SS_INPUT_INACTIVE;
            break;
        }
        default:
        {
            // this is good
            break;
        }
    }
} // finish_trigger

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

    grid->pizza = malloc(SS_JOYSTICK_SLICE_COUNT*sizeof(ss_statnum));

    if (grid->pizza == NULL){
        return SS_RETURN_ERROR;
    }

    for (int index = 0; index < SS_JOYSTICK_SLICE_COUNT; index++){
        grid->pizza[index] = 0;
    }

    grid->prev = -1;
    grid->largest = 0;
    grid->size = SS_JOYSTICK_SLICE_COUNT;
    grid->spacing = CIRCLE_DEG / (double)SS_JOYSTICK_SLICE_COUNT;
    grid->offset = grid->spacing / 2.0;

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
    // special 0 (to make compiler shutup
    unsigned long long appeasement = 0;

    // condition
    bool has_printed_center = false;

    for (int index = 0; index < SS_JOYSTICK_SLICE_COUNT; index++){

        if (!has_printed_center && index == 4){
            // subtract index and print out a 0
            index -= 1;
            has_printed_center = true;
            printf(JOYSTICK_SLICE_STR, appeasement);

        }else{
            printf(JOYSTICK_SLICE_STR, 
                    grid->pizza[JOYSTICK_ACCESSPATTERN[index]]);
        }

        if (has_printed_center){
            if ((index+2) % 3 == 0){
                printf("\n");
            }
        }else{
            if ((index+1) % 3 == 0){
                // prints appropriate newline when appropriate
                printf("\n");
            }
        }
    }

    printf(LARGEST_TIME_STR, "", grid->largest);
    printf("\n");
} // print_joystick_grid

static void print_joystick_stats(ss_joystick_stats *stats){
    printf(JOYSTICK_STAT_STR, ss_get_direction_str(stats->type));
    print_joystick_grid(&(stats->data));
} // print_joystick_states

static void print_trigger_stats(ss_trigger_stats *stats){
    printf(TRIGGER_STAT_STR, ss_get_direction_str(stats->type),
            stats->press_count, stats->press_time);
} // print_trigger_stats

static void process_button(
        ss_button_stats *stats,
        int button,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){

    // add value to press count
    stats->press_counts[button] += 1;

    // calculate time diff and add to press time
    stats->press_times_ms[button] += calculate_time_diff(
            stats->start_times[button], poll, freq);

    // check if largest and set to largest (for drawing later)
    set_largest_if_largest(&(stats->largest), stats->press_times_ms[button]);

} // process_button

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
                    
                    process_button(stats, index, poll, freq);

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

static void process_joystick(
        ss_joystick_stats *stats,
        int slice,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){

    // add timee
    stats->data.pizza[slice] += 
        calculate_time_diff(stats->start_time, poll, freq);

    // set largest value if largest
    set_largest_if_largest(&(stats->data.largest), stats->data.pizza[slice]);

} // process_joystick

static void process_joystick_stats(
        ss_joystick_stats *stats,
        ss_joystick_data *input,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){
    int new_slice;

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
                    stats->data.prev = convert_joystick_coordinate_to_slice(
                            &(stats->data), input->x, input->y);

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
            
            // first calculate the new slice
            new_slice = convert_joystick_coordinate_to_slice(&(stats->data),
                    input->x, input->y);

            if (stats->data.prev != new_slice){
                // input location is different than previous

                process_joystick(stats, stats->data.prev, poll, freq);

                switch (input->state){
                    case SS_INPUT_ACTIVE:
                    {
                        // input is currently active, so prepare for next
                        // input

                        // save new location
                        stats->data.prev = new_slice;

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
} // process_joystick_stats

static void process_trigger(
        ss_trigger_stats *stats,
        LARGE_INTEGER poll,
        LARGE_INTEGER freq
){

    // add value to press count
    stats->press_count += 1;

    // calculate time diff and add to press time
    stats->press_time += calculate_time_diff(stats->start_time, poll, freq);

} // process_trigger

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

                process_trigger(stats, poll, freq);

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

static void setup_joystick_accesspattern(){

#ifdef SS_JOYGRID_FLIP

    JOYSTICK_ACCESSPATTERN[0]   = SS_SLICE_SOUTHEAST;
    JOYSTICK_ACCESSPATTERN[1]   = SS_SLICE_SOUTH;
    JOYSTICK_ACCESSPATTERN[2]   = SS_SLICE_SOUTHWEST;
    JOYSTICK_ACCESSPATTERN[3]   = SS_SLICE_EAST;
    JOYSTICK_ACCESSPATTERN[4]   = SS_SLICE_WEST;
    JOYSTICK_ACCESSPATTERN[5]   = SS_SLICE_NORTHEAST;
    JOYSTICK_ACCESSPATTERN[6]   = SS_SLICE_NORTH;
    JOYSTICK_ACCESSPATTERN[7]   = SS_SLICE_NORTHWEST;

#elif defined SS_JOYGRID_FLIP_Y

    JOYSTICK_ACCESSPATTERN[0]   = SS_SLICE_SOUTHWEST;
    JOYSTICK_ACCESSPATTERN[1]   = SS_SLICE_SOUTH;
    JOYSTICK_ACCESSPATTERN[2]   = SS_SLICE_SOUTHEAST;
    JOYSTICK_ACCESSPATTERN[3]   = SS_SLICE_WEST;
    JOYSTICK_ACCESSPATTERN[4]   = SS_SLICE_EAST;
    JOYSTICK_ACCESSPATTERN[5]   = SS_SLICE_NORTHWEST;
    JOYSTICK_ACCESSPATTERN[6]   = SS_SLICE_NORTH;
    JOYSTICK_ACCESSPATTERN[7]   = SS_SLICE_NORTHEAST;

#elif defined SS_JOYGRID_FLIP_X

    JOYSTICK_ACCESSPATTERN[0]   = SS_SLICE_NORTHEAST;
    JOYSTICK_ACCESSPATTERN[1]   = SS_SLICE_NORTH;
    JOYSTICK_ACCESSPATTERN[2]   = SS_SLICE_NORTHWEST;
    JOYSTICK_ACCESSPATTERN[3]   = SS_SLICE_EAST;
    JOYSTICK_ACCESSPATTERN[4]   = SS_SLICE_WEST;
    JOYSTICK_ACCESSPATTERN[5]   = SS_SLICE_SOUTHEAST;
    JOYSTICK_ACCESSPATTERN[6]   = SS_SLICE_SOUTH;
    JOYSTICK_ACCESSPATTERN[7]   = SS_SLICE_SOUTHWEST;

#else // !defined( SS_JOYGRID_FLIP | SS_JOYGRID_FLIP_Y | SS_JOYGRID_FLIP_X |

    JOYSTICK_ACCESSPATTERN[0]   = SS_SLICE_NORTHWEST;
    JOYSTICK_ACCESSPATTERN[1]   = SS_SLICE_NORTH;
    JOYSTICK_ACCESSPATTERN[2]   = SS_SLICE_NORTHEAST;
    JOYSTICK_ACCESSPATTERN[3]   = SS_SLICE_WEST;
    JOYSTICK_ACCESSPATTERN[4]   = SS_SLICE_EAST;
    JOYSTICK_ACCESSPATTERN[5]   = SS_SLICE_SOUTHWEST;
    JOYSTICK_ACCESSPATTERN[6]   = SS_SLICE_SOUTH;
    JOYSTICK_ACCESSPATTERN[7]   = SS_SLICE_SOUTHEAST;

#endif // !defined( SS_JOYGRID_FLIP | SS_JOYGRID_FLIP_Y | SS_JOYGRID_FLIP_X |

} // setup_joystick_accesspattern
