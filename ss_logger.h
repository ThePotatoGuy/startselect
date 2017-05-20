/**
 * Special custom built logger to log message to file
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Can log messages to three logs:
 * DEBUG - special debug messages
 * ERROR - error messages
 * GENERAL - general log messages
 */

#ifndef SS_LOGGER_H
#define SS_LOGGER_H

//  ENUMS   ===================================================================

// the log enums
typedef enum{
    SS_LOG_DBG, // debug log
    SS_LOG_ERR, // error log
    SS_LOG_GEN  // general log
} SS_LOG_T;

//  FUNCTIONS   ===============================================================

/*
 * Logs the given message to the appropriate log based on the given logtype
 *
 * The message will be preprended with appropriate timestamp and other data
 *
 * IN:
 *  @param logtype - the type of log to log this message to
 *  @param msg - the message to log
 *
 * OUT:
 *  @returns SS_RETURN_SUCCESS on success, SS_RETURN_FAILURE otherwise
 */
int ss_log(SS_LOG_T logtype, const char *msg);

#endif
