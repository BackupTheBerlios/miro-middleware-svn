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

#include "miro/DevConsumer.h"
#include "idl/MotionStatusC.h"

#include <ace/Time_Value.h>

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

    int init_;

    double xPos_;
    double yPos_;

    double ticksL_;
    double ticksR_;
    double prevTicksL_;
    double prevTicksR_;
    int counterL;
    int counterR;
    
    ACE_Time_Value timeStampL_;
    ACE_Time_Value timeStampR_;
    ACE_Time_Value prevTimeStampL_;
    ACE_Time_Value prevTimeStampR_;

    double wheelBase_;
  };
};
#endif

