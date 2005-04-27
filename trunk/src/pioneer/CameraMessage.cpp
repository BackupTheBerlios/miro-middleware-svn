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

#include "CameraMessage.h"
#include <cstring>

#include <iostream>

namespace Pioneer
{
  CameraAnswer::CameraAnswer() :
    mutex(),
    cond(mutex),
    length_(0)
  {
  }

  void CameraAnswer::init()
  {
    length_ = 0;
  }

  int CameraAnswer::add(const unsigned char *buffer, int length)
  {
    Miro::Guard guard(mutex);
    int n = (length + length_ <= MAX_SIZE)? length : MAX_SIZE - length_;
    memcpy((void *)(buffer_ + length_), (void *)buffer, n);
    length_ += n;
    cond.broadcast();
    return n;
  }

  int CameraAnswer::length() const
  {
    return length_;
  }

  const unsigned char * CameraAnswer::buffer() const
  {
    return buffer_;
  }
}

