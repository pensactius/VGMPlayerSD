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
#include "vgmreader.h"
#include "sn76489.h"
#include <SdFat.h>

extern File file;

VGMReader::VGMReader() : m_vgmDataOffset(0), m_dataLength(0), m_bufCursor(0), 
  m_pcmCursor {0}, m_pcmNextPage {0}, m_fileCursor{0} {}

void VGMReader::attachLCD(LCD *lcd) { m_lcd = lcd; }

bool VGMReader::read()
{
  //file.printName(&Serial);Serial.println();
  if (!parseHeader()) return false;
  
  m_dataLength = m_header.gd3Offset - m_vgmDataOffset;
  m_fileCursor = m_vgmDataOffset;
  readData(m_fileCursor);
  //file.printName(&Serial);Serial.println();
  return true;
}
bool VGMReader::parseHeader()
{
  // Read 32 bytes from header
  file.read (&m_header, 32);

  if (m_header.id[0] != 0x56 || //'V' 
      m_header.id[1] != 0x67 || //'G'
      m_header.id[2] != 0x6D || //'M'
      m_header.id[3] != 0x20)   //' '
      return false;

  // Store absolute offset of GD3 data
  m_header.gd3Offset += 0x14;

  // Read the relative offset to VGM data stream
  file.seekSet(0x34);
  uint32_t offset;
  file.read (&offset, sizeof(offset));
  m_vgmDataOffset = offset != 0 ? offset + 0x34 : 0x40;

  // Store loop offset relative to vgm song data
  m_header.loopOffset += m_header.loopOffset != 0 ? 0x1C - m_vgmDataOffset : 0;
    
  parseGD3Info();
  return true;
}

void VGMReader::getVersion(uint8_t &major, uint8_t &minor)
{
  // Version number in BCD-Code. e.g. Version 1.70 is stored as 0x00000171  
  minor = m_header.vgmVersion & 0xff;
  major = (m_header.vgmVersion & 0xff00) >> 8;
}

uint32_t VGMReader::getDataLength()
{
    return m_dataLength;
}

uint16_t VGMReader::readData(uint32_t offset, bool srcPCM = false)
{
  uint16_t &bufCursor = srcPCM ? m_pcmCursor : m_bufCursor;
  uint8_t (&buf)[BUFSIZE] = srcPCM ? m_pcmBuf : m_buf;
  file.seekSet(offset);
  if ( file.read ( buf, sizeof (buf) ) == -1) { 
    SN76489_Off();
    m_lcd->clear();
    m_lcd->print("Read error!");
    for(;;);
  }
  bufCursor = 0;
//  Serial.print(F("."));
}

uint8_t VGMReader::nextByte(bool srcPCM = false)
{
  if (!srcPCM) {
    if (m_bufCursor == BUFSIZE) {    
      readData (m_fileCursor);
    }
    m_fileCursor++;
    return m_buf[m_bufCursor++];
  }
  else {    
    if (m_pcmCursor == BUFSIZE) {      
      //Serial.println ("Next PCM chunk");  
      readData (m_pcmNextPage, true);
      m_pcmNextPage += BUFSIZE;
    }
    return m_pcmBuf[m_pcmCursor++];
  }
  
}

void VGMReader::pcmBegin()
{   
  // pcmBegin() is called right after cmd 0x67 is
  // found on the VGM stream. 
  // Parse 0x67 0x66 tt ss ss ss ss (data)  
  // Skip 0x66 
  file.seekSet(++m_fileCursor);         
  uint8_t tt;
  // and read tt value
  m_fileCursor += file.read (&tt, 1);     
  // only uncompressed pcm data is supported
  if (tt > 0x40) {                      
    Serial.println (F("Unsupported PCM"));
    return;
  }
  else {
    uint32_t pcmOffset {0};                 
    // get pcm length
    m_fileCursor += file.read (&m_pcmLength, 4);
    m_pcmStart = m_fileCursor;
    // Offset in file for the next pcm data chunk
    m_pcmNextPage = m_fileCursor + BUFSIZE;
      
    // I am commenting out reading a pcm chunk now, since usually
    // a 0xE0 cmd will follow a 0x67 to actually set the
    // pcm offset.

    // Cursor in pcm data starts at offset 0
    //m_pcmCursor = 0;
    
    // Read BUFSIZE pcm data into pcm buffer
    //readData (m_pcmStart, true);    
    // VGM data continues after pcm data section
    m_fileCursor += m_pcmLength;
    // Read VGM data
    readData (m_fileCursor);
  }  
}

void VGMReader::pcmNewOffset()
{
  // Read the new pcm offset
  uint32_t newOffset {0};
  file.seekSet (m_fileCursor);
  m_fileCursor += file.read (&newOffset, 4);  
  m_bufCursor += 4;
  //Serial.print ("PCM Offset: "); Serial.println (newOffset);
  // Reset pcm cursor to start of buffer
  m_pcmCursor = 0;
  // Offset in file for the next pcm data chunk
  m_pcmNextPage = m_pcmStart + newOffset + BUFSIZE;  
  //Serial.print ("m_pcmNextPage: "); Serial.println (m_pcmNextPage);  
  // Read BUFSIZE pcm data into pcm buffer
  readData (m_pcmStart + newOffset, true);  
}

uint32_t VGMReader::getCursor()
{
  return m_fileCursor;
}

void VGMReader::loop()
{  
  readData(m_header.loopOffset+m_vgmDataOffset);
  //Serial.print(F("Loop to "));Serial.println(m_header.loopOffset+m_vgmDataOffset,HEX);
  m_fileCursor = m_header.loopOffset+m_vgmDataOffset;
}

void VGMReader::dumpHeader()
{
  /*  Serial.print(F("id: \t\t"));
  for (uint8_t i = 0; i < 3; i++)
    Serial.print((char)m_header.id[i]);
*/
  uint8_t minVersion, majVersion;
  getVersion(majVersion, minVersion);
  //  Serial.print(F("\neofOffset:\t\t")); Serial.println(m_header.eofOffset,HEX);
  Serial.print(F("VGM Version:\t")); Serial.print(majVersion, HEX); Serial.print(".");Serial.println(minVersion, HEX);
  Serial.print(F("clkSn7649:\t\t")); Serial.println((float)m_header.clkSn7649/1e6);
  Serial.print(F("gd3Offset\t\t")); Serial.println(m_header.gd3Offset,HEX);
//  Serial.print(F("totalSamples\t")); Serial.println(m_header.totalSamples,HEX);
  Serial.print(F("loopOffset\t\t")); Serial.println(m_header.loopOffset,HEX);
  Serial.print(F("Data start\t\t")); Serial.println(m_vgmDataOffset, HEX);
  Serial.print(F("Data length\t\t")); Serial.println(m_dataLength);
}

void VGMReader::printGD3()
{  
  //Serial.println((char *)m_GD3GameName);
  m_lcd->clear();
  m_lcd->print(m_GD3TrackName, 0, 0);
  m_lcd->print(m_GD3GameName, 0, 1);
  //Serial.println(strlen(m_GD3TrackName));
  //Serial.println(strlen(m_GD3GameName));
}

uint8_t VGMReader::getTrackNameLength() { return strlen(m_GD3TrackName);}
uint8_t VGMReader::getGameNameLength() { return strlen(m_GD3GameName);}
