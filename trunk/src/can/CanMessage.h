// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
//
// (c) 1999, 2000, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
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
    virtual int            length() const = NULL;
    virtual void           length(int _len) = NULL;

    virtual char           charData(int i) const = NULL;
    virtual void           charData(int i, char d) = NULL;
    virtual unsigned char  byteData(int i) const = NULL;
    virtual void           byteData(int i, unsigned char d) = NULL;
    virtual short shortData(int i) const = NULL;
    virtual void           shortData(int i, unsigned short d) = NULL; // sets two chars !
    virtual long  longData(int i) const = NULL;
    virtual void           longData(int i, unsigned long d) = NULL;   // sets four chars !

    virtual void           setBuffer(int pos, const char * buffer, int length ) = NULL ;

    virtual CanId          id() const = NULL;
    virtual void           id(CanId _id) = NULL;

    //static drivertype driver;

    virtual void canMessage(int ** msg_) = NULL;
//       std::cout << "normal canMessage" << std::endl;

  protected:

  }; 
}
#endif
