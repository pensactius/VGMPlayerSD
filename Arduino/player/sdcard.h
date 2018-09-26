/* VGMPlayerSD, a Video Game Music player with Arduino and hardware audio ICs.
   Copyright (C) 2018  Andrés Mata Bretón

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA 
*/
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
