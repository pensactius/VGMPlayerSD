#ifndef _GD3_H_
#define _GD3_H

#include "config.h"

/***********************************************************************
 *The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno, 51 on Mega 2560, SPI MOSI on DUE
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
     Pin 4 used here for consistency with other Arduino examples
 ************************************************************************/
#ifdef ARDUINO_UNO
const int chipSelect = 10;
#endif
#ifdef ARDUINO_MEGA
const int chipSelect = 53;
#endif
#ifdef ARDUINO_DUE
const int chipSelect = 53;
#endif

void SDInit(const char *dirName);
bool SDOpen(const char *fileName);
bool SDClose();
bool SDNext();

#endif
