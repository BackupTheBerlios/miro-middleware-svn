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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#ifndef McpMessage_h
#define McpMessage_h

#include <cmath>
#include <ace/OS.h>

#include "miro/DevMessage.h"

namespace Mcp
{
  // forward declarations
  class Message;

  std::ostream& operator << (std::ostream& ostr, const Message& rhs);

  struct Message : public Miro::DevMessage
  {
    static const int SIZE = 256;
    
    unsigned char buffer_[SIZE];

    unsigned char * buffer() { return buffer_; }
    const unsigned char * buffer() const { return buffer_; }

    unsigned char opcode() const { return buffer_[1]; }
    unsigned char size() const { return buffer_[0]; }
    unsigned long value() const { 
      if (size() >= 5) {
	return ACE_NTOHL(*((unsigned long*) &buffer_[2])); 
      }
      if (size() == 3) {
	return ACE_NTOHS((*((unsigned short*) &buffer_[2]))); 
      }
      return ACE_NTOHL(buffer_[2]); 
    }
    const unsigned char * message() const { return &buffer_[2]; }
  };

  struct OutMessage
  {
    static const int SIZE = 6;
    unsigned char buffer[SIZE];

  public:
    OutMessage(unsigned char _opcode, unsigned long _parameter)
    {
      buffer[0] = _opcode;
      *((unsigned long*) &buffer[1]) =  ACE_HTONL(_parameter);
      checksum();
    }

    unsigned char opcode() const { return buffer[0]; }
    void opcode(unsigned char _opcode) { 
      buffer[0] = _opcode; 
      checksum();
    }
    unsigned long parameter() const { 
      return ACE_NTOHL(*((unsigned long*) &buffer[1])); 
    }
    void paramter(unsigned long _parameter) { 
      *((unsigned long*) &buffer[1]) = ACE_HTONL(_parameter);
      checksum();
    }

  protected:
    // calculate checksum
    void checksum() {
      buffer[5] = buffer[0] ^ buffer[1] ^ buffer[2] ^ buffer[3] ^ buffer[4];
    }
  };
};


#endif






