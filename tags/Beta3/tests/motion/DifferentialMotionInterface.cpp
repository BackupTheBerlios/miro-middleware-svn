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

#include "DifferentialMotionInterface.h"

#include "miro/Angle.h"
#include "miro/IO.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

DifferentialMotionInterface::DifferentialMotionInterface(Miro::DifferentialMotion_ptr _motion) :
  Super(_motion),
  diffMotion_(Miro::DifferentialMotion::_duplicate(_motion))
{}

DifferentialMotionInterface::~DifferentialMotionInterface()
{}

void
DifferentialMotionInterface::printOptions()
{
  Super::printOptions();
  std::cout << " 4 - getMinMaxLRVelocity" << endl
	    << " 5 - getTargetLRVelocity" << endl
	    << " 6 - setLRVelocity" << endl;
}

bool
DifferentialMotionInterface::evalCommand(char c)
{
  int msec = 0;

  CORBA::Long lTrans, rTrans;
  CORBA::Long minLTrans, maxLTrans;
  CORBA::Long minRTrans, maxRTrans;

  ACE_Time_Value t(0);
  bool rc = true;

  switch (c) {
  case '4':
    diffMotion_->getMinMaxLRVelocity(minLTrans, maxLTrans, minRTrans, maxRTrans);
    std::cout << "Velocity bounds: " << endl
	      << "Left wheel:  " << minLTrans << "mm/s to " << maxLTrans << "mm/s" << endl
	      << "Right wheel: " << minRTrans << "mm/s to " << maxRTrans << "mm/s" << endl;
    break;

  case '5':
    diffMotion_->getTargetLRVelocity(lTrans, rTrans);
    std::cout << "Current target velocity: left - " << lTrans << "mm/s right - " << rTrans << "mm/s" << endl;
    break;

  case '6':
    std::cout << "Set wheel velocity: " << endl
	      << "left translation (mm/s) : " << flush;
    std::cin >> lTrans;
    std::cout << "right translation (mm/s) : " << flush;
    std::cin >> rTrans;
    std::cout << "time befor stop (msec) : " << flush;
    std::cin >> msec;

    diffMotion_->setLRVelocity(lTrans, rTrans);

    t.msec(msec);
    ACE_OS::sleep(t);

    diffMotion_->setLRVelocity(0, 0);
    break;

  default:
    rc = Super::evalCommand(c);
    break;
  }
  return rc;
}
