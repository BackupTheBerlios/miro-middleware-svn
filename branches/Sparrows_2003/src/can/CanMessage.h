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
#ifndef CanMessage_h
#define CanMessage_h

#include "miro/DevMessage.h"

// extern "C"
// {
#include "canmsg.h"
// }

namespace Can
{
  // forward declarations
  class Message;

  std::ostream& operator<< (std::ostream& ostr, const Message& rhs);

  class Message : public Miro::DevMessage
  {
  public:
    int            length() const   { return message_.len; }
    void           length(int _len) { message_.len = _len; }

    char           charData(int i) const { return message_.d[i]; }
    void           charData(int i, char d) { message_.d[i] = d; }
    unsigned char  byteData(int i) const { return message_.d[i]; }
    void           byteData(int i, unsigned char d) { message_.d[i] = d; }
    short shortData(int i) const;
    void           shortData(int i, unsigned short d); // sets two chars !
    long  longData(int i) const;
    void           longData(int i, unsigned long d);   // sets four chars !

    void           setBuffer(int pos, const char * buffer, int length ) ;

    CanId          id()  const     { return message_.id; }
    void           id(CanId _id)   { message_.id = _id; }

    const canmsg* canMessage() const { return &message_; }
    canmsg* canMessage() { return &message_; }
  protected:
    canmsg message_;
  };

  inline
  short
  Message::shortData(int i) const { 
    return (short) ACE_NTOHS(*((const unsigned short *) (&message_.d[i])));
  }
  inline
  void 
  Message::shortData(int i, unsigned short d) 
  { 
    *((unsigned short *) (&message_.d[i])) = ACE_HTONS(d);
  }
  inline
  long
  Message::longData(int i) const { 
    return (long) ACE_NTOHL(*((const unsigned long *) (&message_.d[i])));
  }
  inline
  void 
  Message::longData(int i, unsigned long d) 
  { 
    *((unsigned long *) (&message_.d[i])) = ACE_HTONL(d);
  }
};
#endif 
