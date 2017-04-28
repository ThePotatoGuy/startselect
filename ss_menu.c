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

#include "ss_canvas.h"
#include "ss_canvas_color.h"
#include "ss_constants.h"
#include "ss_gamecontroller.h"
#include "ss_parallel_helpers.h"
#include "ss_shape.h"
#include "ss_statprocessor.h"
#include "ss_stats.h"

#include "ss_ps3_canvas.h"
#include "ss_ps3_constants.h"
#include "ss_ps3_statcolors.h"

#include "ss_menu.h"

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

    pthread_mutex_lock(data->stats_mutex);
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

//                pthread_mutex_lock(data->stats_mutex);
                ss_process_stats(stats, &controller);
//                pthread_mutex_unlock(data->stats_mutex);

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
        

        quit = ph_get(data->end_mutex, data->end);
    }

    ss_print_stats(stats);
    
    pthread_mutex_unlock(data->stats_mutex);

    ss_destroy_generic_controller(&controller);
    ss_destroy_gamecontroller();

    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    return NULL;
}

int ss_menu_run(){

    ss_event_data data;
    pthread_mutex_t end_mutex, sentstat_mutex, stats_mutex;
    pthread_cond_t  sentstat_cond;
    pthread_attr_t  join;
    int end, sentstat;
    void *status;
    ss_generic_controller_stats stats;


    if (ss_init_stats() == SS_RETURN_FAILURE
            || ss_init_generic_controller_stats(&stats) == SS_RETURN_FAILURE
            || ss_ps3_cvs_init() == SS_RETURN_FAILURE
            
        ){

        printf("init fail\n");
        return SS_RETURN_FAILURE;
    }

    pthread_mutex_init(&end_mutex, NULL);
    pthread_mutex_init(&sentstat_mutex, NULL);
    pthread_mutex_init(&stats_mutex, NULL);
//    pthread_cond_init(&sentstat_cond, NULL);
    pthread_attr_init(&join);
    end = 0;
    sentstat = 0;

    status = NULL;

    data.end_mutex = &end_mutex;
    data.end = &end;
    data.sentstat_mutex = &sentstat_mutex;
//    data.sentstat_cond = &sentstat_cond;
    data.sentstat = &sentstat;
    data.stats_mutex = &stats_mutex;
    data.stats = &stats;

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

    window = SDL_CreateWindow("Quit",100,100,814,814,0);
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
    ss_canvas_color tcolor;
    tcolor = SS_CLR_MDBLUE;

    ss_canvas_color gcolor;
    gcolor = SS_CLR_MDGREEN;

    ss_canvas_color rcolor;
    rcolor = SS_CLR_MDRED;

    ss_canvas_color alpha;
    alpha = tcolor;

    ss_canvas_color colors[3];
    colors[0] = tcolor;
    colors[1] = gcolor;
    colors[2] = rcolor;

    ss_circle X;
    X.x = SS_PS3_A_X;
    X.y = SS_PS3_A_Y;
    X.r = SS_PS3_A_R;

    ss_circle Triag;
    Triag.x = SS_PS3_Y_X;
    Triag.y = SS_PS3_Y_Y;
    Triag.r = SS_PS3_Y_R;

    ss_circle square;
    square.x = SS_PS3_X_X;
    square.y = SS_PS3_X_Y;
    square.r = SS_PS3_X_R;

    ss_circle circ;
    circ.x = SS_PS3_B_X;
    circ.y = SS_PS3_B_Y;
    circ.r = SS_PS3_B_R;

    ss_ps3_dpad dright;
    ss_ps3_dpad dleft;
    ss_ps3_dpad dup;
    ss_ps3_dpad ddown;
    for (int i = 0; i < 5; i++){
        dright.vx[i] = SS_PS3_RDP_VX[i];
        dright.vy[i] = SS_PS3_RDP_VY[i];
        dleft.vx[i] = SS_PS3_LDP_VX[i];
        dleft.vy[i] = SS_PS3_LDP_VY[i];
        dup.vx[i] = SS_PS3_UDP_VX[i];
        dup.vy[i] = SS_PS3_UDP_VY[i];
        ddown.vx[i] = SS_PS3_DDP_VX[i];
        ddown.vy[i] = SS_PS3_DDP_VY[i];
    }

    SDL_Rect select;
    select.x = SS_PS3_BCK_X;
    select.y = SS_PS3_BCK_Y;
    select.w = SS_PS3_BCK_W;
    select.h = SS_PS3_BCK_H;

    ss_triangle start;
    for (int i = 0; i < 3; i ++){
        start.vx[i] = SS_PS3_STRT_VX[i];
        start.vy[i] = SS_PS3_STRT_VY[i];
    }

    SDL_Rect lshld;
    lshld.x = SS_PS3_LSH_X;
    lshld.y = SS_PS3_LSH_Y;
    lshld.w = SS_PS3_LSH_W;
    lshld.h = SS_PS3_LSH_H;

    SDL_Rect rshld;
    rshld.x = SS_PS3_RSH_X;
    rshld.y = SS_PS3_RSH_Y;
    rshld.w = SS_PS3_RSH_W;
    rshld.h = SS_PS3_RSH_H;

    ss_ps3_trigger ltrig;
    ltrig.body.x    = SS_PS3_LTR_B_X;
    ltrig.body.y    = SS_PS3_LTR_B_Y;
    ltrig.body.w    = SS_PS3_LTR_B_W;
    ltrig.body.h    = SS_PS3_LTR_B_H;
    ltrig.tip.x     = SS_PS3_LTR_T_X;
    ltrig.tip.y     = SS_PS3_LTR_T_Y;
    ltrig.tip.rx    = SS_PS3_LTR_T_RX;
    ltrig.tip.ry    = SS_PS3_LTR_T_RY;

    ss_ps3_trigger rtrig;
    rtrig.body.x    = SS_PS3_RTR_B_X;
    rtrig.body.y    = SS_PS3_RTR_B_Y;
    rtrig.body.w    = SS_PS3_RTR_B_W;
    rtrig.body.h    = SS_PS3_RTR_B_H;
    rtrig.tip.x     = SS_PS3_RTR_T_X;
    rtrig.tip.y     = SS_PS3_RTR_T_Y;
    rtrig.tip.rx    = SS_PS3_RTR_T_RX;
    rtrig.tip.ry    = SS_PS3_RTR_T_RY;

    ss_circle rjoybut;
    rjoybut.x = SS_PS3_LTHM_X;
    rjoybut.y = SS_PS3_LTHM_Y;
    rjoybut.r = SS_PS3_LTHM_R;

    ss_circle ljoybut;
    ljoybut.x = SS_PS3_RTHM_X;
    ljoybut.y = SS_PS3_RTHM_Y;
    ljoybut.r = SS_PS3_RTHM_R;
    // raduis 77

    ss_ps3_joystick rjoy;
    ss_ps3_joystick ljoy;

    for (int i = 0; i < 8; i++){
//        for (int q = 0; q < 3; q++){
            memcpy(rjoy.slices[i].vx, SS_PS3_RJY_SLS_VX[i], 3*sizeof(Sint16));
            memcpy(rjoy.slices[i].vy, SS_PS3_RJY_SLS_VY[i], 3*sizeof(Sint16));
            memcpy(ljoy.slices[i].vx, SS_PS3_LJY_SLS_VX[i], 3*sizeof(Sint16));
            memcpy(ljoy.slices[i].vy, SS_PS3_LJY_SLS_VY[i], 3*sizeof(Sint16));
//        }
    }
    */

/*
    // 0
    ss_slice sltest;
    sltest.tip.x = 587;
    sltest.tip.y = 542;
    sltest.tip.rx = 6;
    sltest.tip.ry = 29;
    sltest.body.vx[0] = 516;
    sltest.body.vy[0] = 542;
    sltest.body.vx[1] = 587;
    sltest.body.vy[1] = 513;
    sltest.body.vx[2] = 587;
    sltest.body.vy[2] = 571;

    ss_ps3_joystick rjoy;
    rjoy.slices[0] = sltest;

    // 1
    rjoy.slices[1].tip.x = 566;
    rjoy.slices[1].tip.y = 592;
    rjoy.slices[1].tip.rx = 6;
    rjoy.slices[1].tip.ry = 29;
    rjoy.slices[1].body.vx[0] = 516;
    rjoy.slices[1].body.vy[0] = 542;
    rjoy.slices[1].body.vx[1] = 587;
    rjoy.slices[1].body.vy[1] = 571;
    rjoy.slices[1].body.vx[2] = 545;
    rjoy.slices[1].body.vy[2] = 613;

    // 2
    rjoy.slices[2].tip.x = 516;
    rjoy.slices[2].tip.y = 613;
    rjoy.slices[2].tip.rx = 29;
    rjoy.slices[2].tip.ry = 6;
    rjoy.slices[2].body.vx[0] = 516;
    rjoy.slices[2].body.vy[0] = 542;
    rjoy.slices[2].body.vx[1] = 545;
    rjoy.slices[2].body.vy[1] = 613;
    rjoy.slices[2].body.vx[2] = 487;
    rjoy.slices[2].body.vy[2] = 613;

    // 3
    rjoy.slices[3].tip.x = 466;
    rjoy.slices[3].tip.y = 592;
    rjoy.slices[3].tip.rx = 6;
    rjoy.slices[3].tip.ry = 29;
    rjoy.slices[3].body.vx[0] = 516;
    rjoy.slices[3].body.vy[0] = 542;
    rjoy.slices[3].body.vx[1] = 487;
    rjoy.slices[3].body.vy[1] = 613;
    rjoy.slices[3].body.vx[2] = 445;
    rjoy.slices[3].body.vy[2] = 571;

    // 4
    rjoy.slices[4].tip.x = 445;
    rjoy.slices[4].tip.y = 542;
    rjoy.slices[4].tip.rx = 6;
    rjoy.slices[4].tip.ry = 29;
    rjoy.slices[4].body.vx[0] = 516;
    rjoy.slices[4].body.vy[0] = 542;
    rjoy.slices[4].body.vx[1] = 445;
    rjoy.slices[4].body.vy[1] = 571;
    rjoy.slices[4].body.vx[2] = 445;
    rjoy.slices[4].body.vy[2] = 513;

    // 5
    rjoy.slices[5].tip.x = 466;
    rjoy.slices[5].tip.y = 492;
    rjoy.slices[5].tip.rx = 6;
    rjoy.slices[5].tip.ry = 29;
    rjoy.slices[5].body.vx[0] = 516;
    rjoy.slices[5].body.vy[0] = 542;
    rjoy.slices[5].body.vx[1] = 445;
    rjoy.slices[5].body.vy[1] = 513;
    rjoy.slices[5].body.vx[2] = 487;
    rjoy.slices[5].body.vy[2] = 471;

    // 6
    rjoy.slices[6].tip.x = 516;
    rjoy.slices[6].tip.y = 471;
    rjoy.slices[6].tip.rx = 29;
    rjoy.slices[6].tip.ry = 6;
    rjoy.slices[6].body.vx[0] = 516;
    rjoy.slices[6].body.vy[0] = 542;
    rjoy.slices[6].body.vx[1] = 487;
    rjoy.slices[6].body.vy[1] = 471;
    rjoy.slices[6].body.vx[2] = 545;
    rjoy.slices[6].body.vy[2] = 471;

    // 7
    rjoy.slices[7].tip.x = 566;
    rjoy.slices[7].tip.y = 492;
    rjoy.slices[7].tip.rx = 6;
    rjoy.slices[7].tip.ry = 29;
    rjoy.slices[7].body.vx[0] = 516;
    rjoy.slices[7].body.vy[0] = 542;
    rjoy.slices[7].body.vx[1] = 545;
    rjoy.slices[7].body.vy[1] = 471;
    rjoy.slices[7].body.vx[2] = 587;
    rjoy.slices[7].body.vy[2] = 513;
*/
    // joysizes x = 516
    // y = 542
    // r = 77
/*
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

    alpha.a = 45;
    for (int i = 0; i < 8; i++){
        ss_canvas_drawtri(renderer, &(rjoy.slices[i]), &alpha, true);
        ss_canvas_drawtri(renderer, &(ljoy.slices[i]), &alpha, true);
        alpha.a = (alpha.a + 45) % 255;
    }


	ss_canvas_drawcircle(renderer, &(ljoybut), &SS_CLR_BLACK, true);
    ss_canvas_drawcircle(renderer, &(rjoybut), &SS_CLR_BLACK, true);

    alpha.a = 213;
    ss_canvas_drawcircle(renderer, &(ljoybut), &alpha, true);
    alpha.a = 189;
    ss_canvas_drawcircle(renderer, &(rjoybut), &alpha, true);
*/

//    ss_canvas_drawcircle(renderer, &ljoybut, &tcolor, true);

//    ss_canvas_drawslice(renderer, &sltest, &tcolor, true);

//    ss_ps3_cvs_drawjoy(renderer, &rjoy, &tcolor, true, false, NULL, 0);

//    ss_ps3_cvs_drawjoy(renderer, &rjoy, NULL, true, true, &colors, 3);


    /*
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, -23, 23, tcolor.r,
            tcolor.g, tcolor.b, tcolor.a);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 23, 67, tcolor.r,
            tcolor.g, tcolor.b, 180);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 67, 113, rcolor.r,
            rcolor.g, rcolor.b, rcolor.a);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 113, 157, tcolor.r,
            tcolor.g, tcolor.b, tcolor.a);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 157, 203, gcolor.r,
            gcolor.g, gcolor.b, gcolor.a);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 203, 247, rcolor.r,
            rcolor.g, rcolor.b, rcolor.a);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 247, 293, tcolor.r,
            tcolor.g, tcolor.b, tcolor.a);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 293, 337, gcolor.r,
            gcolor.g, gcolor.b, gcolor.a);
*/

    /*
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, -23, 23, tcolor.r,
            tcolor.g, tcolor.b, 160);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 23, 67, tcolor.r,
            tcolor.g, tcolor.b, 180);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 67, 113, tcolor.r,
            tcolor.g, tcolor.b, 127);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 113, 157, tcolor.r,
            tcolor.g, tcolor.b, 15);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 157, 203, tcolor.r,
            tcolor.g, tcolor.b, 70);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 203, 247, tcolor.r,
            tcolor.g, tcolor.b, 90);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 247, 293, tcolor.r,
            tcolor.g, tcolor.b, 210);
    filledPieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 293, 337, tcolor.r,
            tcolor.g, tcolor.b, 189);

    filledPieRGBA(renderer, rjoybut.x, rjoybut.y, rjoybut.r, -23, 23, tcolor.r,
            tcolor.g, tcolor.b, 160);
    filledPieRGBA(renderer, rjoybut.x, rjoybut.y, rjoybut.r, 23, 67, tcolor.r,
            tcolor.g, tcolor.b, 180);
    filledPieRGBA(renderer, rjoybut.x, rjoybut.y, rjoybut.r, 67, 113, tcolor.r,
            tcolor.g, tcolor.b, 127);
    filledPieRGBA(renderer, rjoybut.x, rjoybut.y, rjoybut.r, 113, 157, tcolor.r,
            tcolor.g, tcolor.b, 15);
    filledPieRGBA(renderer, rjoybut.x, rjoybut.y, rjoybut.r, 157, 203, tcolor.r,
            tcolor.g, tcolor.b, 70);
    filledPieRGBA(renderer, rjoybut.x, rjoybut.y, rjoybut.r, 203, 247, tcolor.r,
            tcolor.g, tcolor.b, 90);
    filledPieRGBA(renderer, rjoybut.x, rjoybut.y, rjoybut.r, 247, 293, tcolor.r,
            tcolor.g, tcolor.b, 210);
    filledPieRGBA(renderer, rjoybut.x, rjoybut.y, rjoybut.r, 293, 337, tcolor.r,
            tcolor.g, tcolor.b, 189);
*/

//    pieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, -22, 22, 0, 0, 0, 255);
//    pieRGBA(renderer, ljoybut.x, ljoybut.y, ljoybut.r, 22, 67, 0, 0, 0, 255);


//    filledCircleRGBA(renderer, 627, 492, 25, 13, 59, 95, 255);
//    aacircleRGBA(renderer, 627, 492, 25, 13, 59, 95, 255);

//    circleRGBA(renderer, 500, 492, 25, 13, 59, 95, 255);
//    pieRGBA(renderer, 200, 200, 25, -22, 22, 13, 59, 95, 255);
//    pieRGBA(renderer, 100, 200, 25, 0, 270, 13, 59, 95, 255);
//    filledPieRGBA(renderer, 300, 300, 100, 0, 90, 13, 59, 95, 255);
//    filledCircleRGBA(renderer, 300, 300, 50, 255, 255, 255, 255);
//    aacircleRGBA(renderer, 300, 300, 50, 255, 255, 255, 255);

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
/*
        // retreive sentstat value
        if (ph_getreset(&sentstat_mutex, &sentstat) == 1){
            // lock the stats, process colors, then unlock and draw
            pthread_mutex_lock(&stats_mutex);

            // now its safe to process stats
            ss_colorizealpha(&statcolors, &stats);

            pthread_mutex_unlock(&stats_mutex);

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, bitmapTex, NULL, &outtex);
            // and draw
            if (ss_ps3_cvs_drawps3(renderer, &statcolors, true) == SS_RETURN_FAILURE){
                printf("fail to draw\n");
            
            }

            // and update sdl
            SDL_RenderPresent(renderer);
        }*/

    }

   
    // tell other thread that we are done
    ph_set(&end_mutex, &end, 1);

    pthread_join(controller_thread, &status);

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
                        case SDLK_SPACE:
                        {
                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, bitmapTex, NULL, &outtex);

                            // ready to process
                            ss_colorizealpha(&statcolors, &stats);
                            if (ss_ps3_cvs_drawps3(renderer, &statcolors, true) == SS_RETURN_FAILURE){
                                printf("fail to draw\n");
                            }


                            SDL_RenderPresent(renderer);
                            printf("attempted rerender\n");
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

    ss_destroy_generic_controller_stats(&stats);

    pthread_mutex_destroy(&end_mutex);
    pthread_mutex_destroy(&sentstat_mutex);
    pthread_mutex_destroy(&stats_mutex);
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


