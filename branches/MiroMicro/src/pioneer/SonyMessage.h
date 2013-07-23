// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
//
// Authors: 
//   Ingo van Lil
//
#ifndef SonyMessage_h
#define SonyMessage_h

#include "CameraMessage.h"

namespace Sony
{
  class Message : public Pioneer::CameraMessage
  {
  public:
    Message(unsigned char receiver = 1);
    const int length() const;
    const unsigned char * buffer() const;
    void panTilt(unsigned short panpos, unsigned short tiltpos);
    void zoom(unsigned short value);
    void focus(unsigned short value);
    void autofocus(bool enable);
    void shutter(unsigned char value);
    void autoexposure(unsigned char mode);
    void iris(unsigned char value);
    void gain(unsigned char value);

  protected:
    static const int MAX_MSG_SIZE = 16;
    unsigned char buffer_[MAX_MSG_SIZE];
    int length_;
  };

  inline const int Message::length() const {return length_;}
  inline const unsigned char * Message::buffer() const {return buffer_;}
}

#endif /* SonyMessage_h */
