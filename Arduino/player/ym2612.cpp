#include <Arduino.h>
#include "ym2612.h"

void YM2612SetBus()
{
  //for (auto pin : YM2612_PINS) pinMode (pin, OUTPUT);
  /* Pins setup */
  YM_CTRL_DDR |= _BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD) | _BV(YM_A0) | _BV(YM_A1);
  YM_DATA_DDR = 0xFF;

  YM_CTRL_PORT |= _BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD); /* IC, CS, WR and RD HIGH by default */
  YM_CTRL_PORT &= ~(_BV(YM_A0) | _BV(YM_A1)); /* A0 and A1 LOW by default */
}
void YM2612Reset()
{
  /* Reset YM-2612 by 0 - 1 IC */
  YM_CTRL_PORT &= ~_BV(YM_IC);
  delayMicroseconds(25);
  YM_CTRL_PORT |= _BV(YM_IC);
  //delayMicroseconds(25);
}

/**
 * Send raw data to the YM2612
 * 
 * @author Furrtek
 * @param data Data to write
 */

void _write_ym(uint8_t data) {
  // CS LOW
  YM_CTRL_PORT &= ~_BV(YM_CS); 
  
  // DATA -> YM2612
  //PORTD = (PORTD & 0x03) | (data << 2);           // D2<-N0, D3<-N1 .. D7<-N5
  //PORTB = (PORTB & 0xfc) | ( (data & 0xC0) >> 6); // D11<-N6, D12<-N7
  YM_DATA_PORT = data;

  // Write data
  //_delay_us(1);
  YM_CTRL_PORT &= ~_BV(YM_WR);   
  //_delay_us(1);
  YM_CTRL_PORT |= _BV(YM_WR);

  // CS HIGH
  YM_CTRL_PORT |= _BV(YM_CS); 
  _delay_us(1);
  
}

void YM2612WritePort0(uint8_t reg, uint8_t data) {
  YM_CTRL_PORT &= ~_BV(YM_A1); // A1 low |___ Address of FM channel 1-3 
  YM_CTRL_PORT &= ~_BV(YM_A0); // A0 low |
  _write_ym(reg);
  YM_CTRL_PORT &= ~_BV(YM_A1); // A1 low  |___ Data write of FM channel 1-3
  YM_CTRL_PORT |= _BV(YM_A0);  // A0 high |
  _write_ym(data);  
}

void YM2612WritePort1(uint8_t reg, uint8_t data) {
  YM_CTRL_PORT |=  _BV(YM_A1);  // A1 high |___ Address of FM channel 4-6 
  YM_CTRL_PORT &= ~_BV(YM_A0);  // A0 low  |
  _write_ym(reg);
  
  YM_CTRL_PORT |= _BV(YM_A1);   // A1 high |___ Data write of FM channel 4-6
  YM_CTRL_PORT |= _BV(YM_A0);   // A0 high |
  _write_ym(data);  
}

void YM2612Off() {
  YM2612WritePort0(0x28, 0x00); // Key off
  YM2612WritePort0(0x28, 0x01); // Key off
  YM2612WritePort0(0x28, 0x02); // Key off
  YM2612WritePort0(0x28, 0x04); // Key off
  YM2612WritePort0(0x28, 0x05); // Key off
  YM2612WritePort0(0x28, 0x06); // Key off  
}

