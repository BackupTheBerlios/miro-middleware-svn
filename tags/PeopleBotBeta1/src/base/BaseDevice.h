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
#ifndef BaseDevice_h
#define BaseDevice_h

#include <cmath> // for M_PI

namespace Mcp
{
  class OutMessage;
};

namespace Base
{
  using Mcp::OutMessage;

  typedef enum { LIMP, HALT, 
		 CONTINUOUS_POS, CONTINUOUS_NEG, 
		 TO_POSITION, TORQUE, POWER } StateType;

  //---------------------------------------------------------------------------
  // constants specifying base capabilities
  //---------------------------------------------------------------------------

  // The initial value of the odometry  counters of the base should be in
  // the middle of their range.
  static const unsigned short INITIAL_XPOS = 0x8000;
  static const unsigned short INITIAL_YPOS = 0x8000;

  static const double ENC_PER_CM = 256.3;       // encoders/cm
  static const double POS_PER_CM = 1.001;       // deadreck_pos/cm

  //---------------------------------------------------------------------------
  // battery stuff

  // Battery calls are in terms of battery units, which are .1 volts
  static const double BATTERY_UNITS_PER_VOLT = 10.;

  // The current battery voltage gets polled ever 30 sec by the base server.
  static const int BATTERYPOLL_START = 5;
  static const int BATTERYPOLL_INTERVAL = 30;

  // Obviously, programs will not be running if the base is at 0 volts.
  // So, 0 volts, or a very high voltage reading, is probably a problem with
  // the base (like the base computer has been turned off) so we should not 
  // believe it.
  static const double MIN_BELIEVABLE_VOLTAGE = 1.;
  static const double MAX_BELIEVABLE_VOLTAGE = 100.;

  //---------------------------------------------------------------------------
  // base status report stuff

  // base's rotational resolution
  static const unsigned long TICS_PER_2_PI = 1024;

  // helper functions to convert radiant in base metrics
  double base2rad(long base);
  int rad2base(double rad);
  unsigned long convertMMToEncoders(unsigned long arg);

  // By default get as many status reports from the base as possible.
  // Which is every 1/10 sec. - The base timer resolution is 1/256:
  static const unsigned long DEFAULT_REPORT_PERIOD = 26;

  // what bits of the TranslateStatus is the phase number, the most
  // interesting part of the Translate and Rotate Status fields
  // so if statusPhase(report->RotateStatus) == PHASE_ACCEL, the
  // motor on the rotate axis is accelerating
  static const unsigned long PHASE_STOP   = 0;
  static const unsigned long PHASE_ACCEL  = 1;
  static const unsigned long PHASE_VELOC  = 2;
  static const unsigned long PHASE_DECEL  = 3;
  static const unsigned long PHASE_HALT   = 4;
  static const unsigned long PHASE_LIMP   = 5;
  static const unsigned long PHASE_POWER  = 6;
  static const unsigned long PHASE_TORQUE = 7;

  // These are the bits you send to statusData or statusReport in
  // order to have the corresponding piece of data sent back. That
  // is, to get Xpos and Ypos back, you send (REPORT_X | REPORT_Y)

  // allways set the first one automatically
  static const unsigned long REPORT_STATUS_DATA	= 	    (1<<0);
  static const unsigned long REPORT_BASE_CLOCK =            (1<<1) ;
  static const unsigned long REPORT_GENERAL_STATUS =        (1<<2); 
  static const unsigned long REPORT_X =                     (1<<4);
  static const unsigned long REPORT_Y =                     (1<<5);
  static const unsigned long REPORT_HEADING =               (1<<6);
  static const unsigned long REPORT_BASE_RELATIVE_HEADING = (1<<7);
  static const unsigned long REPORT_TRANSLATE_ERROR =       (1<<8);
  static const unsigned long REPORT_TRANSLATE_VELOCITY=     (1<<9);
  static const unsigned long REPORT_TRANSLATE_STATUS =      (1<<11);
  static const unsigned long REPORT_ROTATE_ERROR =          (1<<16);
  static const unsigned long REPORT_ROTATE_VELOCITY =       (1<<17);
  static const unsigned long REPORT_ROTATE_STATUS =         (1<<19);

