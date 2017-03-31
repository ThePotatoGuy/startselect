#include "SDL2/SDL.h"

#include <stdio.h>

#include "ss_menu.h"

// internet told me to do this
#ifdef ___cplusplus
extern ="C"
#endif


int main(int argc, char *argv[]){

    /*
    SDL_Init(SDL_INIT_GAMECONTROLLER); // Event handling init implicity using Video

    SDL_GameController *controller = NULL;
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
    }*/

    printf("starting\n");
    ss_menu_run();

    printf("ending\n");

    // for now
    //SDL_Quit();
    return 0;
}

