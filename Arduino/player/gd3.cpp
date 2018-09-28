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
/*
  gd3.cpp
    This file provides helper functions to parse GD3 info. It's part of 
    the implementation of the VGMReader class.
*/
#include "vgmreader.h"
#include <SdFat.h>

// Reference to the current openfile
extern File file;

/*
  readGD3String()
    GD3 info is stored as a 2 byte wide character and terminated with
    two null characters. This helper function reads such strings
    (2 bytes/character) and converts to a C like (1 byte/character) null 
    terminated string.
    Additionally if nullptr is passed as the second parameter it is 
    ignored and the function can be used to just skip the GD3 string 
    and updte the offset to point to the next GD3 string. This can be 
    useful to skip Japanese strings and update the offset to the next 
    English GD3 string.

  Parameters:
    [In] offset: 
      Absolute offset in the VGM file where the GD3 string begins. 
      At the end of the function this offset is updated to point to the 
      next GD3 string so it can be used directly on a successfive 
      call to extract the next GD3 string.

    [Out] strValue:
      Converted C like asciiz string or nullptr if we are not interested
      in the converted string.
*/
void VGMReader::readGD3String (uint32_t &offset, char *strValue)
{
  // c1, c2: two bytes are used to read each 2 byte/char individually
  char c1=0, c2=0, i=0;
  
  /*
    Read each 2-byte wide characters, we are only interested in the first
    one (c1) so we append it to the converted C string (strValue) but only
    if we are not beyond the strValue length, set to 40 in the VGMReader
    class definition.
    We must update the offset by two each time we read a 2-byte char. 
    to ensure we point to the next one.
    We keep reading the GD3 string until we read two consecutives 0x00,0x00.
  */
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

/*
  parseGD3Info()
    Extracts the track name and game name from the GD3 header and stores
    them to the corresponding private member variables in the VGMReader class.
*/
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

