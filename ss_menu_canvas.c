/**
 * Implementation Menu Canvas functions
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include <stdbool.h>

#include "SDL2/SDL.h"

#include "ss_menudata.h"
#include "ss_renderdata.h"
#include "ss_threaddata.h"

#include "ss_menu_canvas.h"

//  IMPLEMENTATION  ===========================================================

int ss_mcvs_clearstats(ss_menudata *menudata, bool suppress){
} // ss_mcvs_clearstats

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
 * Opens file loading sialog for stats loading IF recording state is OFF.
 * If recording state is ON, data cannot be loaded and a message will be
 * logged.
 *
 * If suppress is false, a message dialog box will also be displayed when data
 * cannot be loaded or if an error during loading occured
 *
 * The file loading dialog box will always be displayed even if suppress is
 * true
 *
 * IN:
 *  @param menudata - menu data struct
 *  @param suppress - true will suppress message dialog boxes (except file
 *      loading), false will not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_mcvs_loadstats(ss_menudata *menudata, bool suppress);

/*
 * Opens file saving dialog for image saving.
 *
 * If suppress is false, a message dialog box will be displayed if error 
 * occurs.
 *
 * The file saving dialog box will always be displayed even if suppress is true
 *
 * IN:
 *  @param menudata - menu data struct
 *  @param suppress - true will suppress message dialog boxes (except file
 *      saving), false will not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_mcvs_saveimg(ss_menudata *menudata, bool suppress);

/*
 * Opens file saving dialog for stats saving IF recording state is OFF.
 * If recording state is ON, data cannot be saved and a message will be logged.
 *
 * If suppress is false, a message dialog box will also be displayed when data
 * cannot be saved or if an error during saving occurred.
 *
 * The file saving dialog box will always be displayed even if suppress is true
 *
 * IN:
 *  @param menudata - menu data struct
 *  @param suppress - true will suppress message dialog boxes (except file
 *      saving), false will not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FALSE if not
 */
int ss_mcvs_savestats(ss_menudata *menudata, bool suppress);

/*
 * Opens color chooser dialog box and sets the color of the given menu data
 * struct
 *
 * Displays appropriate message boxes unless suppress is true
 *
 * The color choose dialog will still show even if suppress is true
 *
 * IN:
 *  @param menudata - menu data struct
 *  @param suppress - true will suppress message dialog boxes (except color
 *      chooser), false will not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FALSE if not
 */
int ss_mcvs_setcolor(ss_menudata *menudata, bool suppress);

/*
 * Sets the recording state of the given menu data struct and to the given
 * recording state
 *
 * Displays appropriate message boxes unless suppress is true
 *
 * This function ONLY sets the recording state. The actual recording thread
 * must still be started / stopped appropriately
 *
 * IN:
 *  @param menudata - menu data struct
 *  @param state - the recording state to set to
 *  @param suppress - true will suppress message dialog boxes, false will not
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 *
 */
int ss_mcvs_setrec(ss_menudata *menudata, SS_RECSTATE state, bool suppress);

/*
 * Shows the help message box
 *
 * IN:
 *  @param menudata - menu data struct
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_mcvs_showhelp(ss_menudata *menudata);

