/**
 * Implementation of Stat processing
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include <limits.h>

#include "ss_canvas_color.h"
#include "ss_stats.h"

#include "ss_ps3_statcolors.h"

#include "ss_statprocessor.h"

//  STATIC FUNCTIONS    =======================================================

/*
 * Calculates the appropriate alpha for the given value (and largest value)
 * If value > larger, alpha will be maximum
 *
 * IN:
 *  @param value - the value to check
 *  @param largest - the largest value
 *
 * OUT:
 *  @returns the appropriate alpha, between 0 and 255
 */
static unsigned char calculate_alpha(ss_statnum value, ss_statnum largest);

/*
 * Converts the given button stats into color data
 *
 * IN:
 *  @param stats - the button stats to convert/process
 *  @param largest - the largest value to use
 *
 * OUT:
 *  @param colors - the color data with modified alpha
 */
static void colorizealpha_buttons(
        ss_ps3_colors *colors, 
        const ss_button_stats *stats,
        ss_statnum largest
);

/*
 * Converts the given joystick grid stats into color data
 *
 * IN:
 *  @param stats - the joystick stats to convert/process
 *  @param largest - the largest vlaue to use
 *
 * OUT:
 *  @param slices - ptr to the slices with modified alpha
 */
static void colorizealpha_joygrid(
        ss_canvas_color (*slices)[SS_PS3_JOY_SIZE],
        const ss_joystick_grid *stats,
        ss_statnum largest
);

/*
 * Converts the given joystick stast into color data
 *
 * IN:
 *  @param stats - the joystick stats to convert/process
 *  @param largest - the largest value to use
 *
 * OUT:
 *  @param colors - the color data with modified alpha
 */
static void colorizealpha_joystick(
        ss_ps3_colors *colors,
        const ss_joystick_stats *stats,
        ss_statnum largest
);

/*
 * Converts the given trigger stats into color data
 *
 * IN:
 *  @param stats - the trigger stats to convert/process
 *  @param largest - the largest value to use
 *
 * OUT:
 *  @poaram colors - the color data with modified alpha
 */
static void colorizealpha_trigger(
        ss_ps3_colors *colors,
        const ss_trigger_stats *trigger,
        ss_statnum largest
);

/*
 * Compares the two given values nad returns the largest
 *
 * IN:
 *  @param a - one value to check
 *  @param b - the other vluae to check
 *
 * OUT:
 *  @preturns the largest value
 */
static ss_statnum getlargest(ss_statnum a, ss_statnum b);

//  IMPLEMENTATION  ===========================================================

void ss_colorizealpha(
        ss_ps3_colors *colors, 
        const ss_generic_controller_stats *stats
){
    ss_statnum largestjoy, largestnonjoy;

    // get largest values
    largestjoy = getlargest(stats->joystick_left.data.largest,
            stats->joystick_right.data.largest);
    largestnonjoy = getlargest(stats->buttons.largest, getlargest(
                stats->trigger_left.press_time, stats->trigger_right.press_time));

    // now color everything
    colorizealpha_buttons(colors, &(stats->buttons), largestnonjoy);
    colorizealpha_joystick(colors, &(stats->joystick_left), largestjoy);
    colorizealpha_joystick(colors, &(stats->joystick_right), largestjoy);
    colorizealpha_trigger(colors, &(stats->trigger_left), largestnonjoy);
    colorizealpha_trigger(colors, &(stats->trigger_right), largestnonjoy);

} // ss_colorizealpha

//  STATIC IMPLEMENTAITON   ===================================================

static unsigned char calculate_alpha(ss_statnum value, ss_statnum largest){

    // largest check
    if (value >= largest){
        return UCHAR_MAX;
    }

    return (int) (UCHAR_MAX * (value / (double) largest));
} // calculate_alpha

static void colorizealpha_buttons(
        ss_ps3_colors *colors, 
        const ss_button_stats *stats,
        ss_statnum largest
){
    for (int index = 0; index < SS_BUTTON_COUNT; index++){
        if (stats->press_times_ms[index] > 0){
            // button has been pressed
            colors->buttons[index].a = calculate_alpha(
                    stats->press_times_ms[index], largest);
        }
    }
} // colorizealpha_buttons

static void colorizealpha_joygrid(
        ss_canvas_color (*slices)[SS_PS3_JOY_SIZE],
        const ss_joystick_grid *stats,
        ss_statnum largest
){
    for (int index = 0; index < SS_PS3_JOY_SIZE; index++){
        if (stats->pizza[index] > 0){
            // joystick hsa been active
            (*slices)[index].a = calculate_alpha(stats->pizza[index], largest);
        }
    }
} // colorizealpha_joygrid

static void colorizealpha_joystick(
        ss_ps3_colors *colors,
        const ss_joystick_stats *stats,
        ss_statnum largest
){

    switch (stats->type){
        case SS_GAMECONTROLLER_LEFT:
        {
            colorizealpha_joygrid(&(colors->joystick_left), &(stats->data), 
                    largest);
            break;
        }
        case SS_GAMECONTROLLER_RIGHT:
        {
            colorizealpha_joygrid(&(colors->joystick_right), &(stats->data),
                    largest);
            break;
        }
        default:
        {
            break; // we shouldnt get here
        }
    }

} // colorizealpha_joystick

static void colorizealpha_trigger(
        ss_ps3_colors *colors,
        const ss_trigger_stats *stats,
        ss_statnum largest
){

    if (stats->press_time > 0){
        // only do something if we actually have values here

        switch (stats->type){
            case SS_GAMECONTROLLER_LEFT:
            {
                colors->trigger_left.a = calculate_alpha(stats->press_time,
                        largest);
                break;
            }
            case SS_GAMECONTROLLER_RIGHT:
            {
                colors->trigger_right.a = calculate_alpha(stats->press_time,
                        largest);
                break;
            }
            default:
            {
                break; // we shouldnt get here
            }
        }
    }

} // colorizealpha_trigger

static ss_statnum getlargest(ss_statnum a, ss_statnum b){
    if (a > b){
        return a;
    }

    return b;
} // getlargest
