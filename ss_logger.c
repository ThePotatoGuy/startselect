/**
 * Implementation of Logger
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 */

#include <stdio.h>

#include <time.h>

#include "ss_constants.h"

#include "ss_logger.h"

//  DEFINES ===================================================================

// filenames for the logs
#define DEBUG_LOG_FN "ss-debug.log"
#define ERROR_LOG_FN "ss-error.log"
#define GENERAL_LOG_FN "ss-general.log"

// formatting strings
#define DEBUG_LOG_FMT "[%s]-DEBUG: %s\n"
#define ERROR_LOG_FMT "[%s]-ERROR: %s\n"
#define GENERAL_LOG_FMT "[%s]: %s\n"

// log error message
#define ERR_WRITE "ERROR: Failed to write to %s\n"

// log append
#define APPEND "a"

// log erase
#define ERASE "w"

// log bad state
#define BADSTATE "Invalid logtype given"

//  STATIC IMPLEMENTATION   ===================================================

/*
 * Gets the log filename that is associated with teh given logtype
 *
 * IN:
 *  @param logtype - the given logtype
 *
 * OUT:
 *  @returns the associated filename, or NULL if no filename is associated with
 *      the given logtype
 */
static char* get_logfilename(SS_LOG_T logtype);

/*
 * Gets the log format string that is associated with the given logtype
 *
 * IN:
 *  @param logtype - the given logtype
 *
 * OUT:
 *  @returns the associated format string, or NULL if no format string is
 *      associated with the given logtype
 */
static char* get_logfmtstring(SS_LOG_T logtype);

/*
 * Writes the message to file
 *
 * IN:
 *  @param filename - the name of the file to write
 *  @param fmtstr - the format string we are using to write to file
 *  @param msg - the message to write
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
static int write_log(
        const char *filename, 
        const char *fmtstr, 
        const char *msg
);

//  IMPLEMENTATION  ===========================================================

int ss_log(SS_LOG_T logtype, const char *msg){

    char *filename;
    char *fmtstring;

    filename = get_logfilename(logtype);
    fmtstring = get_logfmtstring(logtype);

    if (filename == NULL || fmtstring == NULL){
         // bad log state
        ss_log(SS_LOG_ERR, BADSTATE);
        return SS_RETURN_FAILURE;    
    }

    return write_log(filename, fmtstring, msg);
} // ss_log

//  STATIC IMPLEMENTATION   ===================================================

static char* get_logfilename(SS_LOG_T logtype){
    switch (logtype){
        case SS_LOG_DBG:
        {
            return DEBUG_LOG_FN;
        }
        case SS_LOG_ERR:
        {
            return ERROR_LOG_FN;
        }
        case SS_LOG_GEN:
        {
            return GENERAL_LOG_FN;
        }
        default:
        {
            return NULL;
        }
    }
} // get_logfilename

static char* get_logfmtstring(SS_LOG_T logtype){
     switch (logtype){
        case SS_LOG_DBG:
        {
            return DEBUG_LOG_FMT;
        }
        case SS_LOG_ERR:
        {
            return ERROR_LOG_FMT;
        }
        case SS_LOG_GEN:
        {
            return GENERAL_LOG_FMT;
        }
        default:
        {
            return NULL;
        }
    }   
} // get_logfmtstring

static int write_log(
        const char *filename, 
        const char *fmtstr, 
        const char *msg
){

    FILE *file;
    time_t rawtime;

    // attempt to open file

    file = fopen(filename, APPEND);

    if (file == NULL){
        // couldnt open file
        printf(ERR_WRITE, filename);
        return SS_RETURN_FAILURE;
    }

    // grab current time
    time(&rawtime);

    // now begin file writing
    if (fprintf(file, fmtstr, ctime(&rawtime), msg) < 0){
        // error occured
        printf(ERR_WRITE, filename);
        fclose(file);
        return SS_RETURN_FAILURE;
    }

    // otherwise we good
    fclose(file);

    return SS_RETURN_SUCCESS;

} // write_log
