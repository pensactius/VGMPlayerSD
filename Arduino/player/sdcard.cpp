#include <SPI.h>
//#include <SD.h>
#include <SdFat.h>
#include "sdcard.h"

SdFat SD;
File file;

bool SDInit(const char *dirName)
{
Serial.print(F("\nInitializing SD card...\n"));

  if(!SD.begin(chipSelect,SD_SCK_MHZ(50)))
  {
    SD.initErrorHalt();
    /*Serial.println(F("initialization failed. Things to check:"));
    Serial.println(F("* is a card inserted?"));
    Serial.println(F("* is your wiring correct?"));
    Serial.println(F("* did you change the chipSelect pin to match your shield or module?"));
    return false;*/
  } else {
    Serial.println(F("Wiring is correct and a card is present."));
    Serial.println(F("initialization done."));   

    // Set volume working directory vwd to root.
    SD.chdir(dirName, true);
    //SD.vwd()->rewind(); 
    return true;
  }  
}

bool SDOpen(const char *fileName)
{
  Serial.print(F("\nOpening file "));Serial.println(fileName);
  if (!SD.exists(fileName))
  {
    Serial.print(F("file '"));Serial.print(fileName);
    Serial.println(F("' not found\n"));    
    return false;
  }
  else {
//    Serial.print(F("Found it!\n"));
  }
  if (!file.open(fileName)) {
    Serial.println(F("Error opening file")); 
    return false;
  }
  return true;
}

bool SDClose()
{ 
  return file.close(); 
}

bool SDNext()
{
  return file.openNext(SD.vwd(), O_READ);
}

