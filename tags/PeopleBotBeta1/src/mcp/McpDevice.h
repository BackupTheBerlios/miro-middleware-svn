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
#ifndef McpDevice_h
#define McpDevice_h

#include "McpMessage.h"

/* 
 * Device dependend constants
 */
namespace Mcp
{
  // We set an alarm for this many seconds before attempting to contact
  // the mcp. If the mcp is off, dead, etc the alarm will expire and
  // we can bag out.  If not, we reset the alarm and continue
  static const unsigned int INITIAL_WAIT_TIME = 5;
  
  // resolution of the mcp timer
  static const unsigned int TIME_UNITS_PER_SECOND = 256;
  
  //---------------------------------------------------------------------------
  // watchdog stuff

  // How long in seconds the base controller can go without hearing from
  // the base module. Any longer and it will limp all motors.
  static const unsigned long WATCHDOG_INTERVAL = 20;

  // types of timers registerd for Mcp::EventHandler
  typedef enum { STARTUP, WATCHDOG } timer_type;

  //----------------------------------------------------------------------------
  // mcp opcodes
  //----------------------------------------------------------------------------
  static const unsigned char OP_SYNCH              = 0x00;
  static const unsigned char OP_VERSION_REPORT     = 0x01;
  static const unsigned char OP_KILL               = 0x02;
  static const unsigned char OP_SHUTDOWN           = 0x03;
  static const unsigned char OP_WAIT_EVENT         = 0x04;
  static const unsigned char OP_DELAY              = 0x05;
  static const unsigned char OP_USER_MESSAGE       = 0x08;
  static const unsigned char OP_DIRECT_MODE        = 0x09;
  static const unsigned char OP_ERROR_DELAY        = 0x0A;
  static const unsigned char OP_ERROR_ACKNOWLEDGE  = 0x0B;
  static const unsigned char OP_FULL_DUPLEX        = 0x0C;
  static const unsigned char OP_HALF_DUPLEX        = 0x0D;
  static const unsigned char OP_JOYSTICK_DISABLE   = 0x0E;
  static const unsigned char OP_RADIO_DISABLE      = 0x0F; // deprecated
  static const unsigned char OP_STATUS_REPORT      = 0x10;
  static const unsigned char OP_PSR_DATA           = 0x11;
  static const unsigned char OP_PSR_PERIOD         = 0x12;
  static const unsigned char OP_LOCATE_ROT_INDEX   = 0x13;
  static const unsigned char OP_LOAD_POSITION      = 0x14;
  static const unsigned char OP_LOAD_HEADING       = 0x15;
  static const unsigned char OP_WATCH_DOG          = 0x16;
  
  // undocumented
  static const unsigned char OP_ADJUST_HEADING     = 0x17; // delta is sent as signed short 

  //----------------------------------------------------------------------------
  // mcp messages
  //----------------------------------------------------------------------------
  extern OutMessage MSG_SYNCH;
  extern OutMessage MSG_VERSION_REPORT;
  extern OutMessage MSG_KILL;
  extern OutMessage MSG_SHUTDOWN;
//  extern OutMessage MSG_WAIT_EVENT;
//  extern OutMessage MSG_DELAY;
  extern OutMessage MSG_USER_MESSAGE;
  extern OutMessage MSG_DIRECT_MODE;
//  extern OutMessage MSG_ERROR_DELAY;
  extern OutMessage MSG_ERROR_ACKNOWLEDGE;
  extern OutMessage MSG_FULL_DUPLEX;
  extern OutMessage MSG_HALF_DUPLEX;
//  extern OutMessage MSG_JOYSTICK_DISABLE;
//  extern OutMessage MSG_STATUS_REPORT;
//  extern OutMessage MSG_PSR_DATA;
//  extern OutMessage MSG_PSR_PERIOD;
  extern OutMessage MSG_LOCATE_ROT_INDEX;
//  extern OutMessage MSG_LOAD_POSITION;
//  extern OutMessage MSG_LOAD_HEADING;
//  extern OutMessage MSG_WATCH_DOG;
//  extern OutMessage MSG_ADJUST_HEADING;
};

#endif




