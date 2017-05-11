/**
 * Implementation of Window constants
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include "ss_window_constants.h"

//  DEFINES ===================================================================

//  WINDOW  -------------------------------------------------------------------

// window bounds
#define WINDOW_HT 814
#define WINDOW_WT 814

//  DIALOG  -------------------------------------------------------------------

// general titles
#define TITLE_ERR "Error"
#define TITLE_FAIL "Failure"
#define TITLE_SUCC "Success"

// help
#define HELPSPACE "Space - Start/Stop recording\n"
#define HELPG "g - generate image\n"
#define HELPI "i - save image to file\n"
#define HELPC "c - pick base render color\n"
#define HELPS "s - save stat data (recording must be stopped)\n"
#define HELPL "l - load stat data (recording must be stopped)\n"
#define HELPR "r - clear stat data (recording must be stopped)\n"
#define HELPH "h - show this help window\n"
#define HELPNOTES "\nNOTES:\n"
#define HELPNONE "Default render color is material design blue\n"

#define HELPONE HELPSPACE HELPG HELPI HELPC HELPS HELPL HELPR HELPH
#define HELPADD HELPNOTES HELPNONE

#define HELP HELPONE HELPADD

// file 
#define WILDCARD "*"
#define DOT "."

// save image filter
#define IMGEXT "bmp"
#define IMGSAVEFILTER WILDCARD DOT IMGEXT

// save image default filename
#define DEFIMG "screen001"
#define DEFIMGFILE DEFIMG DOT IMGEXT

// stat file filter
#define STATEXT "sss"
#define STATSAVEFILTER WILDCARD DOT STATEXT

// save stats default filename
#define DEFSTAT "stats001"
#define DEFSTATFILE DEFSTAT DOT STATEXT

// TODO message box stuff

// clear data failure
#define CLEARDATA_FAILMSG "Please stop recording before clearing stat data."

// clear data prompt
#define CLEARDATA_PROMPT_ONE "Are you sure you want to clear the current stats"
#define CLEARDATA_PROMPT_TWO " data?"
#define CLEARDATA_PROMPT CLEARDATA_PROMPT_ONE CLEARDATA_PROMPT_TWO

// save stats failure writing
#define SAVESTAT_FAILWONE "Error saving stats to file. Check log for more "
#define SAVESTAT_FAILWTWO "information."
#define SAVESTAT_FAILWRITE SAVESTAT_FAILWONE SAVESTAT_FAILWTWO

// save stats failure recording
#define SAVESTAT_FAILREC "Please stop recording before saving stat data."

// load stats failure reading
#define LOADSTAT_FAILRONE "Error reading stats from file. Check log for more "
#define LOADSTAT_FAILRTWO "information."
#define LOADSTAT_FAILREAD LOADSTAT_FAILRONE LOADSTAT_FAILRTWO

// load stats failure recording
#define LOADSTAT_FAILREC "Please stop recording before loading stat data."

// save image failure writing
#define SAVEIMG_FAILWONE "Error saving image to file. Check log for more "
#define SAVEIMG_FAILWTWO "information."
#define SAVEIMG_FAILWRITE SAVEIMG_FAILWONE SAVEIMG_FAILWTWO

//  CONSTANTS   ===============================================================

//  WINDOW  -------------------------------------------------------------------

// window bounds
const int SS_WINDOW_HT      = WINDOW_HT;
const int SS_WINDOW_WT      = WINDOW_WT;

// window name
const char SS_WINDOWNAME[]  = "Start Select";
const char SS_WINDOWNAMER[] = "Start Select (RECORDING)";

//  DIALOG  -------------------------------------------------------------------

// help dialog
const char SS_DLG_HLP_TEXT[]    = HELP;
const char SS_DLG_HLP_TITLE[]   = "Help";

// save image dialog
const char SS_DLG_SIMG_TITLE[]  = "Save Image";
const char *SS_DLG_SIMG_FILTER[SS_IMGSV_FILT_CT]    = {IMGSAVEFILTER};
const char SS_DLG_SIMG_FILTERDESC[] = "Image files";
const char SS_DLG_SIMG_DFILENAME[]  = DEFIMGFILE;

// stats dialogs
const char *SS_DLG_STAT_FILTER[SS_SSS_FILT_CT]  = {STATSAVEFILTER};
const char SS_DLG_STAT_FILTERDESC[] = "Start Select Stat files";

// save stats dialogs
const char SS_DLG_SSTAT_TITLE[]     = "Save Stats";
const char SS_DLG_SSTAT_DFILENAME[] = DEFSTATFILE;

// load stats dialogs
const char SS_DLG_LSTAT_TITLE[]     = "Load Stats";

// color chooser dialogs
const char SS_DLG_CLRCHO_TITLE[]    = "Choose Base Color";

// TODO adjustments in ui may render the following useless
// TODO these may be removed in coming updates

// start recording message box 
const char SS_DLG_RSTR_MB_TITLE[]       = "Recording Started";
const char SS_DLG_RSTR_MB_MSG[]         = "Recording Started";

// stop recording message box
const char SS_DLG_RSTP_MB_TITLE[]       = "Recording Stopped";
const char SS_DLG_RSTP_MB_MSG[]         = "Recording Stopped";

// clear data failed (recording) message box
const char SS_DLG_CDFR_MB_TITLE[]       = "Clear Data Failed";
const char SS_DLG_CDFR_MB_MSG[]         = CLEARDATA_FAILMSG;

// clear data prompt message box
const char SS_DLG_CDP_MB_TITLE[]        = "Clear Data?";
const char SS_DLG_CDP_MB_MSG[]          = CLEARDATA_PROMPT;

// save stats failed (writing) message box
const char SS_DLG_SSFW_MB_TITLE[]       = TITLE_ERR;
const char SS_DLG_SSFW_MB_MSG[]         = SAVESTAT_FAILWRITE;

// save stats failed (recording) message box
const char SS_DLG_SSFR_MB_TITLE[]       = TITLE_FAIL;
const char SS_DLG_SSFR_MB_MSG[]         = SAVESTAT_FAILREC;

// load stats failed (reading) message box
const char SS_DLG_LSFRD_MB_TITLE[]      = TITLE_ERR;
const char SS_DLG_LSFRD_MB_MSG[]        = LOADSTAT_FAILREAD;

// load stats failed (recording) message box
const char SS_DLG_LSFR_MB_TITLE[]       = TITLE_FAIL;
const char SS_DLG_LSFR_MB_MSG[]         = LOADSTAT_FAILREC;

// save image failed (writing) message box
const char SS_DLG_SIFW_MB_TITLE[]       = TITLE_ERR;
const char SS_DLG_SIFW_MB_MSG[]         = SAVEIMG_FAILWRITE;
