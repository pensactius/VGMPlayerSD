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
#include <SdFat.h>

extern File file;

VGMReader::VGMReader() : m_vgmDataOffset(0), m_dataLength(0), m_bufCursor(0)
{
}

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

uint16_t VGMReader::readData(uint32_t offset)
{  
  file.seekSet(offset);
  file.read(m_buf, sizeof(m_buf));
  m_bufCursor = 0;
}

uint8_t VGMReader::nextByte()
{
  if (m_bufCursor == BUFSIZE) {
    readData (m_fileCursor);
  }
  m_fileCursor++;
  return m_buf[m_bufCursor++];
}

uint32_t VGMReader::getCursor()
{
  return m_fileCursor;
}

void VGMReader::loop()
{  
  readData(m_header.loopOffset+m_vgmDataOffset);
  Serial.print(F("Loop to "));Serial.println(m_header.loopOffset+m_vgmDataOffset,HEX);
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

void VGMReader::dumpHeader(LCD &lcd)
{  
  Serial.println((char *)m_GD3GameName);
  lcd.clear();
  lcd.print(m_GD3TrackName, 0, 0);
  lcd.print(m_GD3GameName, 0, 1);
  Serial.println(strlen(m_GD3TrackName));
  Serial.println(strlen(m_GD3GameName));
}

uint8_t VGMReader::getTrackNameLength() { return strlen(m_GD3TrackName);}
uint8_t VGMReader::getGameNameLength() { return strlen(m_GD3GameName);}
