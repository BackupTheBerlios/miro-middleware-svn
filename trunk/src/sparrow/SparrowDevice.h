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


  // Sparrow2003Messages

  static const unsigned long CAN_WRITE_MOTOR_LEFT_2003 = 0x05010102;
  static const unsigned long CAN_WRITE_MOTOR_RIGHT_2003 = 0x04010102;
  static const unsigned long CAN_MOTOR_TICKS_LEFT_2003 = 0x85020201;
  static const unsigned long CAN_MOTOR_TICKS_RIGHT_2003 = 0x84020201;
  static const unsigned long CAN_R_MOTOR_ALIVE_2003 = 0x8f020201;
  static const unsigned long CAN_HOST_ALIVE_2003 = 0x1E0f0102;
  static const unsigned long CAN_MOTOR_FLOOD_2003 = 0x03010102;
  static const unsigned long CAN_MOTOR_CANN = 0x83010201;


  // port messages
  /* kicker messages sparrows 2003 */
    /* messages to kicker device */
  static const unsigned long CAN_R_KICK_RESET_2003  = 0x9F0F0301;   // sent once after device reset
  static const unsigned long CAN_R_KICK_STATUS_2003 = 0x9D0F0301;   // error or status report from device
  static const unsigned long CAN_R_KICK_ALIVE_2003  = 0x9e0f0301;
    /* messages from kicker device */
  static const unsigned long CAN_KICK_2003          = 0x81010103;  // control kicker
  static const unsigned long CAN_KICK_RESET_2003    = 0x8d010103;   // enforce device reset
  static const unsigned long CAN_KICK_QUERY_2003    = 0x8e010103;   // query device version/status

  // servo messages

  static const unsigned long CAN_SERVO_GO_2003      = 0x81010104;
  static const unsigned long CAN_R_PAN_ALIVE_2003   = 0x9e0f0401;

  // infrared messages


  static const unsigned long CAN_R_IR_GET_CONT1_2003 = 0x83810501;
  static const unsigned long CAN_R_IR_ALIVE1_2003    = 0x9e0f0501;
  static const unsigned long CAN_IR_SET_FREQ1_2003    = 0x02010105;


  static const unsigned long CAN_R_IR_GET_CONT2_2003 = 0x83810601;
  static const unsigned long CAN_R_IR_ALIVE2_2003    = 0x9e0f0601;
  static const unsigned long CAN_IR_SET_FREQ2_2003    = 0x02010106;

  // omnicam-pan messages
  /* Messages from omnicam can-serial bridge */
  static const unsigned long CAN_R_OMNIPAN_RESET = 0x9F0F0701;   // sent once after device reset
  static const unsigned long CAN_R_OMNIPAN_ALIVE = 0x9E0F0701;   // alive message from device
  static const unsigned long CAN_R_OMNIPAN_STATUS = 0x9D0F0701;  // error or status report from device
  static const unsigned long CAN_R_OMNIPAN_DATA  = 0x910F0701;   // data from motor controller 4 host
  /* messages to omnicam can-serial bridge */
  static const unsigned long CAN_OMNIPAN_RESET   = 0x8d010107;   // enforce device reset
  static const unsigned long CAN_OMNIPAN_QUERY   = 0x8e010107;   // query device version/status
  static const unsigned long CAN_OMNIPAN_DATA    = 0x81010107;   // send data to motor controller

};
#endif
