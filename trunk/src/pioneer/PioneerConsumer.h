// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
};

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
	     Canon::CanonPanTiltImpl * _pCanonPanTilt=NULL);
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
    //public:
    //must be public in order for the connection to close the camera
    Canon::CanonPanTiltImpl * pCanonPanTilt;
    //will be used for CanonCamera too
    Canon::Answer *pAnswer;
    // protected:    

    Miro::MotionStatusIDL status_;
    short prevX, prevY;
    unsigned short bumpers_;
    int infrared_;

    Pioneer::Parameters * params_;
  };
};
#endif

