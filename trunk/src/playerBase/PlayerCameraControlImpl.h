// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#ifndef PlayerCameraControlImpl_h
#define PlayerCameraControlImpl_h

#include "miro/CameraControlImpl.h"

#include "miro/Exception.h"
#include "miro/SvcParameters.h"
#include "miro/TimeHelper.h"
#include <cmath>
#include <playerc++.h>
#include <playerclient.h>

class PlayerConnection;

namespace Player
{

  //! Implementation of a Player CameraControl interface.
  /**
   * This class offers an implementation for the CameraControl interface
   *  for Player.
   * 
   * It only implements zoom functionality. The other interface functions
   *  are inherited from the default 
   *
   * @author Guillem Pagès Gassull
   */
  class  CameraControlImpl : public virtual POA_Miro::CameraControl,
			     public virtual Miro::CameraControlImpl
  {
  public:
    typedef Miro::CameraControlImpl Super;

    //! Initializing constructor.
    CameraControlImpl(const Miro::CameraControlParameters& _cameraParameters);
    virtual ~CameraControlImpl();


    //! CameraControl interface method implementation.
    virtual void setZoom(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual float getZoom() throw(Miro::EDevIO, Miro::ETimeOut);


    void setPlayerConnection(PlayerConnection * _playerConnection);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    
    PlayerConnection * playerConnection_;

  };

}

#endif // PlayerCameraControlImpl_h
