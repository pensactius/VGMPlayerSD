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
#include "VGMPlayer.h"
#include "sdcard.h"
#include "sn76489.h"
#include "ym2612.h"
#include <util/delay.h>

VGMPlayer::VGMPlayer(LCD *lcd) : m_addr(0), m_data(0)
{  
  SN76489SetBus();
  m_vgm.attachLCD (lcd);
  m_lcd = lcd;
}

bool VGMPlayer::read()
{
  SN76489_Off(); 
  if (!m_vgm.read()) return false;
  //m_vgm.dumpHeader();
  m_vgm.printGD3();
  return true;
}

void VGMPlayer::play()
{
  SN76489_Off();
  YM2612Reset();
  unsigned long singleSampleWait {0};
  const float sampleRate = 44100.0;
  const float WAIT60TH = ( (1000.0 / (sampleRate/(float)735))*1000 );
  const float WAIT50TH = ( (1000.0 / (sampleRate/(float)882))*1000 );
  unsigned long startTime {0};
  unsigned long pauseTime {0};
  unsigned long preCalced8nDelays[16] {0};
  unsigned long preCalced7nDelays[16] {0};

  singleSampleWait = ((1000.0 / (sampleRate/(float)1))*1000 );

  for (int i=0; i<16; i++)
  {
    if (i==0) {
      preCalced8nDelays[i] = 0;
      preCalced7nDelays[i] = 1;
    }
    else {
      preCalced8nDelays[i] = ( (1000.0 / (sampleRate/(float)i  ))*1000 );
      preCalced7nDelays[i] = ( (1000.0 / (sampleRate/(float)i+1))*1000 );
    }
  }
  current_time = millis();  
  while (1) {

    // Skip to next song if NEXT button pressed
    if (digitalRead (NEXT_PIN) == 0) return;
    
    // Deal with delay timings
    unsigned long timeInMicros = micros(); 
    if (timeInMicros - startTime <= pauseTime)
    {
      continue;      
    }

    m_data = m_vgm.nextByte();
    switch (m_data) {

     // 0x4F dd : PSG stereo, ignored for now
     case 0x4F:
        SN76489WriteData (0x06);        
        m_data = m_vgm.nextByte();
        SN76489WriteData (m_data);
        startTime = timeInMicros;
        pauseTime = singleSampleWait;
        break;
        
     // 0x50 dd : SN76489 Escribe valor dd
      case 0x50:        
        m_data = m_vgm.nextByte();
        SN76489WriteData (m_data);
        startTime = timeInMicros;
        pauseTime = singleSampleWait;
        break;

    /* 0x52 aa dd: YM2612 Port0, write value dd to register aa
       0x53 aa dd: YM2612 Port1, write value dd to register aa */
      case 0x52:
        m_addr = m_vgm.nextByte();
        m_data = m_vgm.nextByte();
        YM2612WritePort0 (m_addr, m_data);
        startTime = timeInMicros;
        pauseTime = singleSampleWait;
        break;
      case 0x53:
        m_addr = m_vgm.nextByte();
        m_data = m_vgm.nextByte();
        YM2612WritePort1 (m_addr, m_data);
        startTime = timeInMicros;
        pauseTime = singleSampleWait;
        break;
      case 0x61:
      {
        uint8_t lo = m_vgm.nextByte();
        uint8_t hi = m_vgm.nextByte();
        uint32_t wait = (uint32_t)word (hi, lo);
        startTime = timeInMicros;
        pauseTime = ( (1000.0 / (sampleRate/(float)wait))*1000 );        
        break;
      }
      // 0x62: Wait 1/60th second
      case 0x62:  
        startTime = timeInMicros;
        pauseTime = WAIT60TH;
        break;

      // 0x63: Wait 1/50th second
      case 0x63:
        startTime = timeInMicros;
        pauseTime = WAIT50TH;
        break;

      // 0x7n: Wait n+1 samples, n can range from 0 to 15.                
      case  0x70:
      case  0x71:
      case  0x72:
      case  0x73:
      case  0x74:
      case  0x75:
      case  0x76:
      case  0x77:
      case  0x78:
      case  0x79:
      case  0x7A:
      case  0x7B:   
      case  0x7C:
      case  0x7D:
      case  0x7E:
      case  0x7F:
      {
        uint32_t wait = (m_data & 0x0F);        
        startTime = timeInMicros;   
        pauseTime = preCalced7nDelays[wait];
        break;
      }
      case  0x80:
      case  0x81:
      case  0x82:
      case  0x83:
      case  0x84:
      case  0x85:
      case  0x86:
      case  0x87:
      case  0x88:
      case  0x89:
      case  0x8A:
      case  0x8B:   
      case  0x8C:
      case  0x8D:
      case  0x8E:
      case  0x8F:      
      {
        
        uint32_t wait = (m_data & 0x0F);        
        m_data = m_vgm.nextByte(true);
        YM2612WritePort0 (0x2A, m_data);
        startTime = timeInMicros;
        pauseTime = preCalced8nDelays[wait];
        break;
      }
      //0x66 : End of Sound Data
      case 0x66:
      {
        // loop switch activated?
        if (digitalRead(LOOP_PIN) == 0) {
          m_vgm.loop();
          Serial.println(F("End of Song Data (Loop)"));
        }
        else {
          return;
        }
        break;
      }
      // Parse 0x67 0x66 tt ss ss ss ss (data)  
      case 0x67:
        m_vgm.pcmBegin();
        break;

      case 0xE0:
        m_vgm.pcmNewOffset();
        break;
        
      default:
        Serial.print(m_vgm.getCursor()-1, HEX); Serial.print(F(": ")); 
        Serial.print(F("("));
        Serial.print(m_data, HEX);
        Serial.print(F(" "));
        Serial.println(F("UNKNOWN cmd!"));
        SN76489_Off();
        YM2612Reset();
        for(;;);
    } // switch
    /*if ( (m_vgm.getTrackNameLength() > 16 || m_vgm.getGameNameLength() > 16) 
        && (millis() - current_time > 1500) ) {
      m_lcd->scroll();
      current_time = millis();
    }*/
  } // while    
  SN76489_Off();
  YM2612Off();
}

/*
void VGMPlayer::print(const char *msg, uint8_t col = 0, uint8_t row = 0)
{
  m_lcd.clear();
  m_lcd.print (msg, col, row);
}
*/
