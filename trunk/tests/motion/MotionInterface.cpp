// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "MotionInterface.h"

#include "miro/Angle.h"
#include "miro/IO.h"

MotionInterface::MotionInterface(Miro::Motion_ptr _motion) :
  motion_(Miro::Motion::_duplicate(_motion))
{}

MotionInterface::~MotionInterface()
{}

void
MotionInterface::printOptions()
{
  std::cout << " 0 - getMinMaxVelocity" << endl
	    << " 1 - getTargetVelocity" << endl
	    << " 2 - setVelocity" << endl
	    << " 3 - limp" << endl;
}

bool
MotionInterface::evalCommand(char c)
{
  Miro::VelocityIDL velocity;

  int msec = 0;
  int trans = 0;
  double rot = 0.;

  CORBA::Long minTrans, maxTrans;
  CORBA::Double minRot, maxRot;

  ACE_Time_Value t(0);
  bool rc = true;

  switch (c) {
  case '0':
    motion_->getMinMaxVelocity(minTrans, maxTrans, minRot, maxRot);
    std::cout << "Velocity bounds: " << endl
	      << "Translation: " << minTrans << "mm/s to " << maxTrans << "mm/s" << endl
	      << "Rotation: "<< Miro::rad2Deg(minRot) << "° to " << Miro::rad2Deg(maxRot) << "°" << endl;
    break;
  case '1':
    velocity = motion_->getTargetVelocity();
    std::cout << "Current target velocity: " << velocity << endl;
    break;
  case '2':
    std::cout << "Set velocity: " << endl
	      << "translation (mm/s) - " << flush;
    std::cin >> trans;
    std::cout << "rotation (°/s) - " << flush;
    std::cin >> rot;
    std::cout << "time befor stop (msec) - " << flush;
    std::cin >> msec;

    rot = Miro::deg2Rad(rot);
    velocity.translation = trans;
    velocity.rotation = rot;

    motion_->setVelocity(velocity);

    t.msec(msec);
    ACE_OS::sleep(t);
  case '3':
    motion_->limp();
    break;
  default:
    rc = false;
    break;
  }
  return rc;
}
