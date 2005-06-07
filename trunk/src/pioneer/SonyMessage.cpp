/*
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id$
 */

#include "SonyMessage.h"

namespace Sony
{
  Message::Message(unsigned char receiver)
  {
    length_ = 0;
    buffer_[0] = 0x80 + receiver;
    buffer_[1] = 0x01;
  }

  void Message::panTilt(unsigned short panpos, unsigned short tiltpos)
  {
    length_ = 15;
    buffer_[2] = 0x06;
    buffer_[3] = 0x02;
    buffer_[4] = 0x18;
    buffer_[5] = 0x14;
    buffer_[6] = (panpos>>12) & 0x0f;
    buffer_[7] = (panpos>> 8) & 0x0f;
    buffer_[8] = (panpos>> 4) & 0x0f;
    buffer_[9] = panpos & 0x0f;
    buffer_[10] = (tiltpos>>12) & 0x0f;
    buffer_[11] = (tiltpos>> 8) & 0x0f;
    buffer_[12] = (tiltpos>> 4) & 0x0f;
    buffer_[13] = tiltpos & 0x0f;
    buffer_[14] = 0xff;
  }

  void Message::zoom(unsigned short value)
  {
    length_ = 9;
    buffer_[2] = 0x04;
    buffer_[3] = 0x47;
    buffer_[4] = (value>>12) & 0x0f;
    buffer_[5] = (value>> 8) & 0x0f;
    buffer_[6] = (value>> 4) & 0x0f;
    buffer_[7] = value & 0x0f;
    buffer_[8] = 0xff;
  }

  void Message::focus(unsigned short value)
  {
    length_ = 9;
    buffer_[2] = 0x04;
    buffer_[3] = 0x48;
    buffer_[4] = (value>>12) & 0x0f;
    buffer_[5] = (value>> 8) & 0x0f;
    buffer_[6] = (value>> 4) & 0x0f;
    buffer_[7] = value & 0x0f;
    buffer_[8] = 0xff;
  }

  void Message::autofocus(bool enable)
  {
    length_ = 6;
    buffer_[2] = 0x04;
    buffer_[3] = 0x38;
    buffer_[4] = enable? 0x02 : 0x03;
    buffer_[5] = 0xff;
  }

  void Message::shutter(unsigned char value)
  {
    length_ = 9;
    buffer_[2] = 0x04;
    buffer_[3] = 0x4a;
    buffer_[4] = 0x00;
    buffer_[5] = 0x00;
    buffer_[6] = (value>> 4) & 0x0f;
    buffer_[7] = value & 0x0f;
    buffer_[8] = 0xff;
  }

  void Message::autoexposure(unsigned char mode)
  {
    length_ = 6;
    buffer_[2] = 0x04;
    buffer_[3] = 0x39;
    buffer_[4] = mode;
    buffer_[5] = 0xff;
  }

  void Message::iris(unsigned char value)
  {
    length_ = 9;
    buffer_[2] = 0x04;
    buffer_[3] = 0x4b;
    buffer_[4] = 0x00;
    buffer_[5] = 0x00;
    buffer_[6] = (value>> 4) & 0x0f;
    buffer_[7] = value & 0x0f;
    buffer_[8] = 0xff;
  }

  void Message::gain(unsigned char value)
  {
    length_ = 9;
    buffer_[2] = 0x04;
    buffer_[3] = 0x4c;
    buffer_[4] = 0x00;
    buffer_[5] = 0x00;
    buffer_[6] = (value>> 4) & 0x0f;
    buffer_[7] = value & 0x0f;
    buffer_[8] = 0xff;
  }
}
