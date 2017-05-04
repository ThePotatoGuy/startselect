/**
 * Implementation of ss_canvas_color
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include "ss_canvas_color.h"

//  CONSTANTS   ===============================================================

// basic colors
const ss_canvas_color SS_CLR_BLACK  = {0, 0, 0, 255};
const ss_canvas_color SS_CLR_WHITE  = {255, 255, 255, 255};

// material design colors
const ss_canvas_color SS_CLR_MDBLUE     = {33, 150, 243, 255};
const ss_canvas_color SS_CLR_MDGREEN    = {76, 175, 80, 255};
const ss_canvas_color SS_CLR_MDRED      = {244, 67, 54, 255};

//  IMPLEMENTATION  ===========================================================

void ss_cvsclr_get(unsigned char (*colorstr)[3], const ss_canvas_color *color){
    (*colorstr)[0] = color->r;
    (*colorstr)[1] = color->g;
    (*colorstr)[2] = color->b;
} // ss_cvsclr_get

void ss_cvsclr_set(ss_canvas_color *color, const unsigned char (*colorstr)[3]){
    color->r = (*colorstr)[0];
    color->g = (*colorstr)[1];
    color->b = (*colorstr)[2];
} // ss_cvsclr_set
