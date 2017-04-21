/**
 * Constants for PS3 controllers
 * @author Andre Allan Ponce
 * @email andreponce@lavabit.com
 *
 * Contains all the constants used for PS3 controllers
 *
 * TODO: move all ps3 constants that arent in here yet to the file
 */

#ifndef SS_PS3_CONSTANTS_H
#define SS_PS3_CONSTANTS_H

//  PS3 CANVAS  ---------------------------------------------------------------
// circle button radius
extern const unsigned int SS_PS3_CB_R;

// parens denote xinput equivalent
// all constants are named using their XInput equiavlent
// button X (A)
extern const unsigned int SS_PS3_A_X;   // x coordinate
extern const unsigned int SS_PS3_A_Y;   // y coordinate

// button Circle (B)
extern const unsigned int SS_PS3_B_X;   // x coordinate
extern const unsigned int SS_PS3_B_Y;   // y coordinate

// button Square (X)
extern const unsigned int SS_PS3_X_X;   // x coordinate
extern const unsigned int SS_PS3_Y_Y;   // y coordinate

// button Triangle (Y)
extern const unsigned int SS_PS3_Y_X;   // x coordinate
extern const unsigned int SS_PS3_Y_Y;   // coordinate

#endif
