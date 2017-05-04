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

//  DEFINES ===================================================================

#define HELPONE "Space - Start/Stop recording\ng - generate image\ns - save "
#define HELPTWO "stats (recording must be stopped)\nl - load stats (recording "
#define HELPTHR "must be stopped)\nr - clear data (recording must be stopped)"
#define HELPFOR "\nc - pick render color\nh - show this help\n\nNOTES:\n"
#define HELPFIV "Default render color is material design blue\n"

#define DHELP HELPONE HELPTWO HELPTHR HELPFOR HELPFIV

//  CONSTANTS   ===============================================================

static const char * FILTERPATTERN[1] = {"*.sss"};
static const char FILTERDESC[] = "Start Select Stat files";
static const char HELP[] = DHELP;

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
//    pthread_cond_t      *recstart_cond;
    int                 *recstart;

    // stop recording signal
    pthread_mutex_t     *recstop_mutex;
    int                 *recstop;

    // stat data
    pthread_mutex_t     *stats_mutex;
    ss_generic_controller_stats *stats;

} ss_event_data;

typedef struct{
    SDL_Renderer *renderer;

    // background
    SDL_Texture *bg_texture;
    SDL_Rect bg_dim;

    // stats
    ss_generic_controller_stats *stats;
    ss_ps3_colors *statcolors;
} render_data;

//  STATIC VARIABLES    =======================================================

// holds ptr to the background texture
//static SDL_Texture *bg_texture;

// backgrund texture dimensions
//static SDL_Rect bg_tdim;

/*  STATIC FUNCTIONS    =====================================================*/

// initalizes background texture
static int init_bgtexture(render_data *rdata);

// renders data intializat
//static int init_renderdata(render_data *rdata);

