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
#include <Arduino.h>
#include "sn76489.h"
#include "config.h"

/*
  SN7649WriteData()
    Writes a byte to the SN-76489 audio chip. Each supported board
    (Arduino Mega / Due) have different pins assigned to the data
    bus. The function sends the byte data to the correct pins and
    simultaneously if available by direct port manipulation.

  Parameters:
    [in]: data
      Byte data to send to the audio chip.
*/
void SN76489WriteData(char data) {
digitalWrite (PSG_WE, 1); // _WE HIGH

#ifdef ARDUINO_UNO
  // Envia bits 5-0 a PD7-PD2
  PORTD = (PORTD & 0x02) | ( (data & B00111111) << 2 );
  
  // Envia bits 7-6 a PB1-PB0
  PORTB = (PORTB & 0xfc) | ( (data & B11000000) >> 6 );  
#endif
#ifdef ARDUINO_DUE
  // Send data byte to C2-C9 (pin 34 .. pin 41)
  PIOC->PIO_OWER = 0x03FE;
  PIOC->PIO_ODSR = data << 2;
#endif
#ifdef ARDUINO_MEGA
  // Send data byte to PA0-PA7 (pin 22 .. pin 29)
  PORTA = data;
#endif

  // ~WE and ~CE low (active)
  digitalWrite(PSG_WE, 0); 
  delayMicroseconds (14);  
  // ~WE and ~CE high (inactive)
  digitalWrite(PSG_WE, 1);
}

void SN76489SetBus() {
// On all boards A0 is ~WE for SN-76489 and it's an OUTPUT.
  pinMode(PSG_WE, OUTPUT);
  digitalWrite (PSG_WE, HIGH);
}

void SN76489_Off() {
  SN76489WriteData(B10011111);
  SN76489WriteData(B10111111);
  SN76489WriteData(B11011111);
  SN76489WriteData(B11111111);
}
