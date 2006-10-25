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

#include <playerc++.h>
#include <playerclient.h>

// forward declarations
class ACE_Reactor;

namespace Miro
{
  class Exception;
}

namespace Player
{
  class PlayerConnection;

  class PlayerPanTiltImpl : public virtual POA_Miro::PanTilt , 
			    public virtual Miro::PanTiltImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    PlayerPanTiltImpl(const Miro::PanTiltParameters& _panTiltParameters, bool _upsideDown = false) throw(Miro::Exception);
    virtual ~PlayerPanTiltImpl();

    //-------------------------------------------------------------------------
    // from pan.idl
    //-------------------------------------------------------------------------
    virtual void setPan(CORBA::Float angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual CORBA::Float getPan()
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from tilt.idl
    //-------------------------------------------------------------------------
    virtual void setTilt(CORBA::Float angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual CORBA::Float getTilt()
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from panTilt.idl
    //-------------------------------------------------------------------------
    virtual Miro::PanTiltPositionIDL getPosition()
      throw(Miro::EDevIO);
    virtual void setPosition(const Miro::PanTiltPositionIDL & dest) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    

    void setPlayerConnection(PlayerConnection * _playerConnection);


  protected:

    PlayerConnection * playerConnection_;
    
    static ACE_Time_Value maxWait;
    double currentPan;
    double currentTilt;
    double panMinSpeed;
    double panMaxSpeed;
    double tiltMinSpeed;
    double tiltMaxSpeed;
    bool upsideDown;

  };
}

#endif




