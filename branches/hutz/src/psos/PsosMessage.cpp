// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


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

  Message::Message(char cmd, const char * msg)
  {
    header(START_12);
    length(strlen(msg) + 2);
    id(cmd);
    type(SF_ARGSTR);
    strcpy((char *)userData(), msg);
    userData()[strlen(msg)] = '\0'; // 0 termination
    checksum(calcChecksum());
  }

  Message::Message(char cmd, short val)
  {
    header(START_12);
    length( 2+ 2 + 2);
    id(cmd);
    type((val >= 0)? SF_ARGINT : SF_ARGNINT);
    *(short*)userData() = ((val >= 0)? val : -val);
    checksum(calcChecksum());
  }


  Message::Message(char cmd, unsigned short val)
  {
    header(START_12);
    length(2 + 2 + 2);
    id(cmd);
    type(SF_ARGINT);
    *(unsigned short*)userData() = val;
    checksum(calcChecksum());
  }

  Message::Message(char cmd, char byte1, char byte2)
  {
    header(START_12);
    length(1 + 2 + 2);
    id(cmd);
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
};
