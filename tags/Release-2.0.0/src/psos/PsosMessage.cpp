// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
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
#include "PsosMessage.h"
#include "PsosDevice.h"

#include <iostream>

namespace Psos
{
  std::ostream& operator<< (std::ostream& ostr, const Message& rhs)
  {
    ostr.setf(ios::hex, ios::basefield);
    ostr << "id=";
    ostr.width(2);
    ostr.fill('0');
    ostr << (int)rhs.id() << "  msg= 0x ";
    for (int i = 0; i < rhs.length() + 3; ++i) {
      ostr.width(2);
      ostr.fill('0');
      ostr << (int)(rhs.buffer()[i]) << " ";
    }
    ostr.setf(ios::dec, ios::basefield);
    return ostr;
  }

  Message::Message(char cmd)
  {
    header(START_12);
    length(1 + 2);
    id(cmd);
    checksum(calcChecksum());
  }

  Message::Message(char cmd, const char * msg, int msgLength)
  {
    header(START_12);
    if (msgLength==0) msgLength=strlen(msg);
    length(msgLength + 5);
    id(cmd);
    argType(SF_ARGSTR);
    *userData()=(unsigned char)msgLength;
    memcpy((char *)userData()+1, msg, msgLength);
    checksum(calcChecksum());
  }

  Message::Message(char cmd, short val)
  {
    header(START_12);
    length( 2+ 2 + 2);
    id(cmd);
    argType((val >= 0)? SF_ARGINT : SF_ARGNINT);
    *(short*)userData() = ((val >= 0)? val : -val);
    checksum(calcChecksum());
  }


  Message::Message(char cmd, unsigned short val)
  {
    header(START_12);
    length(2 + 2 + 2);
    id(cmd);
    argType(SF_ARGINT);
    *(unsigned short*)userData() = val;
    checksum(calcChecksum());
  }

  Message::Message(char cmd, char byte1, char byte2)
  {
    header(START_12);
    length(2 + 2 + 2); // cmd/arg + 2 byte integer + 2 byte checksum
    id(cmd);
    argType(SF_ARGINT);
    userData()[0] = byte1;
    userData()[1] = byte2;
    checksum(calcChecksum());
  }

  unsigned short
  Message::calcChecksum() const
  {
    unsigned short cs = 0;
    unsigned short n = (length() - 2) / 2;
    unsigned short * d = (unsigned short *) data();

    for (int i = 0; i < n; ++i)
      cs += ACE_NTOHS(d[i]);
    if ((length() & 1) == 1)
      cs ^= (unsigned short) data()[length()-3];

    return cs;
  }
}
