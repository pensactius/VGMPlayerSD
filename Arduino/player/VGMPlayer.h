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
#ifndef _VGMPlayer_H_
#define _VGMPlayer_H_

#include "vgmreader.h"
#include "lcd.h"

//#define WAIT60TH 17
//#define WAIT50TH 20

class VGMPlayer
{
public:
  VGMPlayer(LCD *lcd);
  bool read();
  void play();
  //void print(const char *msg, uint8_t col=0, uint8_t row=0);
private:
  VGMReader m_vgm;
  uint8_t m_addr, m_data;
  unsigned long current_time;
  LCD *m_lcd;
};
#endif
