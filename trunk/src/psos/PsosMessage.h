// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PsosMessage_h
#define PsosMessage_h

#include "miro/DevMessage.h"

#include "PsosDevice.h"

#include <cmath>

namespace Psos
{
  // forward declarations
  class Message;

  std::ostream &operator<<(std::ostream &out, const Message& rhs);

  class Message : public Miro::DevMessage
  {
  public:
    Message() {} // default get message
    Message(char cmd);
    Message(char cmd, const char * msg, int msgLength=0);
    explicit Message(char cmd, short val);
    explicit Message(char cmd, unsigned short val);
    explicit Message(char cmd, char byte1, char byte2);

    unsigned short header() const;
    void header(unsigned short _header);

    unsigned char length() const;
    void length(unsigned char len);

    const unsigned char * data() const;
    unsigned char * data();

    unsigned char id() const;
    void id(char cmd);

    unsigned char argType() const;
    void argType(char _type);

    const unsigned char * userData() const;
    unsigned char * userData();

    const unsigned char * buffer() const;
    unsigned char * buffer();

    unsigned short checksum() const;
    void checksum(unsigned short _checksum);
    bool validChecksum() const;

  protected:
    unsigned short calcChecksum() const;

    static const int MAX_MSG_SIZE = 256;
    unsigned char buffer_[MAX_MSG_SIZE];
  }; 

  class ServerInfoMessage : public Message
  {
  public:
    unsigned char status() const { return buffer_[3] & 0x0f; }
    short xPos() const { return (*(unsigned short *)(&buffer_[4])) & 0x7fff; }
    short yPos() const { return (*(unsigned short *)(&buffer_[6])) & 0x7fff; }
    short theta() const { return *(short *)(&buffer_[8]) & 4095; }
    short lVel() const { return *(short *)(&buffer_[10]); }
    short rVel() const { return *(short *)(&buffer_[12]); }
    double battery() const { return (double)buffer_[14] / 10.; }
    unsigned short bumpers() const { return *(unsigned short *)(&buffer_[15]); }
    unsigned char lBumper() const { return buffer_[15] & 1; }
    unsigned char rBumper() const { return buffer_[16] & 1; }
    short control() const { return *(short *)(&buffer_[17]); }
    unsigned short PTU() const { return *(unsigned short *)(&buffer_[19]); }
    unsigned short flags() const { return *(unsigned short *)(&buffer_[19]); }
    unsigned char say() const { return buffer_[21]; }

    int sonarReadings() const { return buffer_[22]; }
    int sonarNumber(int index) const { return buffer_[23 + index * 3]; }
    short sonarValue(int index) const { 
      return (short) rint((*(unsigned short*)&buffer_[24 + index * 3]));
    }
  };

  class IOpacMessage : public Message
  {
  public:
    unsigned char digin() const { return buffer_[5]; }
    unsigned char frontbump() const { return buffer_[6]; }
    unsigned char rearbump() const { return buffer_[7]; }
    unsigned char ir() const  { return buffer_[8]; }

    unsigned char digout() const  { return buffer_[10]; }
    unsigned char an() const { return buffer_[11]; }
//     unsigned short const * ad() const  { 
//       (unsigned short const *)(&buffer_[12]); 
//    }
  };
  
  // inline functions
  inline
  unsigned short 
  Message::header() const {
    return *(unsigned short*)buffer_; 
  }
  inline
  void 
  Message::header(unsigned short _header) {
    *(unsigned short*)buffer_ = _header; 
  }
  inline
  unsigned char 
  Message::length() const { 
    return buffer_[2]; 
  }
  inline
  void 
  Message::length(unsigned char len) { 
    buffer_[2] = len; 
  }
  inline
  const unsigned char * 
  Message::data() const {
    return &buffer_[3]; 
  }
  inline
  unsigned char * 
  Message::data() { 
    return &buffer_[3]; 
  }
  inline
  unsigned char
  Message::id() const {
    return buffer_[3]; 
  }
  inline
  void
  Message::id(char cmd) { 
    buffer_[3] = cmd; 
  }
  inline
  unsigned char
  Message::argType() const {
    return buffer_[4]; 
  }
  inline
  void
  Message::argType(char _type) { 
    buffer_[4] = _type; 
  }
  inline
  const unsigned char *
  Message::userData() const { 
    return &buffer_[5]; 
  }
  inline
  unsigned char * 
  Message::userData() { 
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
  unsigned short
  Message::checksum() const { 
    return ACE_NTOHS(*((unsigned short*) &data()[length()-2])); 
  }
  inline
  void 
  Message::checksum(unsigned short _checksum) {     
    *((unsigned short*)(&data()[length()-2])) = ACE_HTONS(_checksum);
  };
  inline
  bool
  Message::validChecksum() const { 
    return (checksum() == calcChecksum()); 
  }
};
#endif 
