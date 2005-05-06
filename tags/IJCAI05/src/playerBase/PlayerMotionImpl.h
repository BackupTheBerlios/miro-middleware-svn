// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PlayerMotionImpl_h
#define PlayerMotionImpl_h

#include "miro/MotionImpl.h"
#include "idl/MotionS.h"

class PositionProxy;

namespace Miro
{
 
  //! Simulator implementation for the generalized Motion interface.
  /**
   * This class provides support for the set method
   * of the generalized Motion interface for the Player simulator
   */
  class PlayerMotionImpl : public virtual POA_Miro::Motion, Miro::MotionImpl
  {
    typedef Miro::MotionImpl Super;
  public:
    //! Initializing constructor.
    PlayerMotionImpl(const MotionParameters& _params);

    void setPlayerPositionProxy(PositionProxy* _playerPosition);

    virtual void limp() throw(EDevIO);

    virtual void setVelocity(const VelocityIDL& velocity) throw(EOutOfBounds, EDevIO);

  protected:
    PositionProxy* playerPositionProxy;

  };

};

#endif // PlayerMotionImpl_h
