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

  static const char SF_COMPULSE   = 0x00;
  static const char SF_COMOPEN    = 0x01;
  static const char SF_COMCLOSE   = 0x02;
  static const char SF_COMPOLLING = 0x03;
  static const char SF_COMENABLE  = 0x04; // ???
  static const char SF_COMSETV    = 0x05; // ???
  static const char SF_COMSETA    = 0x06; // ???
  static const char SF_COMSETO    = 0x07;
  static const char SF_COMMOVE    = 0x08; // ???
  static const char SF_COMROTATE  = 0x09; // ???
  static const char SF_COMSETRV   = 0x0a; // ???
  static const char SF_COMVEL     = 0x0b;
  static const char SF_COMHEAD    = 0x0c;
  static const char SF_COMDHEAD   = 0x0d;
  static const char SF_COMDROTATE = 0x0e; // ???
  static const char SF_COMSAY     = 0x0f; // ???
  static const char SF_COMVISION  = 0x10; // ???
  static const char SF_COMVWINDOW = 0x11; // ???
  static const char SF_COMUDP     = 0x14; // ???
  static const char SF_COMRVEL    = 0x15;
  static const char SF_COMDIGOUT  = 0x1e;
  static const char SF_COMTIMER   = 0x1f;
  static const char SF_COMVEL2    = 0x20;
  static const char SF_COMGRIPPER = 0x21;
  static const char SF_COMPTUPOS  = 0x29;
  static const char SF_COMSTEP    = 0x40;

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
