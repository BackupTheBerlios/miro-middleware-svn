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
#include "pioneer/GripperImpl.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/Parameters.h"

#include "miro/Synch.h"
#include "miro/Exception.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif


namespace Miro
{
  using std::cout;
  using std::cerr;
  using std::endl;

  // maximum wait time for cond.wait calls
  ACE_Time_Value GripperImpl::maxWait = ACE_Time_Value(0, 500000);

  GripperImpl::GripperImpl(Pioneer::Connection& _connection) throw(Exception) :
    connection(_connection)
  {
    DBG(cout << "Constructing GripperImpl" << endl);
  }

  GripperImpl::~GripperImpl()
  {
    DBG(cout << "Destructing GripperImpl" << endl);
  }

  // 
  // IDL interface implementation

  void
  GripperImpl::setGripPressure(short value) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    if ((value<0) || (value>100)) throw EOutOfBounds("Requested pressure exceeds capabilities");
    if (value==0) value=1; //the real minimum is 1;

    connection.gripperCommand(16); //GRIPpress
    connection.gripperValue(value); //actual value
  }
  
  void
  GripperImpl::openGrip() throw(Miro::EDevIO)
  {
    connection.gripperCommand(1); //GRIPopen
  }

  void
  GripperImpl::closeGrip() throw(Miro::EDevIO)
  {
    connection.gripperCommand(2); //GRIPclose
  }
  void
  GripperImpl::raiseGrip() throw(Miro::EDevIO)
  {
    connection.gripperCommand(4); //LIFTup
  }
  void
  GripperImpl::lowerGrip() throw(Miro::EDevIO)
  {
    connection.gripperCommand(5); //LIFTdown
  }
  void
  GripperImpl::storeGrip() throw(Miro::EDevIO)
  {
    connection.gripperCommand(7); //GRIPstore
  }
  void
  GripperImpl::deployGrip() throw(Miro::EDevIO)
  {
    connection.gripperCommand(8); //GRIPstore
  }
  void
  GripperImpl::stopGrip() throw(Miro::EDevIO)
  {
    connection.gripperCommand(15); //GRIPhalt
  }
  void
  GripperImpl::moveGrip(short ms) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    ms /= 20; //the gripper works in 20 ms increments

    if (ms==0) return; //if it doesn't have to move, don't move ;-)

    if (abs(ms)>255) throw Miro::EOutOfBounds();

    connection.gripperCommand(17); //LIFTcarry
    connection.gripperValue(ms);
  }

};
