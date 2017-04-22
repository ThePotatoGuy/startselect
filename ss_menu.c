/**
 * Menu Page 
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Implementation of Menu page:
 *
 * For now, this just creates two threads:
 * one thread handles creation of a window with quit button
 * 2nd thread actually waits for controller input and displays the information
 *
 * TODO: 2nd thread should launch a seperate file that handles input
 * TODO: make shared data construct consisting of the table that holds values
 *  of button pressesa nd stuff
 */

#include <windows.h>
#include <xinput.h>

#include <pthread.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <time.h> // testing time

#include "SDL2/SDL.h"

#include "SDL2/SDL2_gfxPrimitives.h"

#include "ss_canvas.h"
#include "ss_constants.h"
#include "ss_gamecontroller.h"
#include "ss_parallel_helpers.h"
#include "ss_shape.h"
#include "ss_stats.h"

#include "ss_ps3_canvas.h"
#include "ss_ps3_constants.h"

#include "ss_menu.h"

/*  TYPES   =================================================================*/

/* temporary, holds mutex/cond/condition for quitting */
typedef struct{

    /* quit signal, broadcasted from openwindow */
    pthread_mutex_t     *end_mutex;
    int                 *end;

} ss_event_data;

/*  STATIC FUNCTIONS    =====================================================*/



/*  IMPLEMENTATION  =========================================================*/

void* ss_event_handling(void *thread_data){
    ss_event_data *data;
    int quit;
    XINPUT_STATE state;
    DWORD res;
    DWORD packet_num;
    ss_generic_controller controller;
    ss_generic_controller_stats stats;

    int rc;
    LARGE_INTEGER pfcount, pfreq, pfend;

    data = (ss_event_data*)thread_data;
    quit = 0;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    packet_num = 0;

    int controller_id = 0; // assuming the first controller for now
    ss_init_gamecontroller();
    ss_init_stats();
    
    if(ss_init_gamecontroller() == SS_RETURN_ERROR
            || ss_init_generic_controller(&controller) == SS_RETURN_ERROR
            || ss_init_generic_controller_stats(&stats) == SS_RETURN_ERROR){

        printf("failureeeee\n");
        return NULL;
    }

    long testtime = 0;
    QueryPerformanceFrequency(&pfreq);
    QueryPerformanceCounter(&pfcount);
    // event handlingloop
    while (!quit){
        
        res = XInputGetState(controller_id, &state);

        if(res == ERROR_SUCCESS){
            // connected
            if(state.dwPacketNumber != packet_num){
                // differences occureed
                ss_process_input(&controller, &(state.Gamepad));
                ss_process_stats(&stats, &controller);
//                ss_print_generic_controller(&controller);
      //          QueryPerformanceCounter(&pfend);
    //            testtime = (1000 * (pfend.QuadPart - pfcount.QuadPart)) / 
//                    pfreq.QuadPart;
//                if(testtime > 50){
//                    printf("diff %f\n",(float)testtime);
//                }
//                pfcount = pfend;

//                printf("change here\n");

                packet_num = state.dwPacketNumber;
            }
        }else{
            printf("connection error\n");
        }
        

        quit = ph_get(data->end_mutex, data->end);
    }

    ss_print_stats(&stats);

    ss_destroy_generic_controller_stats(&stats);
    ss_destroy_generic_controller(&controller);
    ss_destroy_gamecontroller();

    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    return NULL;
}

