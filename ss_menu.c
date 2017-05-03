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

#include <string.h>
#include <time.h> // testing time

#include "SDL2/SDL.h"

#include "SDL2/SDL2_gfxPrimitives.h"

#include "tinyfiledialogs.h"

#include "ss_canvas.h"
#include "ss_canvas_color.h"
#include "ss_constants.h"
#include "ss_gamecontroller.h"
#include "ss_parallel_helpers.h"
#include "ss_shape.h"
#include "ss_statprocessor.h"
#include "ss_stats.h"
#include "ss_window_constants.h"

#include "ss_ps3_canvas.h"
#include "ss_ps3_constants.h"
#include "ss_ps3_statcolors.h"

#include "ss_menu.h"

//  ENUMS   ===================================================================

// recording state
typedef enum {
    ON,
    OFF
} RECORD_STATE;

/*  TYPES   =================================================================*/

/* temporary, holds mutex/cond/condition for quitting */
typedef struct{

    /* quit signal, broadcasted from openwindow */
    pthread_mutex_t     *end_mutex;
    int                 *end;

    // stats have been sent signal
    pthread_mutex_t     *sentstat_mutex;
//    pthread_cond_t      *sentstat_cond;
    int                 *sentstat;

    // start recording signal
    pthread_mutex_t     *recstart_mutex;
    pthread_cond_t      *recstart_cond;
    int                 *recstart;

    // stop recording signal
    pthread_mutex_t     *recstop_mutex;
    int                 *recstop;

    // stat data
    pthread_mutex_t     *stats_mutex;
    ss_generic_controller_stats *stats;

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
    ss_generic_controller_stats *stats;

    int rc;
    LARGE_INTEGER pfcount, pfreq, pfend;

    data = (ss_event_data*)thread_data;

    //pthread_mutex_lock(data->stats_mutex);
    stats = data->stats;

    quit = 0;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    packet_num = 0;

    int controller_id = 0; // assuming the first controller for now
    ss_init_gamecontroller();
    
    if(ss_init_gamecontroller() == SS_RETURN_ERROR
            || ss_init_generic_controller(&controller) == SS_RETURN_ERROR
        ){

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

                pthread_mutex_lock(data->stats_mutex);
                ss_process_stats(stats, &controller);
                pthread_mutex_unlock(data->stats_mutex);

//                ph_set(data->sentstat_mutex, data->sentstat, 1);
                

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
        
        if (ph_getreset(data->recstop_mutex, data->recstop) == 1){
            // okay time to stop recording
            pthread_mutex_lock(data->stats_mutex);
            ss_stats_finishstate(stats);
            pthread_mutex_unlock(data->stats_mutex);
            
            // now wait for main thread to tell us to start again
            ph_wait(data->recstart_mutex, data->recstart_cond, data->recstart);
        }

        quit = ph_get(data->end_mutex, data->end);
    }

    ss_print_stats(stats);
    
//    pthread_mutex_unlock(data->stats_mutex);

    ss_destroy_generic_controller(&controller);
    ss_destroy_gamecontroller();

    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    return NULL;
}

