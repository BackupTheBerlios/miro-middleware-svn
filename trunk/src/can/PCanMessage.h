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
#ifndef PCanMessage_h
#define PCanMessage_h

#include "CanMessage.h"

namespace Can
{





  class PCanMessage : public Message
  {
  public:
    PCanMessage();
    int            length() const;
    void           length(int _len);

    char           charData(int i) const;
    void           charData(int i, char d);
    unsigned char  byteData(int i) const;
    void           byteData(int i, unsigned char d);
    short shortData(int i) const;
    void           shortData(int i, unsigned short d); // sets two chars !
    long  longData(int i) const;
    void           longData(int i, unsigned long d);   // sets four chars !

    void           setBuffer(int pos, const char * buffer, int length ) ;

    CanId          id() const;
    void           id(CanId _id);

    
    virtual void canMessage(int ** msg_) { *msg_ = (int *) messagep_; };// { *msg_=messagep_; }

  protected:
    pcanmsg * messagep_;
  };

  inline
  CanId
  PCanMessage::id() const {
      return messagep_->msg.id;
  }

  inline
  void
  PCanMessage::id(CanId _id) {
      messagep_->msg.id=_id;
  }

  inline
  int
  PCanMessage::length() const {
     return messagep_->msg.len;
  }

  inline
  void
  PCanMessage::length(int _len) {
      messagep_->msg.len=_len;
  }

  inline
  char
  PCanMessage::charData(int i) const {
     return messagep_->msg.data[i];
  }

  inline
  void
  PCanMessage::charData(int i, char d) {
      messagep_->msg.data[i]=d;
  }

  inline
  unsigned char
  PCanMessage::byteData(int i) const {
      return messagep_->msg.data[i];
  }

  inline
  void
  PCanMessage::byteData(int i, unsigned char d) {
      messagep_->msg.data[i]=d;
  }

  inline
  short
  PCanMessage::shortData(int i) const {
      return (short) ACE_NTOHS(*((const unsigned short *) (&(messagep_->msg.data[i]))));
  }

  inline
  void
  PCanMessage::shortData(int i, unsigned short d)
  {
      *((unsigned short *) (&(messagep_->msg.data[i]))) = ACE_HTONS(d);
  }

  inline
  long
  PCanMessage::longData(int i) const {
      return (long) ACE_NTOHL(*((const unsigned long *) (&(messagep_->msg.data[i]))));
  }

  inline
  void
  PCanMessage::longData(int i, unsigned long d)
  {
      *((unsigned long *) (&(messagep_->msg.data[i]))) = ACE_HTONL(d);
  }
};
#endif
