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
    Message();
    virtual int            length() const;
    virtual void           length(int _len);

    virtual char           charData(int i) const;
    virtual void           charData(int i, char d);
    virtual unsigned char  byteData(int i) const;
    virtual void           byteData(int i, unsigned char d);
    virtual short shortData(int i) const;
    virtual void           shortData(int i, unsigned short d); // sets two chars !
    virtual long  longData(int i) const;
    virtual void           longData(int i, unsigned long d);   // sets four chars !

    virtual void           setBuffer(int pos, const char * buffer, int length ) ;

    virtual CanId          id() const;
    virtual void           id(CanId _id);

    //static drivertype driver;

    virtual void canMessage(int ** msg_) {
//       std::cout << "normal canMessage" << std::endl;
    
    };



  protected:

  };

  inline
  CanId
  Message::id() const {

  }

  inline
  void
  Message::id(CanId _id) {

  }

  inline
  int
  Message::length() const {

  }

  inline
  void
  Message::length(int _len) {

  }

  inline
  char
  Message::charData(int i) const {

  }

  inline
  void
  Message::charData(int i, char d) {

  }

  inline
  unsigned char
  Message::byteData(int i) const {

  }

  inline
  void
  Message::byteData(int i, unsigned char d) {

  }

  inline
  short
  Message::shortData(int i) const {

  }

  inline
  void
  Message::shortData(int i, unsigned short d)
  {

  }

  inline
  long
  Message::longData(int i) const {

  }

  inline
  void
  Message::longData(int i, unsigned long d)
  {

  }
};
#endif
