#ifndef _CONFIG_H_
#define _CONFIG_H
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
#define LOOP_PIN  43
#define NEXT_PIN  42
#endif

#endif
