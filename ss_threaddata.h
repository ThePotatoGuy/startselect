/**
 * Contains thread data defintiion
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Just the thread data definition
 */

#ifndef SS_THREADDATA_H
#define SS_THREADDATA_H

#include <pthread.h>

#include "ss_stats.h"

//  TYPES   ===================================================================

// struct shared between the event handling thread and render thread
typedef struct{

    /* quit signal, broadcasted from openwindow */
    pthread_mutex_t     *end_mutex;
    int                 *end;

    // stats have been sent signal
    pthread_mutex_t     *sentstat_mutex;
    int                 *sentstat;

    // start recording signal
    pthread_mutex_t     *recstart_mutex;
    int                 *recstart;

    // stop recording signal
    pthread_mutex_t     *recstop_mutex;
    int                 *recstop;

    // stat data
    pthread_mutex_t     *stats_mutex;
    ss_generic_controller_stats *stats;

} ss_threaddata;

#endif
