// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PsosDevice_h
#define PsosDevice_h

namespace Psos
{
  // forward declarations
  class Message;

  // types of timers registerd for Mcp::EventHandler
  typedef enum { PING_TIMER, SYNCH_TIMER } TimerType;

  // values indicating beginning of a psos message
  static const char START_1 = 0xfa;
  static const char START_2 = 0xfb;
  static const unsigned short START_12 = 0xfbfa;
 
  // PSOS 4.6 supported client commands
  static const char SF_SYNC_0    = 0x00;
  static const char SF_SYNC_1    = 0x01;
  static const char SF_SYNC_2    = 0x02;

  static const char SF_COMPULSE   = 0x00; // watchdog cancel
  static const char SF_COMOPEN    = 0x01; // start SIP stream
  static const char SF_COMCLOSE   = 0x02; // close connection
  static const char SF_COMPOLLING = 0x03; // sonar polling sequence
  static const char SF_COMENABLE  = 0x04; // motor enabling
  static const char SF_COMSETV    = 0x05; // max trans
  static const char SF_COMSETA    = 0x06; // trans acceleration
  static const char SF_COMSETO    = 0x07; // odometry reset
  static const char SF_COMMOVE    = 0x08; // relative translation
  static const char SF_COMROTATE  = 0x09; // rotation
  static const char SF_COMSETRV   = 0x0a; // max rot
  static const char SF_COMVEL     = 0x0b; // translation
  static const char SF_COMHEAD    = 0x0c; // absolute rotation
  static const char SF_COMDHEAD   = 0x0d; // relative rotation
  static const char SF_COMDROTATE = 0x0e; // ???
  static const char SF_COMSAY     = 0x0f; // speech output

  static const char SF_COMCONFIG  = 0x12; // config SIP (aros, p2os)
  static const char SF_COMENCODER = 0x13; // encoder SIP (aros, p2os)

  static const char SF_COMRVEL    = 0x15; // rotation
  static const char SF_COMDCHEAD  = 0x16; // relative heading setpoint
  static const char SF_COMSETRA   = 0x17; // rot acceleration

  static const char SF_COMSONAR   = 0x1c; // rotation
  static const char SF_COMSTOP    = 0x1d; // emergency stop

  static const char SF_COMDIGOUT  = 0x1e; //
  static const char SF_COMTIMER   = 0x1f; //
  static const char SF_COMVEL2    = 0x20; // wheel velo
  static const char SF_COMGRIPPER = 0x21; // gripper commands

  static const char SF_COMGVAL    = 0x23; // gripper values
  static const char SF_COMGQUEST  = 0x24; // gripper SIP

  static const char SF_COMIOREQ   = 0x28; // IO SIP
  static const char SF_COMPTUPOS  = 0x29; // servo pulse

  static const char SF_COMTTY2    = 0x2a; //write to aux1
  static const char SF_COMGETAUX  = 0x2b; //read from aux1

  static const char SF_COMBSTALL  = 0x2c; // stall on bump

  static const char SF_COMESTOP   = 0x37; // emergency stop
  static const char SF_COMESTALL  = 0x38; // emergency button mode

  static const char SF_COMSTEP    = 0x40; // simulator

  // PSOS 4.6 client command types
  static const char SF_ARGINT     = 0x3b;
  static const char SF_ARGNINT    = 0x1b;
  static const char SF_ARGSTR     = 0x2b;

  // predefined const messages
  extern const Message MSG_SYNC_0;
  extern const Message MSG_SYNC_1;
  extern const Message MSG_SYNC_2;

  extern const Message MSG_COMPULSE;
  extern const Message MSG_COMOPEN;
  extern const Message MSG_COMCLOSE;
  extern const Message MSG_COMSETO;

  extern const Message MSG_COMSTEP;
};

#endif
