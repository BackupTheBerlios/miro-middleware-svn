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
#ifndef CanMessage_h
#define CanMessage_h

#include "miro/DevMessage.h"
#include <iostream>

// extern "C"
// {
#include "canmsg.h"
#include "pcan.h"
// }

namespace Can
{
  enum drivertype { OLD, PCAN, UNINITIALIZED };



  // forward declarations
  class Message;


  std::ostream& operator<< (std::ostream& ostr, const Message& rhs);

  class Message : public Miro::DevMessage
  {
  public:
    Message() {};
    virtual ~Message();
    virtual int            length() const = 0;
    virtual void           length(int _len) = 0;

    virtual char           charData(int i) const = 0;
    virtual void           charData(int i, char d) = 0;
    virtual unsigned char  byteData(int i) const = 0;
    virtual void           byteData(int i, unsigned char d) = 0;
    virtual short shortData(int i) const = 0;
    virtual void           shortData(int i, unsigned short d) = 0; // sets two chars !
    virtual long  longData(int i) const = 0;
    virtual void           longData(int i, unsigned long d) = 0;   // sets four chars !

    virtual void           setBuffer(int pos, const char * buffer, int length ) = 0 ;

    virtual CanId          id() const = 0;
    virtual void           id(CanId _id) = 0;

    //static drivertype driver;

    virtual void canMessage(int ** msg_) = 0;
//       std::cout << "normal canMessage" << std::endl;

  protected:

  }; 
}
#endif
