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
#ifndef PioneerStatus_h
#define PioneerStatus_h

#include "miro/MotionStatusC.h" // PioneerReportDataIDL
#include "miro/ExceptionC.h"

#include "miro/Synch.h"
#include "miro/Angle.h"

#include <cmath>

// forward decleration
namespace Psos {
  class ServerInfoMessage;
};

namespace Pioneer
{
  /**
   * raw data of a base status report
   */
  struct ReportData
  {
    ReportData();
    void operator <<= (const Psos::ServerInfoMessage& message);

    ACE_Time_Value time;              // report time
    short xPos;                    // x position in mm
    short yPos;                    // y position in mm
    Miro::Angle heading;           // in rad
    short lVel;
    short rVel;
    short battery;
    short bumpers;
    
    
  };

  /**
   * output operator
   *
   * for debugging purposes only
   */
  ostream& operator << (ostream& out, const ReportData& report);

  /**
   * holds the current status of the base
   */
  class Status
  {
  public:
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------

    Status();

    Miro::MotionStatusIDL getStatus();
    Miro::MotionStatusIDL getWaitStatus() throw(Miro::ETimeOut);
    Miro::PositionIDL getPosition();
    Miro::PositionIDL getWaitPosition() throw(Miro::ETimeOut);
    Miro::VelocityIDL getVelocity();
    Miro::VelocityIDL getWaitVelocity() throw(Miro::ETimeOut);

    /**
     * set robot to world specified position
     */
    void setWorldPosition(const Miro::PositionIDL& robot);
    void updateWorldPosition(const Miro::PositionIDL& delta);

    void operator <<= (const Psos::ServerInfoMessage& report);

  protected:
    //-------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------

    // non - thread safe set postion stuff
    void protectedSetPosition(const Miro::PositionIDL& robot);
    void setTransformation();

    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    Miro::Mutex           mutex;
    Miro::Condition       cond;
    Miro::MotionStatusIDL status;

    ReportData currentReport;
    ReportData previousReport;

  private:
    //-------------------------------------------------------------------------
    // private object data
    //-------------------------------------------------------------------------

    Miro::PositionIDL origin;

    double sinHeading, cosHeading;

    static ACE_Time_Value maxWait;
  };

  //---------------------------------------------------------------------------
  // inlines
  //---------------------------------------------------------------------------


  //---------------------------------------------------------------------------
  // public methods
  //---------------------------------------------------------------------------
  inline
  Miro::MotionStatusIDL
  Status::getStatus() {
    Miro::Guard guard(mutex);
    return status;
  }
  inline
  Miro::MotionStatusIDL
  Status::getWaitStatus()  throw(Miro::ETimeOut) {
    ACE_Time_Value timeout(ACE_OS::gettimeofday() + maxWait);
    Miro::Guard guard(mutex);
    if (cond.wait(&timeout) == -1)
      throw Miro::ETimeOut();
    return status;
  }
  inline
  Miro::PositionIDL
  Status::getPosition() {
    Miro::Guard guard(mutex);
    return status.position;
  }
  inline
  Miro::PositionIDL
  Status::getWaitPosition()  throw(Miro::ETimeOut){
    ACE_Time_Value timeout(ACE_OS::gettimeofday() + maxWait);
    Miro::Guard guard(mutex);
    if (cond.wait(&timeout) == -1)
      throw Miro::ETimeOut();
    return status.position;
  }
  inline
  Miro::VelocityIDL
  Status::getVelocity() {
    Miro::Guard guard(mutex);
    return status.velocity;
  }
  inline
  Miro::VelocityIDL
  Status::getWaitVelocity()  throw(Miro::ETimeOut) {
    ACE_Time_Value timeout(ACE_OS::gettimeofday() + maxWait);
    Miro::Guard guard(mutex);
    if (cond.wait(&timeout) == -1)
      throw Miro::ETimeOut();
    return status.velocity;
  }

  //---------------------------------------------------------------------------
  // protected methods
  //---------------------------------------------------------------------------
  inline
  void
  Status::setTransformation() {
    sinHeading = sin(origin.heading);
    cosHeading = cos(origin.heading);
  }
};
#endif




