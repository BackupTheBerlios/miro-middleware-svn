// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (Midddleware for Robots)
// 
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "MoveToPoint.h"

#include "idl/MotionStatusC.h"
#include "miro/MotionArbiterMessage.h"
#include "miro/Angle.h"

// #include <algorithm>
#include <cmath>

using std::endl;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotification::EventTypeSeq;

std::string MoveToPoint::name_ = "MoveToPoint";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(MoveToPoint, MoveToPointParameters);

MoveToPoint::MoveToPoint(EventChannel_ptr _ec, const std::string& _domainName) :
  Super(_ec)
{
  std::cout << "Constructing MoveToPoint behaviour." << endl;

  setSingleSubscription(_domainName, "Odometry");
}

MoveToPoint::~MoveToPoint()
{
  std::cout << "Destructing MoveToPoint behaviour." << endl;
}

void
MoveToPoint::action()
{
  const Miro::MotionStatusIDL * mStatus;
  const MoveToPointParameters * params =
    dynamic_cast<const MoveToPointParameters *>(params_);

  if (event->remainder_of_body >>= mStatus) {

    // initialize motion arbiter message
    Miro::MotionArbiterMessage message;
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
