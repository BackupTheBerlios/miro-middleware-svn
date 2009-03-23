// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef ButtonsDevice_h
#define ButtonsDevice_h

#include <ace/OS.h>  // for ACE_Time_Value

namespace B21Buttons
{
  // Addresses of the button ports
  static const unsigned int LIGHTS_ADDR = 0x0378;
  static const unsigned int PRESSED_ADDR = 0x0379;

  // Number of Buttons
  // There are four individually coloured buttons
  // and two red kill buttons
  static const unsigned long NUM_BUTTONS = 6;

  // Timer Ids
  static const int POLLING = 1;
  static const int BLINKING = 2;

  // Timer intervals
  static const ACE_Time_Value POLLING_TIME(0, 50000);
  static const ACE_Time_Value BLINKING_TIME(0, 500000);

  // Button bit address on port
  // The in bits are actually shifted by 4 to the right,
  // but become downshifted directly by the method reading the port
  // Note that we can't get the press status of the kill buttons
  static const unsigned char RED_BIT        = 0x01;
  static const unsigned char YELLOW_BIT     = 0x02;
  static const unsigned char GREEN_BIT      = 0x04;
  static const unsigned char BLUE_BIT       = 0x08;
  static const unsigned char RIGHT_KILL_BIT = 0x10;
  static const unsigned char LEFT_KILL_BIT  = 0x20;
};
#endif




