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
#ifndef _VGMREADER_H_
#define _VGMREADER_H_
#include <Arduino.h>
#include "config.h"
#include "lcd.h"

#ifdef ARDUINO_UNO
#define BUFSIZE 256  
#endif  
#if (defined ARDUINO_DUE || defined ARDUINO_MEGA)
#define BUFSIZE 2048
#endif  

typedef struct VGMHeader {
  uint8_t id[4]={0,0,0,0};
  uint32_t eofOffset=0;
  uint32_t vgmVersion=0;
  uint32_t clkSn7649=0;
  uint32_t clkYm2413=0;
  uint32_t gd3Offset=0;
  uint32_t totalSamples=0;
  uint32_t loopOffset=0;
};

class VGMReader
{
public:
  VGMReader();
  void attachLCD(LCD *);
  bool read();
  void dumpHeader();
  void printGD3();
  void getVersion(uint8_t &major, uint8_t &minor);
  uint16_t readData(uint32_t offset, bool srcPCM = false);
  uint8_t nextByte(bool srcPCM = false);
  void pcmBegin();
  void pcmNewOffset();
  uint32_t getCursor();
  uint32_t getDataLength();
  void loop();
  uint8_t getTrackNameLength();
  uint8_t getGameNameLength();

private:
  bool parseHeader();
  void parseGD3Info();
  void readGD3String (uint32_t &offset, char *strValue);

  VGMHeader m_header;
  uint32_t  m_vgmDataOffset;
  uint32_t  m_dataLength;       // # bytes in vgm data
  uint8_t   m_buf[BUFSIZE];     // VGM data buffer
  uint8_t   m_pcmBuf[BUFSIZE];  // PCM data buffer
  uint32_t  m_pcmStart;         // offset in file where pcm data starts
  uint32_t  m_pcmLength;        // # bytes in pcm data

  uint16_t  m_bufCursor;        // position in buffer
  uint16_t  m_pcmCursor;        // position in pcm buffer
  uint32_t  m_pcmNextPage;      // position in file where next pcm chunk
                                // will be read from.
  uint32_t  m_fileCursor;       // position in whole file

  uint8_t   m_GD3TrackName[40] {0};
  uint8_t   m_GD3GameName[40] {0};
  LCD       *m_lcd;
};

#endif
