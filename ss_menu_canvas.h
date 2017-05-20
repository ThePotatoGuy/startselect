/**
 * Canvas menu functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains all functions related to the canvas that involve dialog boxes
 * or other administrative things
 */

#ifndef SS_MENU_CANVAS_H
#define SS_MENU_CANVAS_H

#include "SDL2/SDL.h"

#include "ss_menudata.h"

//  FUNCTIONS   ===============================================================

/*
 * Generates the controller image using the given menu data struct
 *
 * When current RECSTATE is ON, this function will apply a mutex lock to 
 * retrieve the working stats, and then generate the image accordingly.
 *
 * When current RECSTATE is OFF, this function will generate the image without
 * applying a mutex lock
 *
 * If an error occurs, the error will be logged to file and a message box will
 * appear if suppress is false
 *
 * IN:
 *  @param menudata - menu data struct 
 *  @param suppress - true will suppress error message dialog box, false will
 *      not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_mcvs_genimg(ss_menudata *menudata, bool suppress);

/*
 * Toggles the recording state of the given 
 */
int ss_mcvs_togglerec(ss_menudata *menudata);

#endif
