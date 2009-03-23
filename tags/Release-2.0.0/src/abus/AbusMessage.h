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
#ifndef AbusMessage_h
#define AbusMessage_h

#include "miro/DevMessage.h"

#include "AbusDevice.h"

namespace Abus 
{
  static const int MSG_DATA_LEN = 128;

  //----------------------------------------------------------------------------
  // Composite message types
  //----------------------------------------------------------------------------
  struct Message : public Miro::DevMessage
  {
    static const int MSG_LEN = (MSG_DATA_LEN + 4 * sizeof(unsigned char));

    static const int MINOR_OFFSET  = 0;
    static const int MAJOR_OFFSET  = 1;
    static const int DEVID_OFFSET  = 2;
    static const int MSGLEN_OFFSET = 3;

    unsigned char _buffer[MSG_LEN];

    Message() {}
    Message(unsigned char majorOp_, unsigned char minorOp_,
	    unsigned char devId_, unsigned char msgLen_ = 0) {
      majorOp() = majorOp_;
      minorOp() = minorOp_;
      devId()   = devId_;
      msgLen()  = msgLen_;
    }

    const         char* buffer() const { return (char*)_buffer; }
                  char* buffer()       { return (char*)_buffer; }

    unsigned char majorOp() const { return _buffer[MAJOR_OFFSET]; }
    unsigned char minorOp() const { return _buffer[MINOR_OFFSET]; }
    unsigned char devId()   const { return _buffer[DEVID_OFFSET]; }
    unsigned char msgLen()  const { return _buffer[MSGLEN_OFFSET]; }

    unsigned char* message() { return &_buffer[4]; }

    unsigned char& majorOp() { return _buffer[MAJOR_OFFSET]; }
    unsigned char& minorOp() { return _buffer[MINOR_OFFSET]; }
    unsigned char& devId()   { return _buffer[DEVID_OFFSET]; }
    unsigned char& msgLen()  { return _buffer[MSGLEN_OFFSET]; }

    const unsigned char* message() const { return &_buffer[4]; }
  };

  struct BusMessage
  {
    static const int MSG_LEN = (MSG_DATA_LEN + 3 * sizeof(unsigned char) + 1);

    unsigned char buffer[MSG_LEN];

    unsigned char&  dest() { return buffer[0]; }
    unsigned char&  src()  { return buffer[1]; }
    unsigned char&  len()  { return buffer[2]; } // length with protocol

    unsigned char * message()       { return &buffer[4]; }
  };

  //
  // Long Id
  // Used for M2D_LINK_REPLY (0x40) and M2D_LONG_ID (0x41)
  struct LongIdMessage : Message
  {
    LongIdMessage(unsigned char majorOp_, unsigned char minorOp_, unsigned char devId_) :
      Message(majorOp_, minorOp_, devId_, sizeof(DeviceLongId)) 
    {};

          DeviceLongId& longId()       { return *(DeviceLongId*) message(); }
    const DeviceLongId& longId() const { return *(DeviceLongId*) &_buffer[4]; }
  };

  //
  // Dev Id
  // Used for M2D_ID (0x42)
  struct IdMessage : Message
  {
    IdMessage() {}
    IdMessage(unsigned char majorOp_, unsigned char minorOp_,
	      unsigned char devId_) :
      Message(majorOp_, minorOp_, devId_, sizeof(DeviceId)) 
    {};

    DeviceId& devId() { return *(DeviceId*) message(); }
  };

  //
  // Dev Type
  // Used for D2M_LINK_REQ (0x21) and M2D_TYPE (0x43)
  struct DeviceTypeMessage : Message
  {
    DeviceTypeMessage() {}
    DeviceTypeMessage(unsigned char majorOp_, unsigned char minorOp_,
		   unsigned char devId_) :
      Message(majorOp_, minorOp_, devId_, sizeof(DeviceProtocol))
    {};

    DeviceProtocol& devType() { return *(DeviceProtocol*) message(); }
  };
}; // namespace Abus

#endif






