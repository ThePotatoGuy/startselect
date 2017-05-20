/** 
 * Menu Data definition
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Menu data is data that is shared between dialog boxes and menu pages
 */

#ifndef SS_MENUDATA_H
#define SS_MENUDATA_H

#include "ss_renderdata.h"
#include "ss_threaddata.h"

//  ENUMS   ===================================================================

// recording state enum
typedef enum {
    SS_REC_ON,
    SS_REC_OFF
} SS_RECSTATE;

//  TYPES   ===================================================================

// struct that holds data needed by the menus and canvas functions
typedef struct{

    // current recording state
    SS_RECSTATE state;

    // render data for image gen
    ss_renderdata *render_data;

    // thread data for menu threads
    ss_threaddata *thread_data;

} ss_menudata;

#endif
