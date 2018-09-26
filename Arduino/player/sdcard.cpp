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
  This file contains helper functions for SD card access and reading files.
  It uses the SdFat library from "https://github.com/greiman/SdFat"
*/

#include <SPI.h>
#include <SdFat.h>
#include "sdcard.h"

SdFat SD;
File file;

/*
  SDInit()
    Initializes the SD card by calling the corresponding functions in the
    SdFat library. If it fails it halts execution and shows an error message.

    If it succeeds it chdirs to the specified directory in the parameter.

  Parameters
    [in] path: Full path to chdir 

*/
void SDInit(const char *path)
{
  Serial.print(F("\nInitializing SD card...\n"));

  // Try to initialize the SD card at speed of 50MHz. If this fails, try
  // with a number < 50 until it succeeds.
  if(!SD.begin(chipSelect,SD_SCK_MHZ(50)))
    SD.initErrorHalt();
  
  Serial.println(F("Wiring is correct and a card is present."));
  Serial.println(F("initialization done."));   

  // Set volume working directory vwd to root.
  if (!SD.chdir(path, true))
    SD.initErrorHalt(F("Chdir failed, make sure path is correct"));
}


/*
  SDOpen()
    Opens the specified file in the SD card.

  Parameters
    [in] fileName: File to open

  Returns
    True if file was successfully opened, False otherwise.
*/
bool SDOpen(const char *fileName)
{
  Serial.print(F("\nOpening file "));Serial.println(fileName);
  if (!SD.exists(fileName))
  {
    Serial.print(F("file '"));Serial.print(fileName);
    Serial.println(F("' not found\n"));    
    return false;
  }

  if (!file.open(fileName)) {
    Serial.println(F("Error opening file")); 
    return false;
  }
  return true;
}

/*
  SDClose()
    Closes the current open file (or directory). It's just a simple wrap to
    File.close() in the SdFat library.
*/
bool SDClose()
{ 
  return file.close(); 
}

/*
  SDNext()
    Opens the next file available in the current directory. It's just a simple
    wrap to File.openNext() in the SdFat library.
*/
bool SDNext()
{
  return file.openNext(SD.vwd(), O_READ);
}

