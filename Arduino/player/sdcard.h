#ifndef _GD3_H_
#define _GD3_H_

#include "config.h"

/***********************************************************************
 *The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 51 on Arduino MEGA, MOSI(A26) at SPI header on DUE, 
           pin 11 on Arduino Uno
 ** MISO - pin 50 on Arduino MEGA, MISO(A25) at SPI header on DUE, 
           pin 12 on Arduino Uno
 ** CLK  - pin 52 on Arduino MEGA, SCK(A27) at SPI header on DUE, 
           pin 13 on Arduino Uno
 ** CS   - defined below as chipSelect     
 ************************************************************************/
#ifdef ARDUINO_UNO
#define chipSelect 10
#endif
#if (defined ARDUINO_MEGA) || (defined ARDUINO_DUE)
#define chipSelect 53
#endif

void SDInit(const char *dirName);
bool SDOpen(const char *fileName);
bool SDClose();
bool SDNext();

#endif
