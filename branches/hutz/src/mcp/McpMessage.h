// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef McpMessage_h
#define McpMessage_h

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






