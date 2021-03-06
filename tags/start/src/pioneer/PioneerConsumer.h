// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
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
};

namespace Pioneer
{
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
	     Miro::OdometryImpl * _pOdometry = NULL,
	     Pioneer::StallImpl * _pStall = NULL);
    ~Consumer();

    virtual void handleMessage(const Miro::DevMessage * _message);

    // protected:
    Miro::RangeSensorImpl * pSonar;
    Miro::OdometryImpl * pOdometry;
    Miro::MotionStatusIDL status_;
    Pioneer::StallImpl * pStall;                          // test

    short prevX, prevY;
    double prevTheta;
  };
};
#endif

