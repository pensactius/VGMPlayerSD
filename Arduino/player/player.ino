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

    Step by step compilation process:

      1. Make sure you follow the pinout for your board as described in "pinout.txt"
      2. Uncomment the corresponding line in "config.h" according to your board.
      3. Set the constant VGM_DIR -line 20 in "config.h", to the directory where your 
         VGM files are stored.
 
*/

void setup() {  
  // Open serial communication and wait for port to open:
  Serial.begin(9600);

  // Initialize button pins
  pinMode (LOOP_PIN, INPUT_PULLUP);
  pinMode (NEXT_PIN, INPUT_PULLUP);
  
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  SDInit(VGM_DIR);
  VGMPlayer vgmPlayer;
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

void loop() {
  
}
