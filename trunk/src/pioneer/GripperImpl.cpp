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

  GripperImpl::GripperImpl(Pioneer::Connection& _connection,
			 Pioneer::Consumer& _consumer) throw(Exception) :
    connection(_connection),
    consumer(_consumer)
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
  GripperImpl::setGripPressure(double kg) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    if ((kg<0) || (kg>2)) throw EOutOfBounds("Requested pressure exceeds capabilities");

    unsigned short argument = int(kg*100/2); //! The formula is wrong, but is 
    //the best approximation given the "good" explanation on the manual
    if (argument==0) argument=1;

    connection.gripperCommand(16); //GRIPpress
    connection.gripperValue(argument); //actual value
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
  GripperImpl::stopGrip() throw(Miro::EDevIO)
  {
    connection.gripperCommand(15); //GRIPhalt
  }

};
