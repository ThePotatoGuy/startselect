/**
 * Implementation of PS3 statcolors
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include <stdbool.h>
#include <stdio.h> // TODO remove

#include "ss_canvas_color.h"

#include "ss_ps3_constants.h"

#include "ss_ps3_statcolors.h"

//  FUNCTIONS   ===============================================================

void ss_ps3_stclr_fill(
        ss_ps3_colors *colors, 
        const ss_canvas_color *fill,
        bool reset_alpha
){

    ss_canvas_color actual_fill;

    actual_fill = *fill;

    if (reset_alpha){
        actual_fill.a = 0;
    }

    // buttons
    for (int index = 0; index < SS_BUTTON_COUNT; index++){
        colors->buttons[index] = actual_fill;

    }

    // joysticks
    for (int index = 0; index < SS_PS3_JOY_SIZE; index++){
        colors->joystick_left[index] = actual_fill;
        colors->joystick_right[index] = actual_fill;
    }

    // triggers
    colors->trigger_left = actual_fill;
    colors->trigger_right = actual_fill;

    printf("%i\n", colors->trigger_left.a);

} // ss_ps3_stclr_fill
