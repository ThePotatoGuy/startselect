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

#endif
