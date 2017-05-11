/**
 * Window and dialog related constants for SS
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains all Window and dialog related constants
 */

#ifndef SS_WINDOW_CONSTANTS_H
#define SS_WINDOW_CONSTANTS_H

//  DEFINES ===================================================================

//  DIALOG  -------------------------------------------------------------------

// number of image saving filters
#define SS_IMGSV_FILT_CT 1

// number of stat saving filters
#define SS_SSS_FILT_CT 1

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
extern const char *SS_DLG_SIMG_FILTER[SS_IMGSV_FILT_CT];
extern const char SS_DLG_SIMG_FILTERDESC[];
extern const char SS_DLG_SIMG_DFILENAME[];

// stats dialogs
extern const char *SS_DLG_STAT_FILTER[SS_SSS_FILT_CT];
extern const char SS_DLG_STAT_FILTERDESC[];

// save stats dialogs
extern const char SS_DLG_SSTAT_TITLE[];
extern const char SS_DLG_SSTAT_DFILENAME[];

// load stats dialogs
extern const char SS_DLG_LSTAT_TITLE[];

// color chooser dialogs
extern const char SS_DLG_CLRCHO_TITLE[];

// TODO adjustments in ui may render the following useless
// TODO these may be removed in coming updates

// start recording message box 
extern const char SS_DLG_RSTR_MB_TITLE[];
extern const char SS_DLG_RSTR_MB_MSG[];

// stop recording message box
extern const char SS_DLG_RSTP_MB_TITLE[];
extern const char SS_DLG_RSTP_MB_MSG[];

// clear data failed (recording) message box
extern const char SS_DLG_CDFR_MB_TITLE[];
extern const char SS_DLG_CDFR_MB_MSG[];

// clear data prompt message box
extern const char SS_DLG_CDP_MB_TITLE[];
extern const char SS_DLG_CDP_MB_MSG[];

// save stats failed (writing) message box
extern const char SS_DLG_SSFW_MB_TITLE[];
extern const char SS_DLG_SSFW_MB_MSG[];

// save stats failed (recording) message box
extern const char SS_DLG_SSFR_MB_TITLE[];
extern const char SS_DLG_SSFR_MB_MSG[];

// load stats failed (reading) message box
extern const char SS_DLG_LSFRD_MB_TITLE[];
extern const char SS_DLG_LSFRD_MB_MSG[];

// load stats failed (recording) message box
extern const char SS_DLG_LSFR_MB_TITLE[];
extern const char SS_DLG_LSFR_MB_MSG[];

// save image failed (writing) message box
extern const char SS_DLG_SIFW_MB_TITLE[];
extern const char SS_DLG_SIFW_MB_MSG[];

#endif
