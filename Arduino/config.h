#ifndef _CONFIG_H_
#define _CONFIG_H_
/***********************************************************************
 * Set board: one of  
 *  ARDUINO_UNO
 *  ARDUINO_MEGA
 *  ARDUINO_DUE
 ************************************************************************/
//#define ARDUINO_UNO
//#define ARDUINO_MEGA
#define ARDUINO_DUE

#ifdef ARDUINO_DUE
#define LOOP_PIN  43	// Switch button to en/disable vgm loop
#define NEXT_PIN  42	// Push button for next song
#endif

#endif
