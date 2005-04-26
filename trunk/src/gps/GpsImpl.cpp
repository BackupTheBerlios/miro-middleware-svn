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

#include "GpsImpl.h"
#include "GpsMessage.h"
#include "miro/TimeHelper.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/Angle.h"
#include "miro/Log.h"

#include <string>
#include <cmath>

namespace GPS
{
  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value GpsDispatcher::maxWait_(0, 100000);

  GpsDispatcher::GpsDispatcher(Miro::StructuredPushSupplier * _supplier) :
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_),
    newPosition(false),
    newDilution(false),
    newSentence(false)
  {
    MIRO_LOG_CTOR("GPS::GpsDispatcher");
    if (supplier_) {
      // Status Notify Event initialization
      positionEvent_.header.fixed_header.event_type.domain_name =
	CORBA::string_dup(supplier_->domainName().c_str());
      positionEvent_.header.fixed_header.event_type.type_name =
	CORBA::string_dup("GpsPosition");
      positionEvent_.header.fixed_header.event_name = CORBA::string_dup("");
      positionEvent_.header.variable_header.length(0);   // put nothing here
      positionEvent_.filterable_data.length(0);          // put nothing here

      // Dilution event initialization
      dilutionEvent_.header.fixed_header.event_type.domain_name =
	CORBA::string_dup(supplier_->domainName().c_str());
      dilutionEvent_.header.fixed_header.event_type.type_name =
	CORBA::string_dup("GpsDilution");
      dilutionEvent_.header.fixed_header.event_name = CORBA::string_dup("");
      dilutionEvent_.header.variable_header.length(0);   // put nothing here
      dilutionEvent_.filterable_data.length(0);          // put nothing here

      // Dilution event initialization
      sentenceEvent_.header.fixed_header.event_type.domain_name =
	CORBA::string_dup(supplier_->domainName().c_str());
      sentenceEvent_.header.fixed_header.event_type.type_name =
	CORBA::string_dup("GpsSentence");
      sentenceEvent_.header.fixed_header.event_name = CORBA::string_dup("");
      sentenceEvent_.header.variable_header.length(0);   // put nothing here
      sentenceEvent_.filterable_data.length(0);          // put nothing here

      CosNotification::EventTypeSeq offers;
      offers.length(3);
      offers[0] = positionEvent_.header.fixed_header.event_type;
      offers[1] = dilutionEvent_.header.fixed_header.event_type;
      offers[2] = sentenceEvent_.header.fixed_header.event_type;
      supplier_->addOffers(offers);
    }
    MIRO_LOG_CTOR_END("GPS::GpsDispatcher");
  }

  void
  GpsDispatcher::setPosition(const Miro::GpsPositionEventIDL& _position)
  {
    newPosition = true;
    positionEvent_.remainder_of_body <<= _position;
  };

  void
  GpsDispatcher::setDilution(const Miro::GpsDilutionEventIDL& _dilution)
  {
    newDilution = true;
    dilutionEvent_.remainder_of_body <<= _dilution;
  }

  void
  GpsDispatcher::setSentence(const Miro::GpsSentenceEventIDL& _sentence)
  {
    newSentence = true;
    sentenceEvent_.remainder_of_body <<= _sentence;
  }

  int
  GpsDispatcher::svc()
  {
    MIRO_LOG(LL_NOTICE, "Asynchronous Compass dispatching");

    while(!canceled()) {
      Miro::Guard guard(mutex_);
      ACE_Time_Value timeout(ACE_OS::gettimeofday());
      timeout += maxWait_;
      if (cond_.wait(&timeout) != -1 &&
	  !canceled()) {
	dispatch();
      }
    }

    return 0;
  }

  void
  GpsDispatcher::dispatch()
  {
    if (newPosition)
      supplier_->sendEvent(positionEvent_);
    if (newDilution)
      supplier_->sendEvent(dilutionEvent_);
    if (newSentence)
      supplier_->sendEvent(sentenceEvent_);
    newPosition = false;
    newDilution = false;
    newSentence = false;
  }

  void
  GpsDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Miro::Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }


  /** Maximum wait time for cond_.wait calls */
  ACE_Time_Value GpsImpl::maxWait_(10, 0);

  GpsImpl::GpsImpl(const Parameters& _parameters,
		    Miro::StructuredPushSupplier * _supplier,
		    bool _asynchDispatching) :
    parameters_(_parameters),
    supplier_(_supplier),
    pos_mutex_(),
    pos_cond_(pos_mutex_),
    dop_mutex_(),
    dop_cond_(dop_mutex_),
    asynchDispatching_(_asynchDispatching),
    dispatcherThread_(_supplier),
    ref_mutex_()
  {
    MIRO_LOG_CTOR("GPS::GpsImpl");
    position_.time.sec = 0;
    position_.time.usec = 0;
    position_.global.latitude = NAN;
    position_.global.longitude = NAN;
    position_.global.altitude = NAN;
    position_.relative.x = NAN;
    position_.relative.y = NAN;
    position_.relative.z = NAN;

    dilution_.time.sec = 0;
    dilution_.time.usec = 0;
    dilution_.dilution.hdop = NAN;
    dilution_.dilution.vdop = NAN;
    dilution_.dilution.pdop = NAN;

    reference_.latitude = NAN;
    reference_.longitude = NAN;
    reference_.altitude = NAN;
    MIRO_LOG_CTOR_END("GPS::GpsImpl");
  }

  GpsImpl::~GpsImpl()
  {
    MIRO_LOG_DTOR("GPS::GpsImpl");
    MIRO_LOG_DTOR_END("GPS::GpsImpl");
  }

  void
  GpsImpl::integratePosition(const Miro::GpsGlobalPositionEventIDL& _position)
  {
    { // scope for guard
      Miro::Guard pos_guard(pos_mutex_);
      Miro::Guard ref_guard(ref_mutex_);

      position_.time = _position.time;

      double newlat = _position.global.latitude;
      if (!isnan(newlat)) {
	position_.global.latitude = newlat;
	if (isnan(reference_.latitude))
	  reference_.latitude = newlat;
	position_.relative.y = (newlat - reference_.latitude) * 1000
			     * METERS_PER_RAD;
      }

      double newlon = _position.global.longitude;
      if (!isnan(newlon)) {
	position_.global.longitude = newlon;
	if (isnan(reference_.longitude))
	  reference_.longitude = newlon;
	position_.relative.x = (newlon - reference_.longitude) * 1000
			     * (METERS_PER_RAD * cos(reference_.latitude));
      }

      float newalt = _position.global.altitude;
      if (!isnan(newalt)) {
	position_.global.altitude = newalt;
	if (isnan(reference_.altitude))
	  reference_.altitude = newalt;
	position_.relative.z = (newalt - reference_.altitude) * 1000;
      }

      pos_cond_.broadcast();
    }

    // send event
    if (supplier_) {
      Miro::GpsPositionEventIDL event;

      event.time = _position.time;
      event.global = _position.global;
      event.relative.x = isnan(event.global.longitude)?
			  NAN : position_.relative.x;
      event.relative.y = isnan(event.global.latitude)?
			  NAN : position_.relative.y;
      event.relative.z = isnan(event.global.altitude)?
			  NAN : position_.relative.z;

      if (asynchDispatching_) {
	Miro::Guard guard(dispatcherThread_.mutex_);
	dispatcherThread_.setPosition(event);
	dispatcherThread_.cond_.broadcast();
      }
      else {
	dispatcherThread_.setPosition(event);
	dispatcherThread_.dispatch();
      }
    }
  }

  void
  GpsImpl::integrateDilution(const Miro::GpsDilutionEventIDL& _dilution)
  {
    { // scope for guard
      Miro::Guard guard(dop_mutex_);
      dilution_.time = _dilution.time;
      if (!isnan(_dilution.dilution.hdop))
	dilution_.dilution.hdop = _dilution.dilution.hdop;
      if (!isnan(_dilution.dilution.vdop))
	dilution_.dilution.vdop = _dilution.dilution.vdop;
      if (!isnan(_dilution.dilution.pdop))
	dilution_.dilution.pdop = _dilution.dilution.pdop;
      dop_cond_.broadcast();
    }

    // send event
    if (supplier_) {
      if (asynchDispatching_) {
	Miro::Guard guard(dispatcherThread_.mutex_);
	dispatcherThread_.setDilution(_dilution);
	dispatcherThread_.cond_.broadcast();
      }
      else {
	dispatcherThread_.setDilution(_dilution);
	dispatcherThread_.dispatch();
      }
    }
  }

  void
  GpsImpl::handleMessage(const Miro::DevMessage * _message)
  {
    const Message * pmsg = static_cast<const GPS::Message *>(_message);

    std::string msgtype = pmsg->field(0);
    if (msgtype.length() != 5)
      return;

    std::vector<std::string>::const_iterator i;
    for (i = parameters_.nmeaNotify.begin();
	 i < parameters_.nmeaNotify.end(); i++)
      if (*i == msgtype) {
	Miro::GpsSentenceEventIDL sentence;
	Miro::timeA2C(pmsg->time(), sentence.time);
	const std::string s = pmsg->get();
	const char *c = s.c_str();
	sentence.sentence = CORBA::string_dup(c);
	if (supplier_) {
	  if (asynchDispatching_) {
	    Miro::Guard guard(dispatcherThread_.mutex_);
	    dispatcherThread_.setSentence(sentence);
	    dispatcherThread_.cond_.broadcast();
	  }
	  else {
	    dispatcherThread_.setSentence(sentence);
	    dispatcherThread_.dispatch();
	  }
	}
      }

    if (msgtype == "GPGGA")
      handleGGA(pmsg);

    else if (msgtype == "GPGLL")
      handleGLL(pmsg);

    else if (msgtype == "GPGSA")
      handleGSA(pmsg);

    else if (msgtype == "GPRMC")
      handleRMC(pmsg);

    else
      MIRO_LOG_OSTR(LL_PRATTLE, "Unhandled sentence type: " << msgtype);
  }

  Miro::GpsGlobalPositionEventIDL
  GpsImpl::getGlobalPosition() throw()
  {
    Miro::Guard guard(pos_mutex_);

    Miro::GpsGlobalPositionEventIDL pos;
    pos.time = position_.time;
    pos.global = position_.global;
    return pos;
  }

  Miro::GpsGlobalPositionEventIDL
  GpsImpl::getWaitGlobalPosition() throw (Miro::ETimeOut)
  {
    Miro::Guard guard(pos_mutex_);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (pos_cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();

    Miro::GpsGlobalPositionEventIDL pos;
    pos.time = position_.time;
    pos.global = position_.global;
    return pos;
  }

  Miro::GpsDilutionEventIDL
  GpsImpl::getDilution() throw()
  {
    Miro::Guard guard(dop_mutex_);
    return dilution_;
  }

  Miro::GpsDilutionEventIDL
  GpsImpl::getWaitDilution() throw (Miro::ETimeOut)
  {
    Miro::Guard guard(dop_mutex_);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (dop_cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();

    return dilution_;
  }

  void
  GpsImpl::setPositionReference(const Miro::GpsPositionIDL& reference)
	throw (Miro::EOutOfBounds)
  {
    if (reference.longitude <= -M_PI
       || reference.longitude > M_PI
       || reference.latitude < -REF_MAX_LAT
       || reference.latitude > REF_MAX_LAT)
      throw Miro::EOutOfBounds();

    Miro::Guard guard(ref_mutex_);
    reference_ = reference;
  }

  Miro::GpsPositionIDL
  GpsImpl::getPositionReference() throw()
  {
    Miro::Guard guard(ref_mutex_);
    return reference_;
  }

  void
  GpsImpl::setPosition(const Miro::WorldPoint3DIDL& position)
	throw (Miro::EDevIO, Miro::EOutOfBounds)
  {
    Miro::Guard pos_guard(pos_mutex_);

    // Not ready yet?
    if (isnan(position_.global.latitude) || isnan(position_.global.longitude))
      throw Miro::EDevIO();

    Miro::GpsPositionIDL ref;

    ref.latitude = position_.global.latitude -
		   position.y / 1000 / METERS_PER_RAD;
    if (ref.latitude < -REF_MAX_LAT || ref.latitude > REF_MAX_LAT)
      throw Miro::EOutOfBounds();

    ref.longitude = position_.global.longitude - position.x / 1000 /
		    (METERS_PER_RAD * cos(position_.global.latitude));
    if (ref.longitude <= -M_PI)
      ref.longitude += 2*M_PI;
    else if (ref.longitude > M_PI)
      ref.longitude -= 2*M_PI;

    if (isnan(position_.global.altitude))
      ref.altitude = NAN;
    else
      ref.altitude = position_.global.altitude - position.z / 1000;

    Miro::Guard ref_guard(ref_mutex_);
    reference_ = ref;
    position_.relative = position;
  }

  Miro::GpsRelativePositionEventIDL
  GpsImpl::getPosition() throw()
  {
    Miro::Guard pos_guard(pos_mutex_);

    Miro::GpsRelativePositionEventIDL pos;
    pos.time = position_.time;
    pos.relative = position_.relative;
    return pos;
  }

  Miro::GpsRelativePositionEventIDL
  GpsImpl::getWaitPosition() throw (Miro::ETimeOut)
  {
    Miro::Guard pos_guard(pos_mutex_);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (pos_cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();

    Miro::GpsRelativePositionEventIDL pos;
    pos.time = position_.time;
    pos.relative = position_.relative;
    return pos;
  }


  //---------------------------------------------------------------------------
  // protected methods
  //---------------------------------------------------------------------------

  // Global positioning system fixed data
  void
  GpsImpl::handleGGA(const Message * pmsg)
  {
    if (pmsg->fields() != 15) {
      MIRO_LOG(LL_WARNING, "Incorrect number of fields for GPGGA sentence");
      return;
    } else if (pmsg->field(6) == "0") // fix validity indicator
      return;

    Miro::GpsGlobalPositionEventIDL pos;
    Miro::timeA2C(pmsg->time(), pos.time);
    pos.global.latitude = parseLatitude(pmsg->field(2), pmsg->field(3));
    pos.global.longitude = parseLongitude(pmsg->field(4), pmsg->field(5));
    pos.global.altitude = parseDouble(pmsg->field(9));

    Miro::GpsDilutionEventIDL dop;
    Miro::timeA2C(pmsg->time(), dop.time);
    dop.dilution.hdop = parseDouble(pmsg->field(8));
    dop.dilution.vdop = NAN;
    dop.dilution.pdop = NAN;

    if (isnan(pos.global.latitude)
	|| isnan(pos.global.longitude)
	|| isnan(pos.global.altitude))
      MIRO_LOG(LL_WARNING, "Invalid GPGGA sentence");
    else {
      integratePosition(pos);
      integrateDilution(dop);
    }
  }

  // Geographic position - latitude/longitude
  void
  GpsImpl::handleGLL(const Message * pmsg)
  {
    if (pmsg->fields() != 7) {
      MIRO_LOG(LL_WARNING, "Incorrect number of fields for GPGLL sentence");
      return;
    } else if (pmsg->field(6) != "A") // fix validity indicator
      return;

    Miro::GpsGlobalPositionEventIDL pos;
    Miro::timeA2C(pmsg->time(), pos.time);
    pos.global.latitude = parseLatitude(pmsg->field(1), pmsg->field(2));
    pos.global.longitude = parseLongitude(pmsg->field(3), pmsg->field(4));
    pos.global.altitude = NAN;

    if (isnan(pos.global.latitude) || isnan(pos.global.longitude))
      MIRO_LOG(LL_WARNING, "Invalid GPGLL sentence");
    else
      integratePosition(pos);
  }

  // GNSS DOP and active satellites
  void
  GpsImpl::handleGSA(const Message * pmsg)
  {
    if (pmsg->fields() != 18) {
      MIRO_LOG(LL_WARNING, "Incorrect number of fields for GPGSA sentence");
      return;
    }

    Miro::GpsDilutionEventIDL dop;
    Miro::timeA2C(pmsg->time(), dop.time);
    dop.dilution.pdop = parseDouble(pmsg->field(15));
    dop.dilution.hdop = parseDouble(pmsg->field(16));
    dop.dilution.vdop = parseDouble(pmsg->field(17));

    integrateDilution(dop);
  }

  // Recommended minimum specific GNSS data
  void
  GpsImpl::handleRMC(const Message * pmsg)
  {
    if (pmsg->fields() != 11) {
      MIRO_LOG(LL_WARNING, "Incorrect number of fields for GPRMC sentence");
      return;
    } else if (pmsg->field(2) != "A") // fix validity indicator
      return;

    Miro::GpsGlobalPositionEventIDL pos;
    Miro::timeA2C(pmsg->time(), pos.time);
    pos.global.latitude = parseLatitude(pmsg->field(3), pmsg->field(4));
    pos.global.longitude = parseLongitude(pmsg->field(5), pmsg->field(6));
    pos.global.altitude = NAN;

    if (isnan(pos.global.latitude) || isnan(pos.global.longitude))
      MIRO_LOG(LL_WARNING, "Invalid GPGLL sentence");
    else
      integratePosition(pos);
  }

  int
  GpsImpl::parseInteger(const std::string& s)
  {
    int n = 0;
    for (unsigned int i=0; i<s.size(); i++)
      if (s[i] >= '0' || s[i] <= '9')
	n = 10*n + s[i] - '0';
      else
	return -1;
    return n;
  }

  double
  GpsImpl::parseDouble(const std::string& s)
  {
    double d = 0;
    bool left = true;
    int div = 1;
    for (unsigned int i=0; i<s.size(); i++)
      if (left) {
	if (s[i] == '.')
	  left = false;
	else if (s[i] >= '0' && s[i] <= '9')
	  d = 10*d + s[i] - '0';
	else
	  return NAN;
      } else {
	if (s[i] >= '0' && s[i] <= '9') {
	  div *= 10;
	  d += (double)(s[i] - '0') / div;
	} else
	  return NAN;
      }
    return d;
  }

  /**
   * Parse an NMEA-0183 latitude value
   * @param s: Latitude value, format ddmm.mmmm
   * @param i: N=north, S=south
   * @return Latitude value in radians, \c NAN if invalid
   */
  double
  GpsImpl::parseLatitude(const std::string& s, const std::string& ns)
  {
    if (s.size() < 5 || ns.size() != 1 || s[4] != '.')
      return NAN;
    int deg = parseInteger(s.substr(0, 2));
    double min = parseDouble(s.substr(2, s.length()-2));
    if (deg == -1 || isnan(min))
      return NAN;
    if (ns[0] == 'N')
      return Miro::deg2Rad((double)deg + min/60);
    else if (ns[0] == 'S')
      return -Miro::deg2Rad((double)deg + min/60);
    return NAN;
  }

  /**
   * Parse an NMEA-0183 longitude value
   * @param s: Longitude value, format dddmm.mmmm
   * @param ew: E=east, W=west
   * @return Longitude value in radians, \c NAN if invalid
   */
  double
  GpsImpl::parseLongitude(const std::string& s, const std::string& ew)
  {
    if (s.size() < 6 || ew.size() != 1 || s[5] != '.')
      return NAN;
    int deg = parseInteger(s.substr(0, 3));
    double min = parseDouble(s.substr(3, s.length()-3));
    if (deg == -1 || isnan(min))
      return NAN;
    if (ew[0] == 'E')
      return Miro::deg2Rad((double)deg + min/60);
    else if (ew[0] == 'W')
      return -Miro::deg2Rad((double)deg + min/60);
    return NAN;
  }
}
