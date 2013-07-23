// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef CanonPanTiltImpl_hh
#define CanonPanTiltImpl_hh

#include "idl/CanonPanTiltS.h"

#include "miro/PanTiltImpl.h"

#include "miro/Exception.h"
#include "pioneer/PioneerConnection.h"

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class Exception;
}

namespace Pioneer
{
  // forward declarations
  class Consumer;
}

namespace Canon
{
  class CanonPanTiltImpl :  public virtual POA_Miro::CanonPanTilt,
			    public virtual Miro::PanTiltImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    CanonPanTiltImpl(Pioneer::Connection& _connection,
		     Answer * _pAnswer,
		     Miro::PanTiltParameters _panTiltParameters,
		     bool _upsideDown = false) throw(Miro::Exception);
    virtual ~CanonPanTiltImpl();

    //-------------------------------------------------------------------------
    // from pan.idl
    //-------------------------------------------------------------------------
    virtual void setPan(CORBA::Float angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual CORBA::Float getPan()
      throw(Miro::EDevIO);
    /**
     * returns the pan angle limits
     */
    virtual Miro::PanLimitsIDL getPanLimits() throw();

    //-------------------------------------------------------------------------
    // from tilt.idl
    //-------------------------------------------------------------------------
    virtual void setTilt(CORBA::Float angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual CORBA::Float getTilt()
      throw(Miro::EDevIO);
    /**
     * returns the tilt angle limits
     */
    virtual Miro::TiltLimitsIDL getTiltLimits() throw();

    //-------------------------------------------------------------------------
    // from panTilt.idl
    //-------------------------------------------------------------------------
    virtual Miro::PanTiltPositionIDL getPosition()
      throw(Miro::EDevIO);
    virtual void setPosition(const Miro::PanTiltPositionIDL & dest) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual Miro::PanTiltLimitsIDL getPanTiltLimits() throw();

    //-------------------------------------------------------------------------
    // from canonPanTilt.idl
    //-------------------------------------------------------------------------
    virtual void waitCompletion()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void setWaitPosition(const Miro::PanTiltPositionIDL & dest) 
      throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut);
    virtual void setSpdAcc(const Miro::CanonPanTiltSpdAccIDL & dest)
      throw(Miro::ETimeOut, Miro::EDevIO, Miro::EOutOfBounds);
    virtual Miro::CanonPanTiltSpdAccIDL getSpdAcc()
      throw(Miro::EDevIO, Miro::ETimeOut);
    /**
     * DEPRECATED.
     * Use getPanTiltLimits() instead
     */
    virtual Miro::PanTiltLimitsIDL getLimits()
      throw(Miro::EDevIO, Miro::ETimeOut);
   
    void checkAnswer() throw(Miro::EDevIO,Miro::EOutOfBounds, Miro::ETimeOut);

  protected:
    Answer * pAnswer;

    Pioneer::Connection&         connection; // encapsulating communication to hardware
    
    static ACE_Time_Value maxWait;
    bool initialized;
    double panPulseRatio;
    double tiltPulseRatio;
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

    double getPanPulseRatio();
    double getTiltPulseRatio();
    void initialize();
    void waitInitialize(bool force=false,bool forceWait=false);
    void send(const Canon::Message &msg);

  };
  
}

#endif




