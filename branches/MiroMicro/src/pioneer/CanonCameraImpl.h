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
#ifndef CanonCameraImpl_h
#define CanonCameraImpl_h

#include "idl/CanonCameraS.h"

#include "miro/Exception.h"

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class Exception;
};

namespace Pioneer
{
  // forward declarations
  class Connection;
  class Consumer;
};

namespace Canon
{
  class Answer;
  class Message;

  class CanonCameraImpl :  public virtual POA_Miro::CanonCamera
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    CanonCameraImpl(Pioneer::Connection& _connection,
		    Answer * _pAnswer) throw(Miro::Exception);
    virtual ~CanonCameraImpl();

    virtual void setZoom(short factor)
      throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut);
    virtual short getZoom()
      throw(Miro::EDevIO, Miro::ETimeOut);

    virtual void setFocus(short factor)
      throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut);
    virtual short getFocus()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void autoFocus()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual Miro::FocusRangeIDL getFocusRange()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void setAEoff()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void setAEon()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void setAE(short factor)
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void setIrisAssignment(short factor)
      throw(Miro::EDevIO, Miro::ETimeOut);
    void setAGCGain(short value);
    void setShutterSpeed(short value);

    void setWBauto();
    void setWBmanual();
    void setWBlock();
    void setWBvalue(short value);

    Answer * getAnswer();
    void checkAnswer() throw(Miro::EDevIO,Miro::EOutOfBounds, Miro::ETimeOut);

    //closes the connection and returns control to Remote Controller
    void done();


  protected:
    Answer * pAnswer;

    Pioneer::Connection&         connection; // encapsulating communication to hardware
    
    static ACE_Time_Value maxWait;
    bool initialized;

    void initialize();
    void send(const Canon::Message &msg);
  };

  inline Answer* CanonCameraImpl::getAnswer() {return pAnswer;}
};

#endif




