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
#ifndef SparrowDevice_h
#define SparrowDevice_h

#include "miro/Angle.h"
#include <cmath>

// Roland:
// laesst sich diese Datei mit SparrowDevice.h mergen ???
// ansonsten sind hier ein haufen definitionen dupliziert ...

namespace Sparrow
{
  // types of timers registerd for Sparrow::EventHandler
  typedef enum { BUTTONS_TIMER, STALL_TIMER, INIT_TIMER} TimerType;

  //--------------------------------------------------------------------------
  // Motion status
  //--------------------------------------------------------------------------
  typedef enum { LIMP, SPEED, SPEED_ROT, POWER } StateType;

  static const int ACCEL_TABLE_SIZE = 648;

  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------

  static const unsigned long NUMBER_OF_BUTTONS = 2;

  static const int SERVO_FACTOR = 27;

  inline
  unsigned short rad2ticks(double rad) {
    int ticks = (int)rint(rad * 512. / M_PI);
    ticks = (ticks < 0)? -((-ticks)  % 1024) : ticks % 1024;

    return (unsigned short) ticks;
  }

  inline
  double ticks2rad(unsigned short ticks) {
    long h = ticks % 1024;
    if (h > 512)
      h -= 1024;
    return M_PI/512. * (double) h;
  }

  //--------------------------------------------------------------------------
  // Message CAN-IDs
  //--------------------------------------------------------------------------

  // note, that the leading 0x8 is the XID-identifer
  // as defined in canmsg.h

  // motor messages


  static const unsigned long CAN_WRITE_MOTOR_LEFT = 0x05010102;
  static const unsigned long CAN_WRITE_MOTOR_RIGHT = 0x04010102;
  static const unsigned long CAN_MOTOR_TICKS_LEFT = 0x85020201;
  static const unsigned long CAN_MOTOR_TICKS_RIGHT = 0x84020201;
  static const unsigned long CAN_R_MOTOR_ALIVE = 0x8f020201;


  // port messages

  static const unsigned long CAN_READ_PORTS    = 0x81030102;


  // kicker messages

  static const unsigned long CAN_KICK          = 0x81010103;
  static const unsigned long CAN_R_KICK_ALIVE  = 0x9e0f0301;

  // servo messages

  static const unsigned long CAN_SERVO_GO      = 0x81010104;
  static const unsigned long CAN_R_PAN_ALIVE      = 0x9e0f0401;

  // infrared messages


  static const unsigned long CAN_R_IR_GET_CONT1 = 0x83810501;
  static const unsigned long CAN_R_IR_ALIVE1    = 0x9e0f0501;
  static const unsigned long CAN_IR_SET_FREQ1    = 0x02010105;


  static const unsigned long CAN_R_IR_GET_CONT2 = 0x83810601;
  static const unsigned long CAN_R_IR_ALIVE2    = 0x9e0f0601;
  static const unsigned long CAN_IR_SET_FREQ2    = 0x02010106;






};
#endif
