// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef FaulMotorConsumer_h
#define FaulMotorConsumer_h

#include <orbsvcs/CosNotifyCommC.h>

#include "miro/DevConsumer.h"
#include "miro/MotionStatusC.h"
#include <ace/OS.h>

// forward declarations
namespace Miro
{
  class OdometryImpl;
};

namespace FaulMotor
{
  // forward declarations
  class Parameters;

  /**
   * Class for using the FaulMotor robot
   */
  class Consumer : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;

  public:
    Consumer(Miro::OdometryImpl * _pOdometry = NULL);
    ~Consumer();

    virtual void handleMessage(const Miro::DevMessage * _message);

  protected:
    FaulMotor::Parameters * params_;
    Miro::OdometryImpl * pOdometry_;
    Miro::MotionStatusIDL status_;

    double prevPosL, prevPosR;
    double xPos, yPos;

    //unsigned short bumpers_;
    //int infrared_;
    int init;
    ACE_Time_Value prevTimeStamp;
  };
};
#endif

