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
#include "MoveToPoint.h"

#include "idl/MotionStatusC.h"
#include "miro/Angle.h"

#include "botsvcs/BAP/MotionArbiterMessage.h"

#include <cmath>

using std::endl;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotification::EventTypeSeq;

std::string MoveToPoint::name_ = "MoveToPoint";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(MoveToPoint, MoveToPointParameters);

MoveToPoint::MoveToPoint(EventChannel_ptr _ec, const std::string& _domainName) :
  Super(_ec)
{
  MIRO_LOG_CTOR("MoveToPoint.");

  setSingleSubscription(_domainName, "Odometry");
}

MoveToPoint::~MoveToPoint()
{
  MIRO_LOG_DTOR("MoveToPoint.");
}

void
MoveToPoint::action()
{
  const Miro::MotionStatusIDL * mStatus;
  const MoveToPointParameters * params =
    dynamic_cast<const MoveToPointParameters *>(params_);

  if (event->remainder_of_body >>= mStatus) {

    // initialize motion arbiter message
    Miro::BAP::MotionArbiterMessage message;
    message.id = this;
    message.active = true;

    // target coordinates
    double targetX =  params->targetX;
    double targetY =  params->targetY;

    int speed = 0;
    double rot = 0;

    double delx, dely, deltheta, dels, isx, isy, istheta;

    isx     = mStatus->position.point.x;
    isy     = mStatus->position.point.y;
    istheta = Miro::Angle(mStatus->position.heading).rad();

    delx = targetX - isx;
    dely = targetY - isy;

    // target distance
    dels     = sqrt((delx*delx) + (dely*dely));
    // target angle
    deltheta = atan2(dely, delx) - istheta;

    // normalize angle
    if (deltheta > M_PI)
      deltheta -= 2 * M_PI;
    else if (deltheta <= -M_PI)
      deltheta += 2 * M_PI;

    // select rotation proportional to delta theta
    rot = deltheta;
    // select translation
    speed = (dels > 1000.0)? params->fastTranslation : params->slowTranslation;

    // if the target angle is too big, turn in place
    if (fabs(deltheta) > Miro::deg2Rad(30.))
      speed = 0;

    // test for termination condition
    if (dels < params->radius){
      if (!params->correctOrientation) {
	sendMessage ("TargetReached");
	return;
      }
      else {
	double isDelta = Miro::Angle(params->angle - istheta).rad();
	if (fabs(isDelta) <= params->delta) {
	  sendMessage("TargetReached");
	  return;
	}
	else {
	  speed = 0;
	  rot = isDelta;
	}
      }
    }

    message.velocity.translation = speed;
    message.velocity.rotation = std::max(-params->maxRotation, 
					 std::min(params->maxRotation, rot));
    arbitrate(message);
  } 
  else {
    std::cerr << "MoveToPoint: No MotionStatusIDL message." << endl;
  }
}

const std::string&
MoveToPoint::getBehaviourName() const
{
  return name_;
}
