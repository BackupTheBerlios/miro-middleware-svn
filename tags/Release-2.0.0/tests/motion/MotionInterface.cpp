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
  std::cout << " 0 - getMinMaxVelocity" << std::endl
	    << " 1 - getTargetVelocity" << std::endl
	    << " 2 - setVelocity" << std::endl
	    << " 3 - limp" << std::endl;
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
    std::cout << "Velocity bounds: " << std::endl
	      << "Translation: " << minTrans << "mm/s to " << maxTrans << "mm/s" << std::endl
	      << "Rotation: "<< Miro::rad2Deg(minRot) << "° to " << Miro::rad2Deg(maxRot) << "°" << std::endl;
    break;
  case '1':
    velocity = motion_->getTargetVelocity();
    std::cout << "Current target velocity: " << velocity << std::endl;
    break;
  case '2':
    std::cout << "Set velocity: " << std::endl
	      << "translation (mm/s) : " << std::flush;
    std::cin >> trans;
    std::cout << "rotation (°/s) : " << std::flush;
    std::cin >> rot;
    std::cout << "time befor stop (msec) : " << std::flush;
    std::cin >> msec;

    rot = Miro::deg2Rad(rot);
    velocity.translation = trans;
    velocity.rotation = rot;

    motion_->setVelocity(velocity);

    t.msec(msec);
    ACE_OS::sleep(t);

    velocity.translation = 0;
    velocity.rotation = 0.;

    motion_->setVelocity(velocity);
    break;

  case '3':
    motion_->limp();
    break;
  default:
    rc = false;
    break;
  }
  return rc;
}
