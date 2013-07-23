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
#ifndef Can0Message_h
#define Can0Message_h

#include "CanMessage.h"

// extern "C"
// {
#include "canmsg.h"
// }

namespace Can
{

  class Can0Message : public Message
  {
  public:
    Can0Message();
    virtual ~Can0Message();

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

    virtual void canMessage(int ** msg_ )  { *msg_= (int *) message_; }


  protected:
    canmsg * message_;

  };

  inline
  CanId
  Can0Message::id() const {
      return message_->id;
  }

  inline
  void
  Can0Message::id(CanId _id) {
      message_->id = _id;
  }

  inline
  int
  Can0Message::length() const {
      return message_->len;
  }

  inline
  void
  Can0Message::length(int _len) {
      message_->len = _len;
  }

  inline
  char
  Can0Message::charData(int i) const {
      return message_->d[i];
  }

  inline
  void
  Can0Message::charData(int i, char d) {
      message_->d[i] = d;
  }

  inline
  unsigned char
  Can0Message::byteData(int i) const {
      return message_->d[i];
  }

  inline
  void
  Can0Message::byteData(int i, unsigned char d) {
      message_->d[i] = d;
  }

  inline
  short
  Can0Message::shortData(int i) const {
      return (short) ACE_NTOHS(*((const unsigned short *) (&(message_->d[i]))));
  }

  inline
  void
  Can0Message::shortData(int i, unsigned short d)
  {
      *((unsigned short *) (&(message_->d[i]))) = ACE_HTONS(d);
  }

  inline
  long
  Can0Message::longData(int i) const {
      return (long) ACE_NTOHL(*((const unsigned long *) (&(message_->d[i]))));
  }

  inline
  void
  Can0Message::longData(int i, unsigned long d)
  {
      *((unsigned long *) (&(message_->d[i]))) = ACE_HTONL(d);
  }
};
#endif
