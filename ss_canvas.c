/**
 * Implementation of the canavs
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include "SDL2/SDL.h"

#include "SDL2/SDL2_gfxPrimitives.h"

#include "ss_constants.h"
#include "ss_gamecontroller.h"
#include "ss_shape.h"

#include "ss_canvas.h"

//  CONSTANTS   ===============================================================

// 14 buttons, 2 triggers and 2 joysticks
#define PS3_SHAPE_SIZE 18

//  VARIABLES   ===============================================================

// shape data 
static ss_canvas_shape ps3_shapes[PS3_SHAPE_SIZE];

//  STATIC FUNCTIONS    =======================================================

/*
 * Setups the ps3 shapes array
 */
static void setup_ps3_shapes();

//  IMPLEMNENTATION ===========================================================

int ss_canvas_drawcircle(
        SDL_Renderer *renderer, 
        const ss_circle *circle,
        const ss_canvas_color *color,
        bool add_aa
){
    
    if (filledCircleRGBA(renderer, circle->x, circle->y, circle->r, color->r, 
                color->g, color->b, color->a) == -1){
        return SS_RETURN_FAILURE;
    }

    if (add_aa){
        // antialisising 
        
        if (aacircleRGBA(renderer, circle->x, circle->y, circle->r, color->r, 
                    color->g, color->b, color->a) == -1){
            return SS_RETURN_FAILURE;
        }
    }

    return SS_RETURN_SUCCESS;
} // ss_canvas_drawcircle

int ss_canvas_drawellip(
        SDL_Renderer *renderer,
        const ss_ellipse *ellipse,
        const ss_canvas_color *color,
        bool add_aa
){

    if (filledEllipseRGBA(renderer, ellipse->x, ellipse->y, ellipse->rx,
                ellipse->ry, color->r, color->g, color->b, color->a) == -1){
        return SS_RETURN_FAILURE;
    }

    if (add_aa){
        if (aaellipseRGBA(renderer, ellipse->x, ellipse->y, ellipse->rx,
                    ellipse->ry, color->r, color->g, color->b, 
                    color->a) == -1){
            return SS_RETURN_FAILURE;
        }
    }

    return SS_RETURN_SUCCESS;
} // ss_canvas_drawellip

int ss_canvas_drawpoly(
        SDL_Renderer *renderer,
        const Sint16 *vx,
        const Sint16 *vy,
        unsigned int vtx_ct,
        const ss_canvas_color *color,
        bool add_aa
){

    if (filledPolygonRGBA(renderer, vx, vy, vtx_ct, color->r, color->g,
                color->b, color->a) == -1){
        return SS_RETURN_FAILURE;
    }

    if (add_aa){
        // antiaailiasing

        if (aapolygonRGBA(renderer, vx, vy, vtx_ct, color->r, color->g, 
                    color->b, color->a) == -1){
            return SS_RETURN_FAILURE;
        }
    }

    return SS_RETURN_SUCCESS;
} // ss_canvas_drawpoly

int ss_canvas_drawrect(
        SDL_Renderer *renderer, 
        const SDL_Rect *rect, 
        const ss_canvas_color *color
){
    if (boxRGBA(renderer, rect->x, rect->y, rect->x + rect->w,
                rect->y + rect->h, color->r, color->g, color->b, 
                color->a) == -1){
        return SS_RETURN_FAILURE;
    }

    return SS_RETURN_SUCCESS;
} // ss_canvas_drawrect

int ss_canvas_drawslice(
        SDL_Renderer *renderer,
        const ss_slice *slice,
        const ss_canvas_color *color,
        bool add_aa
){
    if (/*ss_canvas_drawellip(renderer, &(slice->tip), color, add_aa)
            == SS_RETURN_FAILURE
            ||*/ ss_canvas_drawpoly(renderer, slice->body.vx, slice->body.vy,
                SS_TRI_SIZE, color, add_aa) == SS_RETURN_FAILURE){
        return SS_RETURN_FAILURE;
    }

    return SS_RETURN_SUCCESS;
} // ss_canavs_drawslice

int ss_canvas_init(){
    // TODO
    return SS_RETURN_SUCCESS;
} // ss_canvas_init

int ss_open_canvas(){
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bitmapTex = NULL;
    SDL_Surface *bitmapSurface = NULL;
    int posX = 1000, posY = 1000, width = 1000, height = 1000;

//    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    bitmapSurface = SDL_LoadBMP("images/ps3gamepadfront.bmp");
    bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
    SDL_FreeSurface(bitmapSurface);

    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(bitmapTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    return 0;
} // ss_open_canvas

//  STATIC IMPLEMENTATION   ===================================================

static void setup_ps3_shapes(){
//    ps3_offsets[SS_BUTTON_A].circle.xoff = 627;
} // setup_ps3_shapes
