// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MspMessage_h
#define MspMessage_h

#include "MspDevice.h"

#include "abus/AbusMessage.h"
#include "abus/AbusDevice.h"

namespace Msp 
{
  struct Message : public Abus::Message
  {
    Message() {}
    Message(unsigned char devId, unsigned char minor, unsigned short msgLen = 2) : 
      Abus::Message(Abus::D2M_MSG_TO_DEVICE, 1, devId, msgLen)
    {
      mspMajorOp() = AB_MSP_OPCODE;
      mspMinorOp() = minor;
    }

    unsigned char mspMajorOp() const { return message()[0]; }
    unsigned char mspMinorOp() const { return message()[1]; }
    
    const unsigned char * mspData() const { return &message()[2]; }
    unsigned char * mspData() { return &message()[2]; }

    unsigned char  charData(int index) const { return _buffer[index + 6]; }
    unsigned short shortData(int index) const { return ntohs(*((unsigned short*)&_buffer[6] + index)); }
    unsigned long  longData(int index) const { return ntohl(*((unsigned long*)&_buffer[6] + index)); }
    
    void setCharData (int index, unsigned char data)  { _buffer[6 + index] = data; }
    void setShortData(int index, unsigned short data) { *(((unsigned short*) &_buffer[6] + index)) = htons(data); }
    void setLlongData(int index, unsigned long data)  { *(((unsigned long*)  &_buffer[6] + index)) = htonl(data); }
    
  protected:
    unsigned char& mspMajorOp() { return message()[0]; }
    unsigned char& mspMinorOp() { return message()[1]; }
  };

  static const unsigned char IR_PARM_INTERVAL = 1;

  static const unsigned char SONAR_START = 1;
  static const unsigned char SONAR_STOP  = 2;

  static const int SONAR_FIRE_INTERVAL  = 1;
  static const int SONAR_ECHO_NUMBER    = 2;
  static const int SONAR_ECHO_TIMEOUT   = 3;
  static const int SONAR_INIT_BLNK_TIME = 4;
  static const int SONAR_ECHO_BLNK_TIME = 5;
  static const int SONAR_START_DELAY    = 6;
  static const int SONAR_FIRE_DELAY     = 7;

  struct SonarRplMessage : public Message
  {
    unsigned short sonars() const { 
      return (msgLen() - 4) / ((echos() +1) * sizeof(short)); 
    }
    unsigned short echos() const {
      return shortData(0); 
    }
    unsigned short sonarId(int index) const {
      return shortData(1 + index * (1 + echos())); 
    }
    unsigned short sonarEcho(int index, int echo) const { 
      return shortData((1 + index * (1 + echos())) + echo + 1); 
    }
  };
}
     
#endif // mspMessage_hh




