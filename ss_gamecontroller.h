/**
 * Game Controller Functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains functions related to game controller input
 * This includes functions for printing values input.
 * TODO: include functions for handling data
 */

#ifndef SS_GAMECONTROLLER_H
#define SS_GAMECONTROLLER_H

#include "SDL2/SDL.h"

/*
 * Prints a message describing what type of input the given value is (in 
 * the context of a game controller)
 *
 * Uses values from SDL_GameControllerButton to determine button type
 * This also prints out the state (pressed or released)
 *
 * IN:
 *  @param event - SDL_ControllerButtonEvent struct
 */
void ss_print_input(SDL_ControllerButtonEvent *event);

#endif
