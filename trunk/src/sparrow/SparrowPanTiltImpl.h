// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef SparrowPanTiltImpl_h
#define SparrowPanTiltImpl_h

#include "miro/SparrowPanTiltS.h"
#include "miro/Synch.h"
#include "miro/TimeHelper.h"
#include "miro/Angle.h"

#include "SparrowParameters.h"

namespace Sparrow
{
  using std::min;
  using std::max;

  // forward declaration
  class Connection;
  class Parameters;

  class  PanTiltImpl : public virtual POA_Miro::SparrowPanTilt
  {
  public:
    //Constructor 
    PanTiltImpl(Connection& _connection);
  
    //Destructor 
    virtual ~PanTiltImpl();
  
    //  virtual Miro::PanTiltPositionIDL getPosition() 
    //    throw (Miro::EDevIO, Miro::ETimeOut);
    //  virtual void setPosition(const Miro::PanTiltPositionIDL& dest)
    //    throw (Miro::EOutOfBounds,Miro::EDevIO);

    virtual void setPan(CORBA::Double value) 
      throw (Miro::EDevIO, Miro::EOutOfBounds);
    virtual CORBA::Double getPan() throw (Miro::EDevIO);

    //  virtual void setTilt(CORBA::Double value) 
    //    throw (Miro::EDevIO, Miro::EOutOfBounds);
    //  virtual CORBA::Double getTilt() throw (Miro::EDevIO);

    virtual CORBA::Boolean panning() throw();
    virtual Miro::PanPositionIDL currentPan() throw();

    //  virtual CORBA::Boolean tilting();

  protected:
    bool prvPanning();
    //  bool prvTilting() const;
    void updateAccuracy();

    Miro::PanPositionIDL currentPosition();

    Connection&              connection;
    const Parameters&        params_;

    Miro::Mutex              mutex;
    double lastPosition;
    double nextPosition;
    double actPosition;
    double accuracy;

    ACE_Time_Value timeLastSet;
  };

  inline
  void PanTiltImpl::updateAccuracy() {
    ACE_Time_Value t = ACE_OS::gettimeofday();
    double delta = 1000.0 * ( t - timeLastSet ).sec() +
                   ( t - timeLastSet ).usec() / 1000.0;
    double move = 0.5 * delta / params_.panMSecPerRad;
    accuracy -= move * 0.8;
    if( nextPosition > actPosition )
       actPosition = min( nextPosition, actPosition + move );
    else
       actPosition = max( nextPosition, actPosition - move );
    //    cout << "Pan: " << actPosition << " " << accuracy << endl;
    timeLastSet = t;
  }

  inline
  bool 
  PanTiltImpl::prvPanning()
  {
    updateAccuracy();
    return accuracy * params_.panMSecPerRad > -params_.panSwing; 
  }

  /*
  inline
  bool
  PanTiltImpl::prvTilting() const
  {
    return ((ACE_OS::gettimeofday() - timeLastSet).msec() * params_.tiltRadPerMSec <
	    fabs(nextPosition.tiltvalue - lastPosition.tiltvalue ));
  }
  */

};

#endif // sparrowPanTiltImpl_hh