  // half the stuff the status report could send is not really of interest 
  // so to get everything useful use REPORT_EVERYTHING 
  static const unsigned long REPORT_EVERYTHING = (REPORT_STATUS_DATA | 
						  REPORT_BASE_CLOCK |
						  REPORT_GENERAL_STATUS |
						  REPORT_X |
						  REPORT_Y |
						  REPORT_HEADING |
						  REPORT_BASE_RELATIVE_HEADING |
						  REPORT_TRANSLATE_ERROR |
						  REPORT_TRANSLATE_VELOCITY |
						  REPORT_TRANSLATE_STATUS |
						  REPORT_ROTATE_ERROR |
						  REPORT_ROTATE_VELOCITY |
						  REPORT_ROTATE_STATUS);
  // The status report can also report this stuff, which seemed fairly
  // obsolete now that the programming interface is so different      
  // 3 = base bump switches, not used 
  // 10-15 A/D and motor stuff 
  // 18-31 More A/D, motor, radio stuff 

  // things to be report by normal base behaviour
  static const unsigned long REQUIRED_STATUS_DATA  = REPORT_EVERYTHING;

  //---------------------------------------------------------------------------
  // base opcodes
  //---------------------------------------------------------------------------

  static const unsigned char OP_TRANS_VEL           = 0x20;
  static const unsigned char OP_TRANS_ACCEL         = 0x21;
  static const unsigned char OP_TRANS_HALT          = 0x22;
  static const unsigned char OP_TRANS_LIMP          = 0x23;
  static const unsigned char OP_TRANS_REL_POS       = 0x24;
  static const unsigned char OP_TRANS_REL_NEG       = 0x25;
  static const unsigned char OP_TRANS_VEL_POS       = 0x26;
  static const unsigned char OP_TRANS_VEL_NEG       = 0x27;
  static const unsigned char OP_TRANS_TRQ_LEFT      = 0x28;
  static const unsigned char OP_TRANS_TRQ_RIGHT     = 0x29;
  static const unsigned char OP_TRANS_PWR_POS       = 0x2A;
  static const unsigned char OP_TRANS_PWR_NEG       = 0x2B;
  static const unsigned char OP_TRANS_CL_RANGE      = 0x2C;
  static const unsigned char OP_TRANS_MAX_TORQUE    = 0x2D;
  static const unsigned char OP_TRANS_SLOPE         = 0x2E;
  static const unsigned char OP_TRANS_FRICTION      = 0x2F;
  static const unsigned char OP_TRANS_TRQ_ZERO      = 0x30;
  static const unsigned char OP_TRANS_CURRENT       = 0x3A;
  static const unsigned char OP_TRANS_POSITION      = 0x3B;
  static const unsigned char OP_TRANS_WHERE         = 0x3C;

  static const unsigned char OP_ROT_VEL             = 0x40;
  static const unsigned char OP_ROT_ACCEL           = 0x41;
  static const unsigned char OP_ROT_HALT            = 0x42;
  static const unsigned char OP_ROT_LIMP            = 0x43;
  static const unsigned char OP_ROT_REL_POS         = 0x44;
  static const unsigned char OP_ROT_REL_NEG         = 0x45;
  static const unsigned char OP_ROT_VEL_POS         = 0x46;
  static const unsigned char OP_ROT_VEL_NEG         = 0x47;
  static const unsigned char OP_ROT_TRQ_LEFT        = 0x48;
  static const unsigned char OP_ROT_TRQ_RIGHT       = 0x49;
  static const unsigned char OP_ROT_PWR_POS         = 0x4A;
  static const unsigned char OP_ROT_PWR_NEG         = 0x4B;
  static const unsigned char OP_ROT_CL_RANGE        = 0x4C;
  static const unsigned char OP_ROT_MAX_TORQUE      = 0x4D;
  static const unsigned char OP_ROT_SLOPE           = 0x4E;
  static const unsigned char OP_ROT_FRICTION        = 0x4F;
  static const unsigned char OP_ROT_TRQ_ZERO        = 0x50;
  static const unsigned char OP_ROT_CURRENT         = 0x5A;
  static const unsigned char OP_ROT_POSITION        = 0x5B;
  static const unsigned char OP_ROT_WHERE           = 0x5C;

