#ifndef _VGMREADER_H_
#define _VGMREADER_H_
#include <Arduino.h>
#include "config.h"

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
  bool read();
  void dumpHeader();
  void getVersion(uint8_t &major, uint8_t &minor);
  uint16_t readData(uint32_t offset);
  uint8_t nextByte();
  uint32_t getCursor();
  uint32_t getDataLength();
  void loop();

private:
  bool parseHeader();
  void parseGD3Info();

  VGMHeader m_header;
  uint32_t  m_vgmDataOffset;
  uint32_t  m_dataLength;    // # bytes in vgm data
  uint8_t   m_buf[BUFSIZE];

  uint16_t  m_bufCursor;    // position in buffer
  uint32_t  m_fileCursor;   // position in whole file
};

#endif
