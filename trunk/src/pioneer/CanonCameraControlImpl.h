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
#ifndef CanonCameraControlImpl_h
#define CanonCameraControlImpl_h

#include "idl/CanonCameraControlS.h"
#include "miro/CameraControlImpl.h"
#include "miro/Exception.h"
#include "miro/SvcParameters.h"

#include "pioneer/CanonMessage.h"
#include "pioneer/Parameters.h"

//forward declarations
namespace Pioneer
{
  class Connection;
}

namespace Canon
{
  class Answer;

  //! Implementation of a canon CameraControl interface.
  /**
   * This class offers a generic implementation for the CameraControl
   *
   * @author Guillem Pagès Gassull
   */
  class  CanonCameraControlImpl : public virtual POA_Miro::CanonCameraControl, public Miro::CameraControlImpl
  {
    typedef Miro::CameraControlImpl Super; 
  public:
    //! Initializing constructor.
    CanonCameraControlImpl(Pioneer::Parameters * _parameters, 
			   Pioneer::Connection& connection,
			   Canon::Answer * _pAnswer
			   ) throw(Miro::Exception);
    virtual ~CanonCameraControlImpl();


    //! CameraControl interface method implementation.
    virtual void setZoom(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual float getZoom() throw(Miro::EDevIO, Miro::ETimeOut);

    //! CameraControl interface method implementation.
    virtual void setFocus(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual float getFocus() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual Miro::FocusRangeIDL getFocusRange() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual void setAutoFocus(CORBA::Boolean value) throw(Miro::EDevIO, Miro::ETimeOut);

    //! CameraControl interface method implementation.
    virtual void setShutter(Miro::TimeIDL & value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual Miro::TimeIDL getShutter() throw(Miro::EDevIO, Miro::ETimeOut);

    //! CameraControl interface method implementation.
    virtual void setAELock(CORBA::Boolean value) throw(Miro::EDevIO, Miro::ETimeOut);

    /*    virtual void setAEoff()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void setAEon()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void setAE(short factor)
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void setIrisAssignment(short factor)
      throw(Miro::EDevIO, Miro::ETimeOut);
    void setAGCGain(short value);

    void setWBauto();
    void setWBmanual();
    void setWBlock();
    void setWBvalue(short value);
    */


    Canon::Answer * getAnswer();
    void checkAnswer() throw(Miro::EDevIO,Miro::EOutOfBounds, Miro::ETimeOut);

    //closes the connection and returns control to Remote Controller
    void done();


  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    Pioneer::Parameters * parameters_;

    Pioneer::Connection&         connection; // encapsulating communication to hardware
    Answer * pAnswer;
    
    static ACE_Time_Value maxWait;
    bool initialized;

    Miro::TimeIDL shutterCanon2Time(short value, std::string format);

    void initialize();
    void send(const Canon::Message &msg);
  };

  inline Canon::Answer* CanonCameraControlImpl::getAnswer() {return pAnswer;}

};

#endif // CameraControlImpl_h