int ss_menu_run(){

    ss_event_data data;
    pthread_mutex_t end_mutex;
    pthread_attr_t  join;
    int end;
    void *status;

    pthread_mutex_init(&end_mutex, NULL);
    pthread_attr_init(&join);
    end = 0;
    status = NULL;

    data.end_mutex = &end_mutex;
    data.end = &end;
    pthread_attr_setdetachstate(&join, PTHREAD_CREATE_JOINABLE);

    pthread_t controller_thread;
    
    pthread_create(&controller_thread, &join, ss_event_handling, (void*)&data);

    //ph_set(&end_mutex, &end, 1);
    //pthread_join(controller_thread, &status);
    //return 0;
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *bitmapTex = NULL;
    SDL_Surface *bitmapSurface = NULL;
    SDL_Rect outtex;
    int quit;

    quit = 0;

    SDL_Init(0);
    SDL_InitSubSystem(SDL_INIT_VIDEO); // init video so we can display a window

    window = SDL_CreateWindow("Quit",100,100,800,800,0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
    if (window == NULL){
        // window failed to create

        printf("Could not create window: %s\n", SDL_GetError());
        quit = 1;
    }

    bitmapSurface = SDL_LoadBMP("images/controllerdiag_temp.bmp");
    bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
    outtex.x = 0;
    outtex.y = 0;
    outtex.w = bitmapSurface->w;
    outtex.h = bitmapSurface->h;
    SDL_FreeSurface(bitmapSurface);
    SDL_RenderCopy(renderer, bitmapTex, NULL, &outtex);

    ss_canvas_color tcolor;
    tcolor.r = 13;
    tcolor.g = 59;
    tcolor.b = 95;
    tcolor.a = 255;

    ss_circle X;
    X.x = SS_PS3_A_X;
    X.y = SS_PS3_A_Y;
    X.r = SS_PS3_CB_R;

    ss_circle Triag;
    Triag.x = SS_PS3_Y_X;
    Triag.y = SS_PS3_Y_Y;
    Triag.r = 25;

    ss_circle square;
    square.x = 562;
    square.y = 433;
    square.r = 25;

    ss_circle circ;
    circ.x = 692;
    circ.y = 433;
    circ.r = 25;

    ss_ps3_dpad dright;
    dright.vx[0] = 190;
    dright.vy[0] = 433;
    dright.vx[1] = 208;
    dright.vy[1] = 414;
    dright.vx[2] = 233;
    dright.vy[2] = 414;
    dright.vx[3] = 233;
    dright.vy[3] = 452;
    dright.vx[4] = 208;
    dright.vy[4] = 452;

    ss_ps3_dpad dleft;
    dleft.vx[0] = 160;
    dleft.vy[0] = 433;
    dleft.vx[1] = 142;
    dleft.vy[1] = 452;
    dleft.vx[2] = 117;
    dleft.vy[2] = 452;
    dleft.vx[3] = 117;
    dleft.vy[3] = 414;
    dleft.vx[4] = 142;
    dleft.vy[4] = 414;

    ss_ps3_dpad dup;
    dup.vx[0] = 175;
    dup.vy[0] = 418;
    dup.vx[1] = 156;
    dup.vy[1] = 400;
    dup.vx[2] = 156;
    dup.vy[2] = 375;
    dup.vx[3] = 194;
    dup.vy[3] = 375;
    dup.vx[4] = 194;
    dup.vy[4] = 400;

    ss_ps3_dpad ddown;
    ddown.vx[0] = 175;
    ddown.vy[0] = 448;
    ddown.vx[1] = 194;
    ddown.vy[1] = 466;
    ddown.vx[2] = 194;
    ddown.vy[2] = 491;
    ddown.vx[3] = 156;
    ddown.vy[3] = 491;
    ddown.vx[4] = 156;
    ddown.vy[4] = 466;

    SDL_Rect select;
    select.x = 317;
    select.y = 424;
    select.w = 32;
    select.h = 18;

    ss_triangle start;
    start.vx[0] = 489;
    start.vy[0] = 433;
    start.vx[1] = 453;
    start.vy[1] = 443;
    start.vx[2] = 453;
    start.vy[2] = 423;

    SDL_Rect lshld;
    lshld.x = 128;
    lshld.y = 129;
    lshld.w = 94;
    lshld.h = 48;

    SDL_Rect rshld;
    rshld.x = 580;
    rshld.y = 129;
    rshld.w = 94;
    rshld.h = 48;

    ss_ps3_trigger ltrig;
    ltrig.body.x = 128;
    ltrig.body.y = 31;
    ltrig.body.w = 94;
    ltrig.body.h = 60;
    ltrig.tip.x = 175;
    ltrig.tip.y = 31;
    ltrig.tip.rx = 47;
    ltrig.tip.ry = 16;

    ss_ps3_trigger rtrig;
    rtrig.body.x = 580;
    rtrig.body.y = 31;
    rtrig.body.w = 94;
    rtrig.body.h = 60;
    rtrig.tip.x = 627;
    rtrig.tip.y = 31;
    rtrig.tip.rx = 47;
    rtrig.tip.ry = 16;

    ss_circle ljoybut;
    ljoybut.x = 516;
    ljoybut.y = 542;
    ljoybut.r = 77;

    // joysizes x = 516
    // y = 542
    // r = 77

    ss_canvas_drawcircle(renderer, &X, &tcolor, true);
    ss_canvas_drawcircle(renderer, &Triag, &tcolor, true);
    ss_canvas_drawcircle(renderer, &square, &tcolor, true);
    ss_canvas_drawcircle(renderer, &circ, &tcolor, true);

    ss_ps3_cvs_drawdpad(renderer, &dright, &tcolor, true);
    ss_ps3_cvs_drawdpad(renderer, &ddown, &tcolor, true);
    ss_ps3_cvs_drawdpad(renderer, &dleft, &tcolor, true);
    ss_ps3_cvs_drawdpad(renderer, &dup, &tcolor, true);

    ss_canvas_drawrect(renderer, &select, &tcolor);
    ss_canvas_drawpoly(renderer, start.vx, start.vy, SS_TRI_SIZE, &tcolor,
            true);

    ss_canvas_drawrect(renderer, &lshld, &tcolor);
    ss_canvas_drawrect(renderer, &rshld, &tcolor);

    ss_ps3_cvs_drawtrig(renderer, &ltrig, &tcolor, true);
    ss_ps3_cvs_drawtrig(renderer, &rtrig, &tcolor, true);

    ss_canvas_drawcircle(renderer, &ljoybut, &tcolor, true);
//    filledCircleRGBA(renderer, 627, 492, 25, 13, 59, 95, 255);
//    aacircleRGBA(renderer, 627, 492, 25, 13, 59, 95, 255);

//    circleRGBA(renderer, 500, 492, 25, 13, 59, 95, 255);
//    pieRGBA(renderer, 200, 200, 25, -22, 22, 13, 59, 95, 255);
//    pieRGBA(renderer, 100, 200, 25, 0, 270, 13, 59, 95, 255);
//    filledPieRGBA(renderer, 300, 300, 100, 0, 90, 13, 59, 95, 255);
//    filledCircleRGBA(renderer, 300, 300, 50, 255, 255, 255, 255);
//    aacircleRGBA(renderer, 300, 300, 50, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);

/*
    // first lets just grab the ps3 controller manually
    SDL_GameController *controller = NULL;

    int test = 1;
    for (int i = 0; i < SDL_NumJoysticks(); i++){
        if (SDL_IsGameController(i)){
            controller = SDL_GameControllerOpen(i);
            if (controller){
                printf("Controller id: %i\n controler name: %s\n", i,
                        SDL_GameControllerName(controller));
            }else{
                printf("Could not open gamecontroller %i: %s\n",
                        i, SDL_GetError());
            }
        }
    }

//    controller = SDL_GameControllerOpen(0);
    printf("que %i\n", SDL_GameControllerEventState(SDL_QUERY));
    */
    SDL_Event event; // event handler

    // event handling loop
    while (!quit){
        // only do things when we have an event
        while (SDL_PollEvent(&event)){

            switch (event.type){
                case SDL_QUIT:
                {
                    quit = 1;
                    break;
                }
                case SDL_KEYDOWN:
                {
                    // spacebar stuff
                    switch (event.key.keysym.sym){
                        case SDLK_SPACE:
                        {
                            quit = 1;
                            break;
                        }
                        default: break;
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    /*
                     * circle button radius 25
                     * A: (627,492)
                     * X: (562, 434)
                     */
                    printf("(%i, %i)\n", event.button.x, event.button.y);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    
    ph_set(&end_mutex, &end, 1);

    pthread_join(controller_thread, &status);

    pthread_mutex_destroy(&end_mutex);
    pthread_attr_destroy(&join);

    // quit
    //SDL_GameControllerClose(controller);
    SDL_DestroyTexture(bitmapTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return SS_RETURN_SUCCESS;
}

void* ss_open_window(void *thread_data){
    SDL_Window *window;
    ss_event_data *data;
    int quit;

    data = (ss_event_data*)thread_data;
    quit = 0;

    SDL_InitSubSystem(SDL_INIT_VIDEO); // init video so we can display a window

    window = SDL_CreateWindow("Quit",100,100,100,100,0);
   
    if (window == NULL){
        // window failed to create

        printf("Could not create window: %s\n", SDL_GetError());
        quit = 1;
    }

    SDL_Event event; // event handler

    // event handling loop
    while (!quit){
        // only do things when we have an event
        while (SDL_PollEvent(&event)){

            // user requets quit
            if (event.type == SDL_QUIT){
                quit = 1;
            }else if (event.type == SDL_KEYDOWN){

                // spacebar stuff
                switch (event.key.keysym.sym){
                    case SDLK_KP_SPACE:
                    {
                        quit = 1;
                        break;
                    }
                    default: break;
                }
            }
        }
    }

    // quit
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    ph_set(data->end_mutex, data->end, 1);
    return NULL;
}


