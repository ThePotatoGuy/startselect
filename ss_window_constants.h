/**
 * Window and dialog related constants for SS
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains all Window and dialog related constants
 */

#ifndef SS_WINDOW_CONSTANTS_H
#define SS_WINDOW_CONSTANTS_H

//  CONSTANTS   ===============================================================

//  WINDOW  -------------------------------------------------------------------

// window bounds
extern const int SS_WINDOW_HT;
extern const int SS_WINDOW_WT;

// window name
extern const char SS_WINDOWNAME[];
extern const char SS_WINDOWNAMER[];

//  DIALOG  -------------------------------------------------------------------

// help window dialog
extern const char SS_DLG_HLP_TEXT[]; 
extern const char SS_DLG_HLP_TITLE[];

// save image dialog
extern const char SS_DLG_SIMG_TITLE[];
extern const char *SS_DLG_SIMG_FILTER[];
extern const char SS_DLG_SIMG_FILTERDESC[];

// stats dialogs
extern const char *SS_DLG_STAT_FILTER[];
extern const char SS_DLG_STAT_FILTERDESC[];

// save stats dialogs
extern const char SS_DLG_SSTAT_TITLE[];

// load stats dialogs
extern const char SS_DLG_LSTAT_TITLE[];

#endif
