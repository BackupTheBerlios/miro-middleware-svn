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
#ifndef JoystickService_h
#define JoystickService_h

#include "miro/StructuredPushSupplier.h"
#include "idl/MotionC.h"
#include "idl/JoystickC.h"

#include <ace/Event_Handler.h>

class EventHandler : public ACE_Event_Handler
{
        
 private:
  Miro::StructuredPushSupplier *JoystickSupplier_;
  ACE_HANDLE fd;
  ACE_Time_Value time_;
  static double oldrot;
  static long oldtrans;
  static int counter;
  double minRotation;
  long minTranslation;
  CosNotification::StructuredEvent event_;
  Miro::JoystickIDL OldStatus;
  Miro::JoystickIDL CurStatus;
    
 public:
  EventHandler(Miro::StructuredPushSupplier *_JoystickSupplier,ACE_HANDLE fd_);
  virtual int handle_input(ACE_HANDLE handle);	
  virtual int handle_timeout(const ACE_Time_Value&, const void *);
  double maxRotation;
  long maxTranslation;
  double PropFaktor;
};

#endif
