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

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/Angle.h"
#include "miro/Log.h"

#include "PlayerPanTiltImpl.h"

#include <iostream>

#include <playerclient.h>

namespace Player
{
  using std::cout;
  using std::cerr;
  using std::endl;

  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;
  using Miro::Exception;
  using Miro::rad2Deg;
  using Miro::deg2Rad;
  //  using Miro::PlayerPanTiltSpdAccIDL;
  //  using Miro::PlayerPanTiltLimitsIDL;

  // maximum wait time for cond.wait calls
  ACE_Time_Value PlayerPanTiltImpl::maxWait = ACE_Time_Value(0, 500000);

  PlayerPanTiltImpl::PlayerPanTiltImpl( bool _upsideDown) throw(Exception) :
    playerPTZ(NULL),
    currentPan(0),
    currentTilt(0),
    panMinSpeed(-1),
    panMaxSpeed(-1),
    tiltMinSpeed(-1),
    tiltMaxSpeed(-1),
    minPan(-1e10),
    maxPan(-1e10),
    minTilt(-1e10),
    maxTilt(-1e10),
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
  PlayerPanTiltImpl::setPan(double angle) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL dest;

    //if camera is inverse mounted, change sign for current saved value
    //as it will be changed again on setPosition
    dest.panvalue = angle;
    dest.tiltvalue = (upsideDown? -currentTilt : currentTilt);

    setPosition(dest);
  }
  
  double
  PlayerPanTiltImpl::getPan() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL pos = getPosition();
    return pos.panvalue;
    
  }
  
  //-------------------------------------------------------------------------
  // from tilt.idl
  //-------------------------------------------------------------------------
  void 
  PlayerPanTiltImpl::setTilt(double angle) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL dest;
    //if camera is not inverse mounted, change sign for current saved value
    //as it will be changed again on setPosition
    //to maintain coherence with other PanTilt implementations it must
    //work opposite to camera default.
    dest.panvalue=(!upsideDown?-currentPan:currentPan);
    dest.tiltvalue=angle;
    setPosition(dest);
  }

  double 
  PlayerPanTiltImpl::getTilt() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL pos = getPosition();
    return pos.tiltvalue;

  }
  
  //-------------------------------------------------------------------------
  // from panTilt.idl
    //-------------------------------------------------------------------------
  Miro::PanTiltPositionIDL 
  PlayerPanTiltImpl::getPosition() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL result;

    result.tiltvalue=playerPTZ->tilt;
    result.panvalue=playerPTZ->pan;

    if (upsideDown) {
      //if inverse mounting, switch directions
      result.tiltvalue = -result.tiltvalue;
    } else {
      //Miro's horitzontal standart is opposite to camera's !!!
      result.panvalue = -result.panvalue;
    }

    return result;
  }

  void 
  PlayerPanTiltImpl::setPosition(const Miro::PanTiltPositionIDL & dest) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    //if camera in inverse position, change sign for pan/tilt
    //Miro's pan works opposite to camera's!!!
    currentPan = dest.panvalue * (!upsideDown? -1 : 1);
    currentTilt = dest.tiltvalue* (upsideDown? -1 : 1);
    
    playerPTZ->SetCam(currentPan,currentTilt,playerPTZ->zoom);
  }

  void
  PlayerPanTiltImpl::setPlayerPTZProxy(PtzProxy * _playerPTZ)
  {
    playerPTZ = _playerPTZ;
  }
  
};
