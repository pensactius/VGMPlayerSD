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
/*
El bus de datos está conectado de la siguiente manera:

  7   6   5   4   3   2   1   0     sn76489 data 
PB1  PB0 PD7 PD6 PD5 PD4  PD3 PD2   Avr naming)
 D9   D8  D7  D6  D5  D4   D3  D2   Arduino naming

Por tanto hay que seleccionar los bits y posicionarlos correctamente
a la hora de enviarlos por el puerto correspondiente (PORTD o PORTB).

*/
  // Put the data to the bus by setting the corresponding bus pins 
  // to high/low.
#ifdef ARDUINO_UNO
  // Envia bits 5-0 a PD7-PD2
  PORTD = (PORTD & 0x02) | ( (data & B00111111) << 2 );
  
  // Envia bits 7-6 a PB1-PB0
  PORTB = (PORTB & 0xfc) | ( (data & B11000000) >> 6 );
  
  // ~WE y ~CE a low (Active)
  PORTB &= B11101011;
  // Small delay to make sure the SN-76489 is ready to fetch the data
  _delay_us (10);
  // ~WE y ~CE a high (InActive)
  PORTB |= B00010100;
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
  delayMicroseconds (10);  
  // ~WE and ~CE high (inactive)
  digitalWrite(PSG_WE, 1);



}

void SN76489SetBus() {
#ifdef ARDUINO_UNO  
  DDRB |= 0x17; // Bits PB4, PB2, PB1 and PB0 (D12, D10, D9,D8) como Salida
  DDRD |= 0xfc; // Bits PD2 to PD7  (D2-D7) como Salida

  // ~WE y ~CE a high (InActive)
  PORTB |= B00010100;
#endif

#ifdef ARDUINO_DUE
  for (uint8_t i = 34; i <=41; i++) pinMode(i, OUTPUT);
  Serial.println(F("SN76489 initialized"));
#endif

// On Arduino MEGA PORTA is the data bus, we must set all
// pins in PORTA as OUTPUTs.
#ifdef ARDUINO_MEGA
  DDRA = 0xFF;
#endif

// On all boards A0 is ~WE for SN-76489 and it's an OUTPUT.
  pinMode(PSG_WE, OUTPUT);
}

void SN76489_Off() {
  SN76489WriteData(B10011111);
  SN76489WriteData(B10111111);
  SN76489WriteData(B11011111);
  SN76489WriteData(B11111111);
}
