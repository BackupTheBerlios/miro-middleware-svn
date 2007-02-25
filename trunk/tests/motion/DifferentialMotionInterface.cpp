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
