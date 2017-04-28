/**
 * Processes STats into colors
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Contains functions that process a stat struct into appropriate colors
 */

#ifndef SS_STATPROCESSOR_H
#define SS_STATPROCESSOR_H

#include "ss_stats.h"

#include "ss_ps3_statcolors.h"

//  FUNCTIONS   ===============================================================

/*
 * Converts the given stats into color data (modifiying alpha data only)
 *
 * IN:
 *  @param stats - the stats to convert/process
 *
 * OUT:
 *  @param colors - the color data with modified alphas
 */
void ss_colorizealpha(
        ss_ps3_colors *colors, 
        const ss_generic_controller_stats *stats
);

#endif
