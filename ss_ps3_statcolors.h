/**
 * Colors data type for tailored for ps3 processing
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#ifndef SS_PS3_STATCOLORS_H
#define SS_PS3_STATCOLORS_H

#include <stdbool.h>

#include "ss_canvas_color.h"
#include "ss_gc_constants.h"

#include "ss_ps3_constants.h"

//  TYPES   ===================================================================

// holds the color values for a particular ps3
typedef struct{
    // buttons
    ss_canvas_color buttons[SS_BUTTON_COUNT];

    // joysticks
    ss_canvas_color joystick_left[SS_PS3_JOY_SIZE];
    ss_canvas_color joystick_right[SS_PS3_JOY_SIZE];

    // triggers
    ss_canvas_color trigger_left;
    ss_canvas_color trigger_right;
} ss_ps3_colors;

//  FUNCTIONS   ===============================================================

/*
 * Fills the given ss_ps3_colors struct with the given fill color
 *
 * IN:
 *  @param fill - the ss_canvas_color to fill with
 *  @param reset_alpha - true will set alphas to 0, false will use whatever 
 *      alpha the fill color has
 *
 * OUT:
 *  @param colors - ss_ps3_colors struct filled with the fill color
 */
void ss_ps3_stclr_fill(
        ss_ps3_colors *colors, 
        const ss_canvas_color *fill,
        bool reset_alpha
);

#endif
