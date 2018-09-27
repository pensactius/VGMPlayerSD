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

void VGMReader::readGD3String (uint32_t &offset, char *strValue)
{
  char c1=0, c2=0, i=0;
  
  do {
    file.read(&c1, 1);    
    file.read(&c2, 1);
    if ( (strValue != nullptr) && (i < 40) )
    {
      strValue[i] = c1;
    }
    ++i;
    offset += 2;
    file.seekSet (offset);
  } while ((c1 != '\0') || (c2 != '\0'));
}

void VGMReader::parseGD3Info()
{
  // First 12 bytes of gd3 data are not relevant to us.  
  uint32_t offset = m_header.gd3Offset+12;  
  file.seekSet (offset);

  //Serial.println (F("Reading gd3 from")); Serial.println(offset, HEX);

  // Read track name
  readGD3String (offset, m_GD3TrackName);
  // Skip japanese track name
  readGD3String(offset, nullptr);
  // Read game name 
  readGD3String(offset, m_GD3GameName);
}

