/*
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id$
 */

#ifndef GpsImpl_h
#define GpsImpl_h

#include "Parameters.h"
#include "idl/GpsS.h"
#include "idl/GpsDataC.h"
#include "idl/WorldPoint3DC.h"

#include "miro/DevConsumer.h"
#include "miro/Synch.h"
#include "miro/Thread.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;
}

namespace GPS
{
  // forward declarations
  class Message;
  class GpsImpl;

  class GpsDispatcher : public Miro::Thread
  {
    typedef Miro::Thread Super;
  public:
    GpsDispatcher(Miro::StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setPosition(const Miro::GpsPositionEventIDL& _position);
    void setDilution(const Miro::GpsDilutionEventIDL& _dilution);
    void setSentence(const Miro::GpsSentenceEventIDL& _sentence);
    void dispatch();

  protected:
    //! Supplier for events.
    Miro::StructuredPushSupplier * supplier_;

    //! Lock
    Miro::Mutex mutex_;
    //! Condition for triggering the dispatcher thread.
    Miro::Condition cond_;

    //! Preinitialized data structure for GPS position event.
    CosNotification::StructuredEvent positionEvent_;
    //! Position updated since last dispatch() call?
    bool newPosition;
    //! Preinitialized data structure for GPS dilution event.
    CosNotification::StructuredEvent dilutionEvent_;
    //! Dilution updated since last dispatch() call?
    bool newDilution;
    //! Preinitialized data structure for GPS NMEA sentence event.
    CosNotification::StructuredEvent sentenceEvent_;
    //! NMEA sentence updated since last dispatch call?
    bool newSentence;

    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend class GpsImpl;
  };

  //! Implementation of the GPS interface.
  /**
   * This class offers a generic implementation for the GPS
   * interface. Simply pass the lowlevel GPS data through the
   * respective integrate*() method and the rest will be taken
   * care of for you.
   *
   * @author Hans Utz, Ingo van Lil
   */
  class GpsImpl : public virtual POA_Miro::Gps,
		  public Miro::DevConsumer
  {
  public:
    GpsImpl(const Parameters& _parameters = *Parameters::instance(),
	      Miro::StructuredPushSupplier * _supplier = NULL,
	      bool _asynchDispatching = false);
    virtual ~GpsImpl();

    virtual void integratePosition(
	const Miro::GpsGlobalPositionEventIDL& _position);
    virtual void integrateDilution(const Miro::GpsDilutionEventIDL& _dilution);
    virtual void handleMessage(const Miro::DevMessage * _message);

    //! GPS interface method implementation.
    virtual Miro::GpsGlobalPositionEventIDL getGlobalPosition()
	    throw ();
    //! GPS interface method implementation.
    virtual Miro::GpsGlobalPositionEventIDL getWaitGlobalPosition()
	    throw (Miro::ETimeOut);
    //! GPS interface method implementation.
    virtual Miro::GpsDilutionEventIDL getDilution()
	    throw ();
    //! GPS interface method implementation.
    virtual Miro::GpsDilutionEventIDL getWaitDilution()
	    throw (Miro::ETimeOut);
    //! GPS interface method implementation.
    virtual void setPositionReference(const Miro::GpsPositionIDL& reference)
	    throw (Miro::EOutOfBounds);
    //! GPS interface method implementation.
    virtual Miro::GpsPositionIDL getPositionReference()
	    throw ();
    //! GPS interface method implementation.
    virtual void setPosition(const Miro::WorldPoint3DIDL& position)
	    throw (Miro::EDevIO, Miro::EOutOfBounds);
    //! GPS interface method implementation.
    virtual Miro::GpsRelativePositionEventIDL getPosition()
	    throw ();
    //! GPS interface method implementation.
    virtual Miro::GpsRelativePositionEventIDL getWaitPosition()
	    throw (Miro::ETimeOut);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    //! Reference to service parameters
    const Parameters& parameters_;

    //! Supplier for events.
    Miro::StructuredPushSupplier * supplier_;

    //! Lock for position_
    Miro::Mutex pos_mutex_;
    //! Condition for getWaitPosition()
    Miro::Condition pos_cond_;
    //! Lock for dilution_
    Miro::Mutex dop_mutex_;
    //! Condition for getWaitDilution()
    Miro::Condition dop_cond_;
    //! Last position update
    Miro::GpsPositionEventIDL position_;
    //! Last dilution update
    Miro::GpsDilutionEventIDL dilution_;

    //! Flag to indicate asynchronous event dispatching
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events
    GpsDispatcher dispatcherThread_;

    //! Timeout for dispatching thread condition
    static ACE_Time_Value maxWait_;

    //! Lock for reference_
    Miro::Mutex ref_mutex_;
    //! Position reference coordinates
    Miro::GpsPositionIDL reference_;
    //! Valid reference latitude range (about -85 .. 85 deg)
    static const double REF_MAX_LAT;
    //! Meters per longitude (Equator) or latitude radian
    static const double METERS_PER_RAD;

    //-------------------------------------------------------------------------
    // helper functions
    //-------------------------------------------------------------------------
    void handleGGA(const Message * pmsg);
    void handleGLL(const Message * pmsg);
    void handleGSA(const Message * pmsg);
    void handleRMC(const Message * pmsg);

    int parseInteger(const std::string& s);
    double parseDouble(const std::string& s);
    double parseLatitude(const std::string& s, const std::string& ns);
    double parseLongitude(const std::string& s, const std::string& ew);
  };
}

#endif // GpsImpl_h
