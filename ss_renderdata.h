/**
 * Render Data defintiion
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Just the Render data struct definition
 */

#ifndef SS_RENDERDATA_H
#define SS_RENDERDATA_H

#include "SDL2/SDL.h"

#include "ss_stats.h"

#include "ss_ps3_statcolors.h"

//  TYPES   ===================================================================

// struct that holds data needed for rendering a picture
typedef struct{
    SDL_Renderer *renderer;

    // background
    SDL_Texture *bg_texture;
    SDL_Rect bg_dim;

    // stats
    ss_generic_controller_stats *stats;
    ss_ps3_colors *statcolors;

    // colorstrings
    unsigned char colorstrdef[SS_CLRSTR_LEN];
    unsigned char colorstrset[SS_CLRSTR_LEN];
} ss_renderdata;

#endif 
