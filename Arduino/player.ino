#include "config.h"
#include "sdcard.h"
#include "vgmreader.h"
#include "vgmplayer.h"
#include "sn76489.h"
/*
#define NUM_SONGS 8

char *songs[NUM_SONGS] = {
  "songs/Lemming1",
  "songs/Lemming7",
  "songs/Aladdin (GG) - 06 - BGM 06.vgm",
  "Sonic The Hedgehog - 01 - Title Screen",
  "Sonic The Hedgehog - 02 - Act Start",
  "Sonic The Hedgehog - 03 - Green Hill Zone",
  "songs/Sonic The Hedgehog/Sonic The Hedgehog - 16 - Marble Zone (unused)",  
  "songs/DuckTales"
};
*/

void setup() {
  // put your setup code here, to run once:
  // Open serial communication and wait for port to open:
  Serial.begin(9600);

  // Initialize button pins
  pinMode (LOOP_PIN, INPUT_PULLUP);
  pinMode (NEXT_PIN, INPUT_PULLUP);
  
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  SDInit("songs/Sonic The Hedgehog");
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
