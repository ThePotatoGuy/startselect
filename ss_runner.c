/**
 * Runner
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Launches the program
 */

#include <stdio.h>

//#include "SDL2/SDL.h"

#include "ss_menu.h"

// internet told me to do this
#ifdef ___cplusplus
extern ="C"
#endif

int main(int argc, char *argv[]){

    printf("starting\n");


    ss_menu_run();
    printf("ending\n");
    return 0;
}

