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

namespace Sparrow
{
  // types of timers registerd for Sparrow::EventHandler
  typedef enum { BUTTONS_TIMER, STALL_TIMER, INIT_TIMER } TimerType;

  //--------------------------------------------------------------------------
  // Motion status
  //--------------------------------------------------------------------------
  typedef enum { LIMP, SPEED, SPEED_ROT, POWER } StateType;

  static const int ACCEL_TABLE_SIZE = 648;

  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------

  static const int NUMBER_OF_BUTTONS = 2;

  static const int SERVO_FACTOR = 27;
  
  inline
  unsigned short rad2ticks(double rad) { 
    int ticks = (int)rint(rad * 512. / M_PI);
    ticks = (ticks < 0)? -((-ticks)  % 1024) : ticks % 1024;

    return (unsigned short) ticks; 
  }

  inline
  double ticks2rad(unsigned short ticks) { 
    return ((double)(ticks % 1024)) * M_PI / 512.;

  }

  //--------------------------------------------------------------------------
  // Message CAN-IDs
  //--------------------------------------------------------------------------

  // note, that the leading 0x8 is the XID-identifer
  // as defined in canmsg.h

  // motor messages

  static const unsigned long CAN_SET_POWER     = 0x81010102;
  static const unsigned long CAN_SET_SPEED     = 0x82010102;
  static const unsigned long CAN_SET_SPEED_ROT = 0x83010102;

  // preliminary values and message names
  static const unsigned long CAN_SET_ACCELS    = 0x89010102;
  static const unsigned long CAN_GET_ACCELS    = 0x88010102;
  static const unsigned long CAN_R_GET_ACCELS  = 0x88810201;

  static const unsigned long CAN_INIT_DRIVE    = 0x8a010102;
  static const unsigned long CAN_INIT_MAX      = 0x8b010102;
  static const unsigned long CAN_MOTOR_INIT    = 0x8c010102;
  static const unsigned long CAN_MOTOR_ALL_OFF = 0x8e010102;
  static const unsigned long CAN_MOTOR_ALIVE   = 0x8f010102;
  static const unsigned long CAN_R_MOTOR_ALIVE = 0x8f810201;

  // odometry messages

  static const unsigned long CAN_GET_POS_CONT  = 0x81020102;
  static const unsigned long CAN_R_GET_POS     = 0x81820201;
  static const unsigned long CAN_SET_POS       = 0x8a020102; 
  static const unsigned long CAN_SET_POS_REL   = 0x8b020102;
  static const unsigned long CAN_ODO_ALIVE     = 0x8f020102;
  static const unsigned long CAN_R_ODO_ALIVE   = 0x8f820201;

  // port messages

  static const unsigned long CAN_READ_PORTS    = 0x81030102;
  static const unsigned long CAN_R_READ_PORTS  = 0x81830201;
  static const unsigned long CAN_WRITE_PORT    = 0x82030102;
  static const unsigned long CAN_READ_ANALOG   = 0x83030102;
  static const unsigned long CAN_R_READ_ANALOG = 0x83830201;
  static const unsigned long CAN_SET_PORT      = 0x8a030102;
  static const unsigned long CAN_PORTS_ALL_OFF = 0x8e030102;
  static const unsigned long CAN_PORTS_ALIVE   = 0x8f030102;
  static const unsigned long CAN_R_PORTS_ALIVE = 0x8f830201;

  // stall messages

  static const unsigned long CAN_STALL_INIT    = 0x8a040102;
  static const unsigned long CAN_R_STALL       = 0x81840201;
  static const unsigned long CAN_STALL_ALIVE   = 0x8f040102;
  static const unsigned long CAN_R_STALL_ALIVE = 0x8f840201;

  // kicker messages

  static const unsigned long CAN_KICK          = 0x81010103;
  static const unsigned long CAN_KICK_IF_BALL  = 0x82010103;
  static const unsigned long CAN_KICK_ALL_OFF  = 0x8e010103;
  static const unsigned long CAN_KICK_ALIVE    = 0x8f010103;
  static const unsigned long CAN_R_KICK_ALIVE  = 0x8f810301;

  // servo messages

  static const unsigned long CAN_SERVO_GO      = 0x81010104;
  static const unsigned long CAN_SERVO_ALL_OFF = 0x8e010104;
  static const unsigned long CAN_SERVO_ALIVE   = 0x8f010104;
  static const unsigned long CAN_R_SERVO_ALIVE = 0x8f810401;

  // infrared messages

  static const unsigned long CAN_IR_GET_CONT   = 0x81050102;
  static const unsigned long CAN_R_IR_GET_CONT = 0x81850201;
  static const unsigned long CAN_IR_ALIVE      = 0x8f050102;
  static const unsigned long CAN_R_IR_ALIVE    = 0x8f850201;

  // debug messages

  static const unsigned long CAN_R_DBG_PRINT   = 0x810e0200;
  static const unsigned long CAN_STATUS        = 0x8e0e0102;
  static const unsigned long CAN_DBG_ALIVE     = 0x8f0e0102;
  static const unsigned long CAN_R_DBG_ALIVE   = 0x8f8e0001;

  // test messages

  static const unsigned long CAN_TEST_ADD      = 0x860e0102;
  static const unsigned long CAN_TEST_RESULT   = 0x870e0102;

};
#endif
