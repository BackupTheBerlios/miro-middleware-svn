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

#include "PioneerStatus.h"

// forward declerations
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

  static const int NUM_SONARS = 8;

  /**
   * Class for using the Pioneer robot
   */
                                                          // nur zum test
  class StallImpl;
  class Consumer : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;
  public:
    Consumer(Miro::RangeSensorImpl * _pSonar,
	     Miro::RangeSensorImpl * _pTactile = NULL,
	     Miro::OdometryImpl * _pOdometry = NULL,
	     Miro::BatteryImpl * _pBattery = NULL,
	     Pioneer::StallImpl * _pStall = NULL,
	     Canon::CanonPanTiltImpl * _pCanonPanTilt=NULL);
    ~Consumer();

    virtual void handleMessage(const Miro::DevMessage * _message);
    
    // protected:
    Miro::RangeSensorImpl * pSonar;
    Miro::RangeSensorImpl * pTactile;
    Miro::OdometryImpl * pOdometry;
    Miro::BatteryImpl * pBattery;
    Pioneer::StallImpl * pStall;
    //public:
    //must be public in order for the connection to close the camera
    Canon::CanonPanTiltImpl * pCanonPanTilt;
    //protected:
    Canon::Answer *pAnswer;
    

    Miro::MotionStatusIDL status_;
    short prevX, prevY;
    double prevTheta;
    unsigned short bumpers_;

    Pioneer::Parameters * params_;
  };
};
#endif

