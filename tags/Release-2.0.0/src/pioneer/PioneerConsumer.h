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
#ifndef PioneerConsumer_h
#define PioneerConsumer_h

#include <orbsvcs/CosNotifyCommC.h>

#include "miro/Synch.h"
#include "miro/DevConsumer.h"
#include "idl/MotionStatusC.h"

// forward declarations
namespace Miro
{
  class OdometryImpl;
  class RangeSensorImpl;
  class BatteryImpl;
}

namespace Canon
{
  class CanonPanTiltImpl;
  class Answer;
}

namespace Pioneer
{
  // forward declarations
  class Parameters;
  class MotionImpl;

  static const int NUM_SONARS = 8;

  /**
   * Class for using the Pioneer robot
   */
                                                          // nur zum test
  class StallImpl;
  class TCM2Impl;
  class CameraAnswer;

  class Consumer : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;
  public:
    Consumer(Miro::RangeSensorImpl * _pSonar,
	     Miro::RangeSensorImpl * _pTactile = NULL,
	     Miro::RangeSensorImpl * _pInfrared = NULL,
	     MotionImpl * _pMotion = NULL,
	     Miro::OdometryImpl * _pOdometry = NULL,
	     Miro::BatteryImpl * _pBattery = NULL,
	     Pioneer::StallImpl * _pStall = NULL,
	     Pioneer::TCM2Impl * _pTCM2 = NULL,
	     Pioneer::CameraAnswer * _pCameraAnswer = NULL);
    ~Consumer();

    virtual void handleMessage(const Miro::DevMessage * _message);
    
    // protected:
    Miro::RangeSensorImpl * pSonar;
    Miro::RangeSensorImpl * pTactile;
    Miro::RangeSensorImpl * pInfrared;
    MotionImpl * pMotion;
    Miro::OdometryImpl * pOdometry;
    Miro::BatteryImpl * pBattery;
    Pioneer::StallImpl * pStall;
    Pioneer::TCM2Impl * pTCM2;
    Pioneer::CameraAnswer * pCameraAnswer;

    Miro::MotionStatusIDL status_;
    short prevX, prevY;
    unsigned short bumpers_;
    int infrared_;

    Pioneer::Parameters * params_;
  };
};
#endif

