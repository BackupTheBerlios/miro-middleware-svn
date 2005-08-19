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

#include "SynchroMotionInterface.h"

#include "miro/Angle.h"
#include "miro/IO.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

SynchroMotionInterface::SynchroMotionInterface(Miro::SynchroMotion_ptr _motion) :
  Super(_motion),
  synchroMotion_(Miro::SynchroMotion::_duplicate(_motion))
{}

SynchroMotionInterface::~SynchroMotionInterface()
{}

void
SynchroMotionInterface::printOptions()
{
  Super::printOptions();
  std::cout << " 4 - rotate" << endl
	    << " 5 - rotateLimp" << endl
	    << " 6 - translate" << endl
	    << " 7 - translateLimp" << endl;
}

bool
SynchroMotionInterface::evalCommand(char c)
{
  int msec = 0;

  CORBA::Long trans;
  CORBA::Double rot;

  ACE_Time_Value t(0);
  bool rc = true;

  switch (c) {
  case '4':
    std::cout << "Set tranlational velocity: " << endl
	      << "translation (mm/s) : " << flush;
    std::cin >> trans;
    std::cout << "time before stop (msec) : " << flush;
    std::cin >> msec;

    synchroMotion_->setTranslation(trans);

    t.msec(msec);
    ACE_OS::sleep(t);
  case '5':
    synchroMotion_->translateLimp();
    break;
  case '6':
    std::cout << "Set rotational velocity: " << endl
	      << "rotation (°/s) : " << flush;
    std::cin >> rot;
    std::cout << "time before stop (msec) : " << flush;
    std::cin >> msec;

    rot = Miro::deg2Rad(rot);
    synchroMotion_->setRotation(rot);

    t.msec(msec);
    ACE_OS::sleep(t);
  case '7':
    synchroMotion_->rotateLimp();
    break;
  default:
    rc = Super::evalCommand(c);
    break;
  }
  return rc;
}
