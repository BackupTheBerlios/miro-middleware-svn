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


#include "SparrowPanTiltImpl.h"
#include "SparrowConnection.h"
#include "miro/Angle.h"

namespace Sparrow
{
  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;

  // Implementation skeleton constructor
  PanTiltImpl::PanTiltImpl(Connection& _connection) :
    connection(_connection),
    params_(*Parameters::instance()),
    timeLastSet(ACE_OS::gettimeofday())
  {
    connection.setServo(0, Miro::deg2Rad(0));

    //    connection.setServo(1, params_.farAngle);

    lastPosition = Miro::deg2Rad(0);
    nextPosition = lastPosition;
    actPosition = nextPosition;
    accuracy = 0;
  }
  
  // Implementation skeleton destructor
  PanTiltImpl::~PanTiltImpl()
  {
  }
  
#ifdef LETSTILTAGAIN
  Miro::PanTiltPositionIDL 
  PanTiltImpl::getPosition() throw (EDevIO, ETimeOut)
  {
    Miro::Guard guard(mutex);

    return currentPosition();
  }
  
  void 
  PanTiltImpl::setPosition(const PanTiltPositionIDL& dest)
    throw (EOutOfBounds, EDevIO)
  {
    Miro::Guard guard(mutex);

    connection.setServo(0, dest.panvalue);
    connection.setServo(1, dest.tiltvalue);

    timeLastSet = ACE_OS::gettimeofday();
    lastPosition = currentPosition();
    nextPosition = dest;
  }
#endif

  void 
  PanTiltImpl::setPan(CORBA::Double value) throw (EDevIO, EOutOfBounds)
  {
    Miro::Guard guard(mutex);

    if (value != nextPosition) {

      connection.setServo(0, value);

      updateAccuracy();
      lastPosition = nextPosition;
      nextPosition = value;
      if( accuracy < 0.0 )
        accuracy = 0.0;
      accuracy += fabs( lastPosition - nextPosition ) * 0.6;
    }
  }
  
  CORBA::Double 
  PanTiltImpl::getPan() throw (EDevIO)
  {
    Miro::Guard guard(mutex);
    return currentPosition().angle;
  }

#ifdef LETSTILTAGAIN  
  void
  PanTiltImpl::setTilt(CORBA::Double value) throw (EDevIO, EOutOfBounds)
  {
    Miro::Guard guard(mutex);

    connection.setServo(1, value);

    timeLastSet = ACE_OS::gettimeofday();
    lastPosition = currentPosition();
    nextPosition.tiltvalue = value;
  }

  CORBA::Double
  PanTiltImpl::getTilt() throw (EDevIO)
  {
    Miro::Guard guard(mutex);
    return currentPosition().tiltvalue;
  }
#endif
  

  CORBA::Boolean 
  PanTiltImpl::panning() throw()
  {
    Miro::Guard guard(mutex);
    return prvPanning();
  }

  Miro::PanPositionIDL
  PanTiltImpl::currentPan() throw()
  {
    Miro::Guard guard(mutex);
    return currentPosition();
  }

#ifdef LETSTILTAGAIN  
  CORBA::Boolean
  PanTiltImpl::tilting() throw()
  {
    Miro::Guard guard(mutex);
    return prvTilting();
  }
#endif
  
  Miro::PanPositionIDL
  PanTiltImpl::currentPosition()
  {
    Miro::PanPositionIDL position;
    position.angle = actPosition;
    position.accuracy = Miro::deg2Rad(3.);

    if (prvPanning()) {
      position.accuracy += max( Miro::deg2Rad(7.), accuracy );

#ifdef WEGOTIT
      ACE_Time_Value delta = ACE_OS::gettimeofday() - timeLastSet;

      position.angle = lastPosition;
      position.accuracy = Miro::deg2Rad(10.);

      double blur = Miro::deg2Rad(0.04 * delta.msec());

      int t_msec = (int)fabs((nextPosition - lastPosition) * 300);

      // sind wir noch in der Bewegung?
      if (delta < ACE_Time_Value(t_msec / 1000 , (t_msec % 1000) * 1000)) {
	position.angle += (nextPosition - lastPosition) * (double)delta.msec() / (double)t_msec;
      }
      else {
	position.angle = nextPosition;
	blur *= 1 - (delta.msec() - t_msec) / 400;
      }
      position.accuracy += blur;
#endif

    }

#ifdef LETSTILTAGAIN
    if (!prvTilting())
      position.ti(!prvTilting())
      position.tiltvalue = nextPosition.tiltvalue;
    else
      position.tiltvalue = (lastPosition.panvalue <= nextPosition.panvalue)?
	lastPosition.panvalue + (now - timeLastSet).msec() * params_.tiltRadPerMSec :
	lastPosition.panvalue - (now - timeLastSet).msec() * params_.tiltRadPerMSec;
#endif

    return position;
  }
};


