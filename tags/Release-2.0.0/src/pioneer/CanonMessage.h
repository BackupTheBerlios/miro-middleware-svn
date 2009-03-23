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
#ifndef CanonPanTiltMessage_h
#define CanonPanTiltMessage_h

#include "miro/Synch.h"

#include "CameraMessage.h"
#include "CanonDevice.h"

#include <cmath>
/*namespace Miro
  {
  class Mutex;
  }*/

namespace Canon
{
  // forward declarations
  class Message;
  class Answer;

  std::ostream &operator<<(std::ostream &out, const Message& rhs);
  std::ostream &operator<<(std::ostream &out, const Answer& rhs);

  char * int2str(char * buffer, int num, int digits);
  char * int2str(char * buffer, int num1, int num2, int digits);

  int str2int(const char * buffer, int digits);
  void str2int(const char* buffer, int &num1, int &num2, int digits);

  inline int str2int(const unsigned char * buffer, int digits) {
    return str2int((char*)buffer,digits);
  }
  inline void str2int(const unsigned char* buffer, int &num1, int &num2, int digits) {
    str2int((char*)buffer,num1,num2,digits);
  }
  


  class Message : public Pioneer::CameraMessage
  {
  public:
    Message() {} // default get message
    Message(unsigned short cmd, unsigned char byte1=0, unsigned char byte2=0, unsigned char byte3=0);
    Message(unsigned short cmd, char * str);

    unsigned char header() const;
    void header(unsigned char _header);

    const unsigned short deviceNum() const;
    void deviceNum(unsigned short num);

    const int length() const;

    const unsigned short command() const;
    void command(unsigned short cmd);

    const unsigned char * parameter() const;
    unsigned char * parameter();

    const unsigned char * buffer() const;
    unsigned char * buffer();

  protected:

    static const int MAX_MSG_SIZE = 256;
    unsigned char buffer_[MAX_MSG_SIZE];
    int length_;
  }; 

  class Answer : public Pioneer::CameraAnswer
  {
  public:
    unsigned char header() const;
    void header(unsigned char _header);

    const unsigned short deviceNum() const;
    void deviceNum(unsigned short num);

    const unsigned short errorCode() const;
    void errorCode(unsigned short errorCode);

    const unsigned char * parameter() const;

    bool isValid() const;
  };

  /*  class Timer
  {
  public:
    //count is the maximum time in usecs to sleep
    Timer(int count_=500000);

    //sleep usec microseconds and return true if has slept count or more usec
    bool usleep(int usec=10000);
    // (re)set count
    void reset(int count_=500000);

  protected:
    int count;
  };
 
  */
 // inline functions
  inline
  unsigned char 
  Message::header() const {
    return buffer_[0]; 
  }
  inline
  void 
  Message::header(unsigned char _header) {
    buffer_[0] = _header; 
  }
  inline
  const unsigned short 
  Message::deviceNum() const {
    return (buffer_[1]<<8)+buffer_[2];  //return 2 bytes as a short
  }
  inline
  void
  Message::deviceNum(unsigned short num) { 
    buffer_[1]=(unsigned char) (num>>8) & 0x0ff; 
    buffer_[2]=(unsigned char) num & 0x0ff; 
  }
  inline
  const int
  Message::length() const {
    return length_;
  }
  inline
  const unsigned short
  Message::command() const {
    return (buffer_[3]<< 8)+buffer_[4]; //return 2 bytes as a short
  }
  inline
  void
  Message::command(unsigned short cmd) { 
    buffer_[3]=(unsigned char) (cmd>>8) & 0x0ff; 
    buffer_[4]=(unsigned char) cmd & 0x0ff; 
  }
  inline
  const unsigned char *
  Message::parameter() const { 
    return &buffer_[5]; 
  }
  inline
  unsigned char * 
  Message::parameter() { 
    return &buffer_[5]; 
  }
  inline
  const unsigned char *
  Message::buffer() const { 
    return buffer_; 
  }
  inline
  unsigned char * 
  Message::buffer() { 
    return buffer_; 
  }


  inline
  unsigned char 
  Answer::header() const {
    if (length()==0) return 0;
    else return buffer_[0]; 
  }
  inline
  void 
  Answer::header(unsigned char _header) {
    buffer_[0] = _header; 
  }
  inline
  const unsigned short 
  Answer::deviceNum() const {
    if (length()<3) return 0;
    else return (buffer_[1]<<8)+buffer_[2];  //return 2 bytes as a short
  }
  inline
  void
  Answer::deviceNum(unsigned short num) { 
    buffer_[1]=(unsigned char) (num>>8) & 0x0ff; 
    buffer_[2]=(unsigned char) num & 0x0ff; 
  }
  inline
  const unsigned short
  Answer::errorCode() const {
    if (length()<5) return 0;
    return (buffer_[3]<< 8)+buffer_[4]; //return 2 bytes as a short
  }
  inline
  void
  Answer::errorCode(unsigned short errorCode) { 
    buffer_[3]=(unsigned char) (errorCode>>8) & 0x0ff; 
    buffer_[4]=(unsigned char) errorCode & 0x0ff; 
  }
  inline
  const unsigned char *
  Answer::parameter() const { 
    return &buffer_[5]; 
  }
  inline 
  bool 
  Answer::isValid() const {
    if (length() == 0)
      return false;
    return (buffer_[length()-1] == END_MARK);
  }

};
#endif 
