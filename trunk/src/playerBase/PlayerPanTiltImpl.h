// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PlayerPanTiltImpl_h
#define PlayerPanTiltImpl_h

#include "idl/PanTiltS.h"
#include "miro/PanTiltImpl.h"

#include "miro/Exception.h"

// forward declarations
class PtzProxy;
class ACE_Reactor;
namespace Miro
{
  class Exception;
};

namespace Player
{
  class PlayerPanTiltImpl : public virtual POA_Miro::PanTilt , Miro::PanTiltImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    PlayerPanTiltImpl(Miro::PanTiltParameters _panTiltParameters, bool _upsideDown = false) throw(Miro::Exception);
    virtual ~PlayerPanTiltImpl();

    //-------------------------------------------------------------------------
    // from pan.idl
    //-------------------------------------------------------------------------
    virtual void setPan(double angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual double getPan()
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from tilt.idl
    //-------------------------------------------------------------------------
    virtual void setTilt(double angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual double getTilt()
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from panTilt.idl
    //-------------------------------------------------------------------------
    virtual Miro::PanTiltPositionIDL getPosition()
      throw(Miro::EDevIO);
    virtual void setPosition(const Miro::PanTiltPositionIDL & dest) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    

    void setPlayerPTZProxy(PtzProxy * _playerPTZ);


  protected:

    PtzProxy * playerPTZ;
    
    static ACE_Time_Value maxWait;
    double currentPan;
    double currentTilt;
    double panMinSpeed;
    double panMaxSpeed;
    double tiltMinSpeed;
    double tiltMaxSpeed;
    double minPan;
    double maxPan;
    double minTilt;
    double maxTilt;
    bool upsideDown;

  };

};

#endif




