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
#ifndef PanTiltOpcodes_h
#define PanTiltOpcodes_h

#include <string>

namespace DpPanTilt
{
  /* 
   * a nice enum for indexing in the array
   */
  enum {PANPOSITION, TILTPOSITION, 
	TARGETPANPOSITION, TARGETTILTPOSITION, 
	MINPANPOSITION, MAXPANPOSITION,
	MINTILTPOSITION, MAXTILTPOSITION,
	LIMITBOUNDSENABLED, LIMITBOUNDSDISABLED,
	TARGETPANSPEED, TARGETTILTSPEED, 
	PANACCELERATION, TILTACCELERATION,
	PANBASESPEED, TILTBASESPEED, 
	MAXPANSPEED, MINPANSPEED, 
	MAXTILTSPEED, MINTILTSPEED, 
	ECHOINGON, ECHOINGOFF, 
	ASCIIVERBOSE, ASCIITERSE, 
	VERSION, 
	PANHOLDLOW, PANHOLDREGULAR, PANHOLDOFF, 
	TILTHOLDLOW, TILTHOLDREGULAR, TILTHOLDOFF, 
	PANMOVELOW, PANMOVEREGULAR, PANMOVEHIGH, 
	TILTMOVELOW, TILTMOVEREGULAR, TILTMOVEHIGH
  };
  const int FIRSTOPCODE = PANPOSITION;
  const int LASTOPCODE = TILTMOVEHIGH;
  /**
   * strings in an array
   */
  const std::string panTiltMessages[] = { 
    " Current Pan position is ", " Current Tilt position is ",  
    " Target Pan position is ", " Target Tilt position is ",
    " Minimum Pan position is ", " Maximum Pan position is ",
    " Minimum Tilt position is ", " Maximum Tilt position is ",
    " Limit bounds are ENABLED (soft limits enabled)", " Limit bounds are DISABLED (hard limits only: use care)",
    " Target Pan speed is ", " Target Tilt speed is ",
    " Pan acceleration is ", " Tilt acceleration is ",
    " Current Pan base speed is ", " Current Tilt base speed is ",
    " Maximum Pan speed is ", " Minimum Pan speed is ",
    " Maximum Tilt speed is ", " Minimum Tilt speed is ",
    " Echoing ON", " Echoing OFF",
    " ASCII verbose mode", " ASCII terse mode",
    " Pan-Tilt Controller v1.09.0b, (C)1996 Directed Perception, Inc., All Rights Reserved",
    " Pan in LOW hold power mode", " Pan in REGULAR hold power mode", " Pan in OFF hold power mode",
    " Tilt in LOW hold power mode", " Tilt in REGULAR hold power mode", " Tilt in OFF hold power mode",
    " Pan in LOW move power mode", " Pan in REGULAR move power mode", " Pan in HIGH move power mode",
    " Tilt in LOW move power mode", " Tilt in REGULAR move power mode", " Tilt in HIGH move power mode"
  };
};


// PR * 185.1428 seconds arc per position
// TR * 185.1428 seconds arc per position 

// R !T!T!P!P*

#endif




