/**
 * Configuration
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Configuration settings for all files
 */

#ifndef SS_CONFIG_H
#define SS_CONFIG_H

// flips the printing/accessing of the joystick grid so the grid is accessed
// in both reverse column and reverse row order
//#define SS_JOYGRID_FLIP

#ifndef SS_JOYGRID_FLIP
// flips printing/accessing of the joystick grid so the grid is accessed
// in reverse column order
//#define SS_JOYGRID_FLIP_X

#else // SS_JOYGRID_FLIP defined
#define SS_JOYGRID_FLIP_X
#endif // SS_JOYGRID_FLIP defined

#ifndef SS_JOYGRID_FLIP
// flips printing/accessing of the joystick grid so the grid is accessed in
// reverse row order
//#define SS_JOYGRID_FLIP_Y

#else // SS_JOYGRID_FLIP defined
#define SS_JOYGRID_FLIP_Y
#endif // SS_JOYGRID_FLIP defined

// swaps the coordinates for x and y such that x becomes up and down axis
// and y is left and right x axis (needed for ps3 controller)
#define SS_XY_SWAP

#endif