// renders the screen nicely
static int renderscreen(render_data *rdata);

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

    RECORD_STATE rstate;
    rstate = OFF;

    // event handlingloop
    while (!quit){
       
        switch (rstate){

            case ON:
            {
                // recording
                res = XInputGetState(controller_id, &state);

                if(res == ERROR_SUCCESS){
                    // connected
                    if(state.dwPacketNumber != packet_num){
                        // differences occureed
                        ss_process_input(&controller, &(state.Gamepad));

                        pthread_mutex_lock(data->stats_mutex);
                        ss_process_stats(stats, &controller);
                        pthread_mutex_unlock(data->stats_mutex);

                        packet_num = state.dwPacketNumber;
                    }
                }else{
                    printf("connection error\n");
                }

                
                // check if we should stop recording
                if (ph_getreset(data->recstop_mutex, data->recstop) == 1){
                    rstate = OFF;
                    // okay time to stop recording
                    pthread_mutex_lock(data->stats_mutex);
                    ss_stats_finishstate(stats);
                    pthread_mutex_unlock(data->stats_mutex);
                }

                break;
            }
            case OFF:
            {
                // check if main thread tells us to start again
                if (ph_getreset(data->recstart_mutex, data->recstart) == 1){
                    rstate = ON;
                }
                break;
            }
            default:
            {
                break;
            }
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
    render_data rdata;
    pthread_mutex_t end_mutex, sentstat_mutex, stats_mutex;
    pthread_mutex_t recstart_mutex, recstop_mutex;
//    pthread_cond_t  sentstat_cond;
//    pthread_cond_t  recstart_cond;
    pthread_attr_t  join;
    int end, sentstat;
    int rc;
    int recstart, recstop;
    void *status;
    const char *filename = NULL;
    const char *colorCh = NULL;
    unsigned char colorstrc[3];
    unsigned char colorstr[3];
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

    rdata.stats = &workingcopy;

    // okay attempt read here
//    ss_stats_read(&stats, "statsout");

    pthread_mutex_init(&end_mutex, NULL);
    pthread_mutex_init(&sentstat_mutex, NULL);
    pthread_mutex_init(&stats_mutex, NULL);
    pthread_mutex_init(&recstart_mutex, NULL);
    pthread_mutex_init(&recstop_mutex, NULL);
//    pthread_cond_init(&recstart_cond, NULL);
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
    //data.recstart_cond = &recstart_cond;
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

    rdata.renderer = renderer;

    if (init_bgtexture(&rdata) == SS_RETURN_FAILURE){
        // error init texture

        printf("Could not texture\n");
        quit = 1;
    }


    ss_ps3_colors statcolors;

    ss_canvas_color maincolor;
    maincolor = SS_CLR_MDBLUE;
    ss_cvsclr_get(&colorstrc, &maincolor);
//    ss_cvsclr_get(&colorstr, &maincolor);

    ss_ps3_stclr_fill(&statcolors, &maincolor, true);
    rdata.statcolors = &statcolors;

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

    if (renderscreen(&rdata) == SS_RETURN_FAILURE){
        printf("\nrenderfail\n");
        quit = 1;
    }
        

    SDL_Event event; // event handler

    RECORD_STATE state;

    state = OFF;

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

                            if (renderscreen(&rdata) == SS_RETURN_FAILURE){
                                printf("\nrenderfali\n");
                                quit = 1;
                            }

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

                                    SDL_SetWindowTitle(window, SS_WINDOWNAME);

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

                                    ph_set(&recstart_mutex, &recstart, 1);
                                    state = ON;
                                    SDL_SetWindowTitle(window, SS_WINDOWNAMER);
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
                        case SDLK_r:
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
                                    if (tinyfd_messageBox(
                                                "Clearing data",
                                                "Are you sure you want to clear data?",
                                                "okcancel",
                                                "question",
                                                0) == 1){
                                        // we are off, clear data
                                    
                                        // clear stats in thread
                                        pthread_mutex_lock(&stats_mutex);
                                        ss_stats_clear(&stats);
                                        pthread_mutex_unlock(&stats_mutex);

                                        // clear our copy of stats
                                        ss_stats_clear(&workingcopy);

                                        // clear draw colors
                                        ss_ps3_stclr_alpha(&statcolors, 0);
                                        //ss_print_stats(&workingcopy);

                                        if (renderscreen(&rdata) == SS_RETURN_FAILURE){
                                            printf("\nfailure to redne\n");
                                            quit = 1;
                                        }
                                    }

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
                        case SDLK_s:
                        {
                            switch (state){
                                case OFF:
                                {
                                    // save the stats
                                    filename = tinyfd_saveFileDialog(
                                            "Save Stats",
                                            "stats001.sss",
                                            1,
                                            FILTERPATTERN,
                                            FILTERDESC);

                                    if (filename != NULL){
                                        // person selected file, lets save
                                        pthread_mutex_lock(&stats_mutex);
                                        ss_stats_copy(&workingcopy, &stats, true);
                                        pthread_mutex_unlock(&stats_mutex);

                                        if (ss_stats_write(&workingcopy, filename)
                                                == SS_RETURN_FAILURE){
                                            tinyfd_messageBox(
                                                    "BAD",
                                                    "Save failed",
                                                    "ok",
                                                    "error",
                                                    1);
                                        }
                                    } 
                                    
                                    break;
                                }
                                case ON:
                                {
                                    tinyfd_messageBox(
                                            "HOLDON",
                                            "turn off recording before saving",
                                            "ok",
                                            "warning",
                                            1);
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                            }
                            break;
                        }
                        case SDLK_l:
                        {
                            switch (state){
                                case OFF:
                                {
                                    // load stats
                                    filename = tinyfd_openFileDialog(
                                            "Load stats",
                                            "",
                                            1,
                                            FILTERPATTERN,
                                            FILTERDESC,
                                            0);

                                    if (filename != NULL){
                                        // person selected file, lets load
                                        pthread_mutex_lock(&stats_mutex);
                                        rc = ss_stats_read(&stats, filename);
                                        pthread_mutex_unlock(&stats_mutex);

                                        if (rc == SS_RETURN_FAILURE){
                                            tinyfd_messageBox(
                                                    "BAD",
                                                    "Load failed",
                                                    "ok",
                                                    "error",
                                                    1);
                                        }

                                        // now get ready to render
                                        pthread_mutex_lock(&stats_mutex);
                                        ss_stats_copy(&workingcopy, &stats, true);
                                        pthread_mutex_unlock(&stats_mutex);

                                        // and rendder
                                        if (renderscreen(&rdata) == SS_RETURN_FAILURE){
                                            printf("\nfailed\n");
                                        }
                                    }
                                    break;   
                                }
                                case ON:
                                {
                                    tinyfd_messageBox(
                                            "HOLDON",
                                            "turn off recording before loading",
                                            "ok",
                                            "warning",
                                            1);
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                            }
                            break;
                        }
                        case SDLK_h:
                        {
                            tinyfd_messageBox(
                                    "Help",
                                    HELP,
                                    "ok",
                                    "info",
                                    1);

                            break;
                        }
                        case SDLK_c:
                        {
                            // pick color
                            
                            colorCh = tinyfd_colorChooser(
                                    "Pick color",
                                    NULL,
                                    colorstrc,
                                    colorstr);

                            if (colorCh != NULL){
                                // we selected a color
                                ss_cvsclr_set(&maincolor, &colorstr);

                                // now redo colors
                                ss_ps3_stclr_fill(&statcolors, &maincolor, true);

                                //and rerender
                                if (renderscreen(&rdata) == SS_RETURN_FAILURE){
                                    printf("\nrenderfailed\n");
                                }

                                // now setup the prev color str
                                ss_cvsclr_get(&colorstrc, &maincolor);
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

/*
    if (ss_stats_write(&stats, "statsout") == SS_RETURN_FAILURE){
        printf("failure to write\n");
    }*/

    ss_destroy_generic_controller_stats(&stats);

    pthread_mutex_destroy(&end_mutex);
    pthread_mutex_destroy(&sentstat_mutex);
    pthread_mutex_destroy(&stats_mutex);
    pthread_mutex_destroy(&recstart_mutex);
    pthread_mutex_destroy(&recstop_mutex);
//    pthread_cond_destroy(&recstart_cond);
//    pthread_cond_destroy(&sentstat_cond);
    pthread_attr_destroy(&join);

    // quit
    //SDL_GameControllerClose(controller);
    SDL_DestroyTexture(rdata.bg_texture);
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

//  STATIC IMPLEMENTATION   ===================================================

static int init_bgtexture(render_data *rdata){
    SDL_Surface *bitmapSurface = NULL;

    bitmapSurface = SDL_LoadBMP("images/tests/controllermapREV8.bmp");

    if (bitmapSurface == NULL){
        return SS_RETURN_FAILURE;
    }

    rdata->bg_texture = SDL_CreateTextureFromSurface(rdata->renderer, 
            bitmapSurface);

    if (rdata->bg_texture == NULL){
        return SS_RETURN_FAILURE;
    }

    // dimensions
    rdata->bg_dim.x = 0;
    rdata->bg_dim.y = 0;
    rdata->bg_dim.w = bitmapSurface->w;
    rdata->bg_dim.h = bitmapSurface->h;

    SDL_FreeSurface(bitmapSurface);
    return SS_RETURN_SUCCESS;
} // init_bgtexture

static int renderscreen(render_data *rdata){
    
    ss_colorizealpha(rdata->statcolors, rdata->stats);

    if (SDL_RenderClear(rdata->renderer) < 0 
            || SDL_RenderCopy(rdata->renderer, rdata->bg_texture, NULL, &(rdata->bg_dim)) < 0
            || ss_ps3_cvs_drawps3(rdata->renderer, rdata->statcolors, true)
            == SS_RETURN_FAILURE){
        return SS_RETURN_FAILURE;
    }

    // finally show the goods
    SDL_RenderPresent(rdata->renderer); 

    return SS_RETURN_SUCCESS;
} // renderscreen
