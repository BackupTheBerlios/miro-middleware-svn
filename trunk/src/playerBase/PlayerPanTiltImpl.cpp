// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/Angle.h"
#include "miro/Log.h"

#include "PlayerConnection.h"
#include "PlayerPanTiltImpl.h"

namespace Player
{
  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;
  using Miro::Exception;
  using Miro::rad2Deg;
  using Miro::deg2Rad;
  using Miro::PanTiltLimitsIDL;
  using Miro::PanLimitsIDL;
  using Miro::TiltLimitsIDL;

  // maximum wait time for cond.wait calls
  ACE_Time_Value PlayerPanTiltImpl::maxWait = ACE_Time_Value(0, 500000);

  PlayerPanTiltImpl::PlayerPanTiltImpl(const Miro::PanTiltParameters& _panTiltParameters,
				       bool _upsideDown) throw(Exception) :
    Miro::PanTiltImpl(_panTiltParameters),
    playerConnection_(NULL),
    currentPan(0),
    currentTilt(0),
    panMinSpeed(-1),
    panMaxSpeed(-1),
    tiltMinSpeed(-1),
    tiltMaxSpeed(-1),
    upsideDown(_upsideDown)
  {
    MIRO_LOG(LL_NOTICE,"Constructing PlayerPanTiltImpl");
  }
  
  PlayerPanTiltImpl::~PlayerPanTiltImpl()
  {
    MIRO_LOG(LL_NOTICE,"Destructing PlayerPanTiltImpl");
  }
  
  // 
  // IDL interface implementation


  //-------------------------------------------------------------------------
  // from pan.idl
  //-------------------------------------------------------------------------
  void
  PlayerPanTiltImpl::setPan(CORBA::Float angle) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL dest;

    if (!testPan(angle)) {
      throw Miro::EOutOfBounds();
    }

    //if camera is not inverse mounted, change sign for current saved value
    //as it will be changed again on setPosition
    dest.panValue = angle;
    dest.tiltValue = (upsideDown? currentTilt : -currentTilt);
  
    setPosition(dest);
  }
  
  CORBA::Float
  PlayerPanTiltImpl::getPan() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL pos = getPosition();
    return pos.panValue;
    
  }
  
  //-------------------------------------------------------------------------
  // from tilt.idl
  //-------------------------------------------------------------------------
  void 
  PlayerPanTiltImpl::setTilt(CORBA::Float angle) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL dest;

    if (!testTilt(angle)) {
      throw Miro::EOutOfBounds();
    }

    //if camera is inverse mounted, change sign for current saved value
    //as it will be changed again on setPosition
    dest.panValue=(upsideDown?-currentPan:currentPan);
    dest.tiltValue=angle;

    setPosition(dest);
  }

  CORBA::Float 
  PlayerPanTiltImpl::getTilt() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL pos = getPosition();
    return pos.tiltValue;

  }
  
  //-------------------------------------------------------------------------
  // from panTilt.idl
    //-------------------------------------------------------------------------
  Miro::PanTiltPositionIDL 
  PlayerPanTiltImpl::getPosition() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL result;

    result.tiltValue=playerConnection_->getTilt();
    result.panValue=playerConnection_->getPan();

    if (upsideDown) {
      //if inverse mounting, switch directions
      result.panValue = -result.panValue;
    } else {
      //Miro's vertical standard is opposite to camera's !!!
      result.tiltValue = -result.tiltValue;
    }

    return result;
  }

  void 
  PlayerPanTiltImpl::setPosition(const Miro::PanTiltPositionIDL & dest) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    if (!testPosition(dest)) {
      throw Miro::EOutOfBounds();
    }

    //if camera in inverse position, change sign for pan/tilt
    //Miro's tilt works opposite to camera's!!!
    currentPan = dest.panValue *  (upsideDown? -1 :  1);
    currentTilt = dest.tiltValue* (upsideDown?  1 : -1);
    
    if (!playerConnection_->setPanTilt(currentPan,currentTilt)) {
      throw Miro::EDevIO("Error on Player PanTilt device.");
    } else {
      setTargetPosition(dest);
    }
  }

  void
  PlayerPanTiltImpl::setPlayerConnection(PlayerConnection * _playerConnection)
  {
    playerConnection_ = _playerConnection;

    //set a reasonable initial value. Namely the values the pantilt has.
    setPosition(getPosition());
  }
  
};
