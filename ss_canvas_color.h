/**
 * Canvas color type
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains the color datatype and multiple consts
 */

#ifndef SS_CANVAS_COLOR_H
#define SS_CANVAS_COLOR_H

//  TYPES   ===================================================================

// parts of a color
// 255 is max, 0 is min
typedef struct{
    unsigned char r;    // red
    unsigned char g;    // green
    unsigned char b;    // blue
    unsigned char a;    // alpha
} ss_canvas_color;

//  CONSTANTS   ===============================================================

// basic colors
extern const ss_canvas_color SS_CLR_BLACK;
extern const ss_canvas_color SS_CLR_WHITE;

// material design colors
extern const ss_canvas_color SS_CLR_MDBLUE;
extern const ss_canvas_color SS_CLR_MDGREEN;
extern const ss_canvas_color SS_CLR_MDRED;

//  FUNCTIONS   ===============================================================

/*
 * Gets the color in the given canvas color and stores it in the rgb array
 *
 * IN:
 *  @param color - the canvas color to get color from
 *
 * OUT:
 *  @param colorstr - rgb color array
 */
void ss_cvsclr_get(unsigned char (*colorstr)[3], const ss_canvas_color *color);

/*
 * Sets the color of the given canvas color to the given color values
 *
 * IN:
 *  @param colorstr - ptr to array of 3 color values (unsigned char), RGB
 *
 * OUT:
 *  @param color - color set to values in colorstr
 */
void ss_cvsclr_set(ss_canvas_color *color, const unsigned char (*colorstr)[3]);

#endif
