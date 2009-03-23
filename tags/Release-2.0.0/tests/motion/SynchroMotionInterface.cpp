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