int ss_menu_run(){

    ss_event_data data;
    pthread_mutex_t end_mutex, sentstat_mutex, stats_mutex;
    pthread_mutex_t recstart_mutex, recstop_mutex;
    pthread_cond_t  sentstat_cond;
    pthread_cond_t  recstart_cond;
    pthread_attr_t  join;
    int end, sentstat;
    int recstart, recstop;
    void *status;
    ss_generic_controller_stats stats;
    ss_generic_controller_stats workingcopy;


    if (ss_init_stats() == SS_RETURN_FAILURE
            || ss_init_generic_controller_stats(&stats) == SS_RETURN_FAILURE
            || ss_init_generic_controller_stats(&workingcopy) 
            == SS_RETURN_FAILURE
            || ss_ps3_cvs_init() == SS_RETURN_FAILURE
            
        ){

        printf("init fail\n");
        return SS_RETURN_FAILURE;
    }

    // okay attempt read here
    ss_stats_read(&stats, "statsout");

    pthread_mutex_init(&end_mutex, NULL);
    pthread_mutex_init(&sentstat_mutex, NULL);
    pthread_mutex_init(&stats_mutex, NULL);
    pthread_mutex_init(&recstart_mutex, NULL);
    pthread_mutex_init(&recstop_mutex, NULL);
    pthread_cond_init(&recstart_cond, NULL);
//    pthread_cond_init(&sentstat_cond, NULL);
    pthread_attr_init(&join);
    end = 0;
    sentstat = 0;
    recstart = 0;
    recstop = 0;

    status = NULL;

    data.end_mutex = &end_mutex;
    data.end = &end;
    data.sentstat_mutex = &sentstat_mutex;
//    data.sentstat_cond = &sentstat_cond;
    data.sentstat = &sentstat;
    data.stats_mutex = &stats_mutex;
    data.stats = &stats;
    data.recstart_mutex = &recstart_mutex;
    data.recstart_cond = &recstart_cond;
    data.recstart = &recstart;
    data.recstop_mutex = &recstop_mutex;
    data.recstop = &recstop;

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

    window = SDL_CreateWindow(
            SS_WINDOWNAME, 
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            SS_WINDOW_WT, 
            SS_WINDOW_HT,
            0
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
    if (window == NULL){
        // window failed to create

        printf("Could not create window: %s\n", SDL_GetError());
        quit = 1;
    }

    //bitmapSurface = SDL_LoadBMP("images/controllerdiag_temp.bmp");
//    bitmapSurface = SDL_LoadBMP("images/controllermap.bmp");

//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV.bmp");
//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV1.bmp");
//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV2.bmp");
//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV3.bmp");
//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV4.bmp");
//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV5.bmp");
//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV6.bmp");
//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV7.bmp");
    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV8.bmp");
//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV9.bmp");
//    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV10.bmp");

    //bitmapSurface = SDL_LoadBMP("images/controllermap_white.bmp");
    bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
    outtex.x = 0;
    outtex.y = 0;
    outtex.w = bitmapSurface->w;
    outtex.h = bitmapSurface->h;
    SDL_FreeSurface(bitmapSurface);
    SDL_RenderCopy(renderer, bitmapTex, NULL, &outtex);
    SDL_RenderPresent(renderer);


    ss_ps3_colors statcolors;

    ss_canvas_color maincolor;
    maincolor = SS_CLR_MDBLUE;

    ss_ps3_stclr_fill(&statcolors, &maincolor, true);

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


//    SDL_RenderPresent(renderer);
    SDL_Event event; // event handler
/*
    // event handling loop
    while (!quit){
        // only do things when we have an event
        if (SDL_PollEvent(&event)){

            switch (event.type){
                case SDL_QUIT:
                {
                    quit = 1;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
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
*/
   
 
    // TEMP TODO (needed incase of resolution mishaps
    // manuall set values
/*
    stats.buttons.press_times_ms[0]     = 221494;
    stats.buttons.press_times_ms[1]     = 9419;
    stats.buttons.press_times_ms[2]     = 44498;
    stats.buttons.press_times_ms[3]     = 996;
    stats.buttons.press_times_ms[4]     = 3739;
    stats.buttons.press_times_ms[5]     = 3312;
    stats.buttons.press_times_ms[6]     = 9806;
    stats.buttons.press_times_ms[7]     = 99;
    stats.buttons.press_times_ms[8]     = 817;
    stats.buttons.press_times_ms[9]     = 3844;
    stats.buttons.press_times_ms[10]    = 7583;
    stats.buttons.press_times_ms[11]    = 6586;
    stats.buttons.press_times_ms[12]    = 4683;
    stats.buttons.press_times_ms[13]    = 6389;
    stats.buttons.largest               = 221494;
    stats.joystick_left.data.pizza[0]   = 1515381;
    stats.joystick_left.data.pizza[7]   = 28093;
    stats.joystick_left.data.pizza[6]   = 13977;
    stats.joystick_left.data.pizza[5]   = 36687;
    stats.joystick_left.data.pizza[4]   = 774962;
    stats.joystick_left.data.pizza[3]   = 27218;
    stats.joystick_left.data.pizza[2]   = 18946;
    stats.joystick_left.data.pizza[1]   = 28694;
    stats.joystick_left.data.largest    = 1515381;
    stats.joystick_right.data.pizza[0]  = 749160;
    stats.joystick_right.data.pizza[7]  = 437;
    stats.joystick_right.data.pizza[6]  = 1126;
    stats.joystick_right.data.pizza[5]  = 890;
    stats.joystick_right.data.pizza[4]  = 3374;
    stats.joystick_right.data.pizza[3]  = 19;
    stats.joystick_right.data.pizza[2]  = 1876;
    stats.joystick_right.data.pizza[1]  = 0;
    stats.joystick_right.data.largest   = 749160;
    stats.trigger_left.press_time       = 230272;
    stats.trigger_right.press_time      = 2454525;

    ss_print_stats(&stats);
  */
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bitmapTex, NULL, &outtex);

    // ready to process
    ss_colorizealpha(&statcolors, &stats);
    if (ss_ps3_cvs_drawps3(renderer, &statcolors, true) == SS_RETURN_FAILURE){
        printf("fail to draw\n");
    }

    // and update sdl
    SDL_RenderPresent(renderer);

    quit = 0;

    RECORD_STATE state;

    state = ON;

    // event handling loop
    while (!quit){
        // only do things when we have an event
        if (SDL_PollEvent(&event)){

            switch (event.type){
                case SDL_QUIT:
                {
                    quit = 1;
                    break;
                }
                case SDL_KEYDOWN:
                {
                    // hit space, do a rerender
                    switch(event.key.keysym.sym){
                        case SDLK_g:
                        {
                            // g to generate image

                            pthread_mutex_lock(&stats_mutex);
                            ss_stats_copy(&workingcopy, &stats, true);
                            pthread_mutex_unlock(&stats_mutex);

                            ss_colorizealpha(&statcolors, &stats);

                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, bitmapTex, NULL, &outtex);

                            // ready to process
                            if (ss_ps3_cvs_drawps3(renderer, &statcolors, true) == SS_RETURN_FAILURE){
                                printf("fail to draw\n");
                            }


                            SDL_RenderPresent(renderer);
                            printf("attempted rerender\n");
                            break;
                        }
                        case SDLK_SPACE:
                        {
                            // space to start and stop recording
                            switch (state){
                                case ON:
                                {
                                    // time to turn off
                                    ph_set(&recstop_mutex, &recstop, 1);
                                    state = OFF;

                                    tinyfd_messageBox(
                                            "Okay",
                                            "Turning recording off",
                                            "ok",
                                            "info",
                                            1);

                                    break;
                                }
                                case OFF:
                                {
                                    // time to turn on

                                    tinyfd_messageBox(
                                            "Okay",
                                            "Turning recording on",
                                            "ok",
                                            "info",
                                            1);

                                    ph_signal(&recstart_mutex, 
                                            &recstart_cond, &recstart);
                                    state = ON;
                                    break;
                                }
                                default:
                                {
                                    // nothing
                                    break;
                                }
                            }
                            break;
                        }
                        case SDLK_c:
                        {
                            // clear data, but only do this if OFF
                            switch (state){
                                case ON:
                                {
                                    // we are on, dont clear, give warning
                                    tinyfd_messageBox(
                                            "Hold on",
                                            "Please stop recording before clearing data",
                                            "ok",
                                            "warning",
                                            1);
                                    break;
                                }
                                case OFF:
                                {
                                    // we are off, clear data
                                    // TODO tell other thread to clear
                                    break;
                                }
                                default:
                                {
                                    // nothing
                                    break;
                                }
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
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

   // tell other thread that we are done
    ph_set(&end_mutex, &end, 1);

    pthread_join(controller_thread, &status);


    if (ss_stats_write(&stats, "statsout") == SS_RETURN_FAILURE){
        printf("failure to write\n");
    }

    ss_destroy_generic_controller_stats(&stats);

    pthread_mutex_destroy(&end_mutex);
    pthread_mutex_destroy(&sentstat_mutex);
    pthread_mutex_destroy(&stats_mutex);
    pthread_mutex_destroy(&recstart_mutex);
    pthread_mutex_destroy(&recstop_mutex);
    pthread_cond_destroy(&recstart_cond);
//    pthread_cond_destroy(&sentstat_cond);
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


