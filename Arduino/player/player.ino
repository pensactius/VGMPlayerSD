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
#include "config.h"
#include "sdcard.h"
#include "vgmreader.h"
#include "VGMPlayer.h"
#include "sn76489.h"

/*
    Reads VGM (Video Game Music) files stored in the SD card and plays
    uncompressed vgm files to the supported audio chips. For now, it only 
    reads a single directory, specified below and does not read subdirectories.

    As stated before, it only supports uncompressed VGM files. For now, the only audio
    chip supported is the SN-76489 (included in the Sega Master System console).

    The following boards are supported:

      - ARDUINO MEGA
      - ARDUINO DUE

    Please, make sure you uncomment the corresponding line in "config.h" to compile for
    your board.

    Different boards use different pinout for connections. Please, make sure you follow
    the pinout for your board as described in the file "pinout.txt".

    This project requires the SdFat library installed. You can download the latest 
    version from:

      https://github.com/greiman/SdFat

    Step by step compilation process:

      1. Make sure you follow the pinout for your board as described in "pinout.txt"
      2. Uncomment the corresponding line in "config.h" according to your board.
      3. Set the constant VGM_DIR -line 20 in "config.h", to the directory where your 
         VGM files are stored.
      4. Install the SdFat library from "https://github.com/greiman/SdFat"
*/

void setup() 
{  

  // Open serial communication and wait for port to open:
  Serial.begin(9600);

  // Initialize button pins
  pinMode (LOOP_PIN, INPUT_PULLUP);
  pinMode (NEXT_PIN, INPUT_PULLUP);
  
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  // Try to read the SD card and "chdir" to VGM_DIR
  SDInit(VGM_DIR);

  // If the above was successful create a new VGMPlayer object to play
  // all the vgm files in VGM_DIR
  VGMPlayer vgmPlayer;

  // Main loop: open next file in VGM_DIR, and if it's a supported vgm
  // file play it, close the file (or directory) and continue to next file.
  // Keep doing it until it reaches the last file in VGM_DIR directory.
  while (SDNext())
  {
    if (vgmPlayer.read())    
    { 
      delay(500);      
      vgmPlayer.play();
      SN76489_Off();       
    }
    SDClose();
  }
  Serial.println(F("Done!"));
}

/*
    Nothing to do in loop(), everything is handled in setup()
 */
void loop() {}