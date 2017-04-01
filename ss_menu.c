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

#include "SDL2/SDL.h"

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#include "ss_constants.h"
#include "ss_gamecontroller.h"
#include "ss_parallel_helpers.h"

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

    data = (ss_event_data*)thread_data;
    quit = 0;

    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    // first lets just grab the ps3 controller manually
    SDL_GameController *controller = NULL;

    int test = 1;
    for (int i = 0; test && i < SDL_NumJoysticks(); i++){
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

    controller = SDL_GameControllerOpen(0);

    // event handlingloop
    while (!quit){
        // for now, just wait until we get signal to stop
        quit = ph_get(data->end_mutex, data->end);
    }

    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    return NULL;
}

int ss_menu_run(){

    /*
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

    pthread_t window_thread;
    pthread_t controller_thread;
    
    printf("made it here\n");
    pthread_create(&window_thread, &join, ss_open_window, (void*)&data);
    pthread_create(&controller_thread, &join, ss_open_window, (void*)&data);

    pthread_join(controller_thread, &status);
    pthread_join(window_thread, &status);

    pthread_mutex_destroy(&end_mutex);
    pthread_attr_destroy(&join);
    */

    SDL_Window *window;
    int quit;

    quit = 0;

    SDL_Init(0);
    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER); // init video so we can display a window

    window = SDL_CreateWindow("Quit",100,100,100,100,0);
   
    if (window == NULL){
        // window failed to create

        printf("Could not create window: %s\n", SDL_GetError());
        quit = 1;
    }

    // first lets just grab the ps3 controller manually
    SDL_GameController *controller = NULL;

    int test = 1;
    for (int i = 0; test && i < SDL_NumJoysticks(); i++){
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

    controller = SDL_GameControllerOpen(0);
    printf("que %i\n", SDL_GameControllerEventState(SDL_QUERY));
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
                }
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                {
                    printf("in here\n");
                    ss_print_input(&event.cbutton);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    // quit
    SDL_GameControllerClose(controller);
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