  static const unsigned char OP_BATTERY_VOLTAGE     = 0x60;
  static const unsigned char OP_BATTERY_CURRENT     = 0x61;

  static const unsigned char OP_BUMP_SWITCHES       = 0x62;  // Deprecated
  static const unsigned char OP_CLOCK               = 0x63;
  static const unsigned char OP_BUMP_STOP_ENABLE_VEL= 0x64;  // Deprecated
  
  // undocumented
  static const unsigned char OP_INDEX_REPORT        = 0x65;

  //---------------------------------------------------------------------------
  // base messages
  //---------------------------------------------------------------------------

//  extern OutMessage MSG_TRANS_VEL;
//  extern OutMessage MSG_TRANS_ACCEL;
  extern OutMessage MSG_TRANS_HALT;
  extern OutMessage MSG_TRANS_LIMP;
//  extern OutMessage MSG_TRANS_REL_POS;
//  extern OutMessage MSG_TRANS_REL_NEG;
  extern OutMessage MSG_TRANS_VEL_POS;
  extern OutMessage MSG_TRANS_VEL_NEG;
//  extern OutMessage MSG_TRANS_TRQ_LEFT;
//  extern OutMessage MSG_TRANS_TRQ_RIGHT;
//  extern OutMessage MSG_TRANS_PWR_POS;
//  extern OutMessage MSG_TRANS_PWR_NEG;
//  extern OutMessage MSG_TRANS_CL_RANGE;
//  extern OutMessage MSG_TRANS_MAX_TORQUE;
//  extern OutMessage MSG_TRANS_SLOPE;
//  extern OutMessage MSG_TRANS_FRICTION;
//  extern OutMessage MSG_TRANS_TRQ_ZERO;
  extern OutMessage MSG_TRANS_CURRENT;
  extern OutMessage MSG_TRANS_POSITION;
  extern OutMessage MSG_TRANS_WHERE;

//  extern OutMessage MSG_ROT_VEL;
//  extern OutMessage MSG_ROT_ACCEL;
  extern OutMessage MSG_ROT_HALT;
  extern OutMessage MSG_ROT_LIMP;
//  extern OutMessage MSG_ROT_REL_POS;
//  extern OutMessage MSG_ROT_REL_NEG;
  extern OutMessage MSG_ROT_VEL_POS;
  extern OutMessage MSG_ROT_VEL_NEG;
//  extern OutMessage MSG_ROT_TRQ_LEFT;
//  extern OutMessage MSG_ROT_TRQ_RIGHT;
//  extern OutMessage MSG_ROT_PWR_POS;
//  extern OutMessage MSG_ROT_PWR_NEG;
//  extern OutMessage MSG_ROT_CL_RANGE;
//  extern OutMessage MSG_ROT_MAX_TORQUE;
//  extern OutMessage MSG_ROT_SLOPE;
//  extern OutMessage MSG_ROT_FRICTION;
//  extern OutMessage MSG_ROT_TRQ_ZERO;
  extern OutMessage MSG_ROT_CURRENT;
  extern OutMessage MSG_ROT_POSITION;
  extern OutMessage MSG_ROT_WHERE;

  extern OutMessage MSG_BATTERY_VOLTAGE;
  extern OutMessage MSG_BATTERY_CURRENT;

  extern OutMessage MSG_CLOCK;
  
  // undocumented
  extern OutMessage MSG_INDEX_REPORT;

  //---------------------------------------------------------------------------
  // inlines
  //---------------------------------------------------------------------------

  inline
  double
  base2rad(long base) {
    return M_PI/512. * (double) (base % 1024);
  }

  inline
  int
  rad2base(double rad) {
    return ((int)rint(512./M_PI * rad) % 1024);
  }

  inline
  unsigned long
  convertMMToEncoders(unsigned long arg) {
    return (unsigned long) ((double)arg * ENC_PER_CM / 10);
  }
};

#endif




