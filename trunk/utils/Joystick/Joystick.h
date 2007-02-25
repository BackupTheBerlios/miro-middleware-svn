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
#ifndef Joystick_h
#define Joystick_h

#include "idl/MotionC.h"
#include "idl/KickerC.h"

#include <ace/Event_Handler.h>

class EventHandler : public ACE_Event_Handler
{
        
 private:
  Miro::Motion_var motion_;
  Miro::Kicker_var kicker_;

  Miro::VelocityIDL OldVelocity;
  Miro::VelocityIDL CurVelocity;

  ACE_HANDLE fd;
  static double oldrot;
  static long oldtrans;
  bool kickeron_;
  double minRotation;
  double maxRotation;
  long minTranslation;
  long maxTranslation;
    
 public:
  EventHandler(Miro::Motion_var _motion, Miro::Kicker_var _kicker, bool
	       _kickeron, ACE_HANDLE fd_);
  virtual int handle_input(ACE_HANDLE handle);
  virtual int handle_timeout(const ACE_Time_Value&, const void *);
    
};

#endif
