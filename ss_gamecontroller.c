/**
 * Implementation of Game Controller functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 */

#include "SDL2/SDL.h"

#include <stdio.h>

#include "ss_gamecontroller.h"

/*  CONSTANTS   =============================================================*/

// button descriptions
static const char BUTTON_A[]                = "A";
static const char BUTTON_B[]                = "B";
static const char BUTTON_X[]                = "X";
static const char BUTTON_Y[]                = "Y";
static const char BUTTON_BACK[]             = "Back";
static const char BUTTON_GUIDE[]            = "Guide";
static const char BUTTON_START[]            = "Start";
static const char BUTTON_LEFTSTICK[]        = "Left stick";
static const char BUTTON_RIGHTSTICK[]       = "Right stick";
static const char BUTTON_LEFTSHOULDER[]     = "Left shoulder";
static const char BUTTON_RIGHTSHOULDER[]    = "Right shoulder";
static const char BUTTON_DPAD_UP[]          = "D up";
static const char BUTTON_DPAD_DOWN[]        = "D down";
static const char BUTTON_DPAD_LEFT[]        = "D left";
static const char BUTTON_DPAD_RIGHT[]       = "D right";
static const char BUTTON_MAX[]              = "Max";
static const char BUTTON_INVALID[]          = "Invalid";

// pressed released description
static const char BUTTON_PRESSED[]          = "Pressed";
static const char BUTTON_RELEASED[]         = "Released";

// the fmt string we use
static const char BUTTON_FMTSTR[] = "Controller id: %i, Button %s: %s\n";

/*  STATIC FUNCTIONS    =====================================================*/

// returns the state of this button (pressed or released)
static const char* to_string_buttonstate(int state);

/*  IMPLEMENTATION  =========================================================*/

void ss_print_input(SDL_ControllerButtonEvent *event){

    switch (event->button){
        case SDL_CONTROLLER_BUTTON_A:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_A, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_B:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_B, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_X:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_X, 
                    to_string_buttonstate(event->state));
            break;
        }   
        case SDL_CONTROLLER_BUTTON_Y:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_Y, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_BACK:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_BACK, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_GUIDE:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_GUIDE, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_LEFTSTICK:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_LEFTSTICK, 
                    to_string_buttonstate(event->state));
            break;
        }   
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_RIGHTSTICK, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_LEFTSHOULDER, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_RIGHTSHOULDER, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_DPAD_UP, 
                    to_string_buttonstate(event->state));
            break;
        }   
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_DPAD_DOWN, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_DPAD_LEFT, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_DPAD_RIGHT, 
                    to_string_buttonstate(event->state));
            break;
        }
        case SDL_CONTROLLER_BUTTON_MAX:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_MAX, 
                    to_string_buttonstate(event->state));
            break;
        }   
        case SDL_CONTROLLER_BUTTON_START:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_START, 
                    to_string_buttonstate(event->state));
            break;
        }
        default:
        {
            printf(BUTTON_FMTSTR, event->which, BUTTON_INVALID,
                    to_string_buttonstate(event->state));
            break;
        }
    }
}

static const char* to_string_buttonstate(int state){
    switch (state){
        case SDL_PRESSED:
        {
            return BUTTON_PRESSED;
        }
        case SDL_RELEASED:
        {
            return BUTTON_RELEASED;
        }
        default:
        {
            return BUTTON_INVALID;
        }
    }
}
