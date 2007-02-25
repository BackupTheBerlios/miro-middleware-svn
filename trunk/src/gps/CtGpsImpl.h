// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
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
//
// Authors: 
//   Ingo van Lil
//
#ifndef CtGpsImpl_h
#define CtGpsImpl_h

#include "gps/Parameters.h"
#include "GpsImpl.h"
#include "GpsConnection.h"
#include "idl/CtGpsS.h"
#include "idl/GpsDataC.h"

#include "miro/DevConsumer.h"
#include "miro/DevMessage.h"
#include "miro/Synch.h"
#include "miro/Thread.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace GPS
{
  //! Implementation of the CtGps interface.
  /**
   * This class is derived from GpsImpl and implements the additional
   * interface methods defined by the CtGps interface.
   *
   * @author Ingo van Lil
   */
  class CtGpsImpl : public virtual POA_Miro::CtGps,
		    public GpsImpl
  {
    typedef GpsImpl Super;
  public:
    CtGpsImpl(const Parameters& _parameters = *Parameters::instance(),
	      Miro::StructuredPushSupplier * _supplier = NULL,
	      bool _asynchDispatching = false);
    void setConnection(Connection * _connection);

    //! Override handleMessage() to intercept ACK messages.
    virtual void handleMessage(const Miro::DevMessage * _message);

    //! CtGps interface method implementation.
    virtual void setDifferentialSource(Miro::CtGps::DifferentialSource source)
	    throw(Miro::EOutOfBounds, Miro::ETimeOut);
    //! CtGps interface method implementation.
    virtual void setAltitudeAidingNever()
	    throw(Miro::ETimeOut);
    //! CtGps interface method implementation.
    virtual void setAltitudeAidingSometimes(CORBA::Float threshold)
	    throw(Miro::ETimeOut);
    //! CtGps interface method implementation.
    virtual void setAltitudeAidingAlways(CORBA::Float altitude)
	    throw(Miro::ETimeOut);
    //! CtGps interface method implementation.
    virtual void setSentencesDefault()
	    throw(Miro::ETimeOut);
    //! CtGps interface method implementation.
    virtual void setSentencesOff()
	    throw(Miro::ETimeOut);
    //! CtGps interface method implementation.
    virtual void setSentenceRate(
	Miro::CtGps::NmeaSentence msg, CORBA::Float rate)
	    throw(Miro::EOutOfBounds, Miro::ETimeOut);
    //! CtGps interface method implementation.
    virtual void saveConfig()
	    throw(Miro::ETimeOut);

  protected:
    //! Connection to use for sending out commands.
    Connection * connection_;
    Miro::Mutex mutex_;
    //! Triggered whenever an ACK message is received.
    Miro::Condition cond_;

    //! Timeout for ACK message
    static ACE_Time_Value maxWait_;
  };
}

#endif // CtGpsImpl_h
