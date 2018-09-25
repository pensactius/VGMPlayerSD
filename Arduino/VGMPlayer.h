#ifndef _VGMPlayer_H_
#define _VGMPlayer_H_

#include "vgmreader.h"

#define WAIT60TH 1000 / 60 // delay in ms for 60th frame of a second
#define WAIT50TH 1000 / 50 // delay in ms for 60th frame of a second

class VGMPlayer
{
public:
  VGMPlayer();
  bool read();
  void play();
private:
  VGMReader m_vgm;
  uint8_t m_addr, m_data;
};
#endif
