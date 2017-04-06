/**
 * Implementation of stats functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include <windows.h>

#include <stdlib.h>

#include "ss_constants.h"
#include "ss_gamecontroller.h"

#include "ss_stats.h"

//  CONSTANTS   ===============================================================

// how large to make the square grid for joystick
#define JOYSTICK_GRIDSIZE 100

//  STATIC FUNCTIONS    =======================================================

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


//  IMPLEMENTATION  ===========================================================

void ss_destroy_generic_controller_stats(ss_generic_controller_stats *stats){
    destroy_button_stats(&(stats->buttons));
    destroy_joystick_stats(&(stats->joystick_left));
    destroy_joystick_stats(&(stats->joystick_right));
} // ss_destroy_generic_controller_stats

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

    return SS_RETURN_SUCCESS;
} // ss_init_generic_controller_stats

//  STATIC IMPLEMENTATION   ===================================================

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
    buttons->states = malloc(SS_BUTTON_COUNT*sizeof(SS_BUTTON_STATE));
    buttons->start_times = malloc(SS_BUTTON_COUNT*sizeof(LARGE_INTEGER));
   
    if (buttons->press_times_ms == NULL
            || buttons->press_counts == NULL
            || buttons->states == NULL
            || buttons->start_times == NULL){
        return SS_RETURN_ERROR;
    }

    buttons->size = SS_BUTTON_COUNT;

    for (int index = 0; index < SS_BUTTON_COUNT; index++){
        buttons->press_times_ms[index] = 0;
        buttons->press_counts[index] = 0;
        buttons->states[index] = SS_BUTTON_RELEASED;
    }

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

    grid->size = JOYSTICK_GRIDSIZE;

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
    trigger->state = SS_BUTTON_RELEASED;
} // init_trigger_stats

