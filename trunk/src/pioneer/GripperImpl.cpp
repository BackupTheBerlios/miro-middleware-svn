// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "pioneer/GripperImpl.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/PioneerParameters.h"

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
