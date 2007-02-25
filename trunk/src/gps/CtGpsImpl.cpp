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
#include "CtGpsImpl.h"
#include "GpsMessage.h"
#include "miro/Log.h"
#include <sstream>
#include <string>

namespace GPS
{
  /** Maximum time to wait for ACK. */
  ACE_Time_Value CtGpsImpl::maxWait_(5, 0);

  CtGpsImpl::CtGpsImpl(const Parameters& _parameters,
			Miro::StructuredPushSupplier * _supplier,
			bool _asynchDispatching) :
    Super(_parameters, _supplier, _asynchDispatching),
    connection_(NULL),
    mutex_(),
    cond_(mutex_)
  {
    MIRO_LOG_CTOR("GPS::CtGpsImpl");
    MIRO_LOG_CTOR_END("GPS::CtGpsImpl");
  }

  void CtGpsImpl::setConnection(Connection * _connection)
  {
    connection_ = _connection;
  }

  void CtGpsImpl::handleMessage(const Miro::DevMessage * _message)
  {
    const Message * pmsg = static_cast<const GPS::Message *>(_message);

    if (pmsg->fields() == 1 && pmsg->field(0) == ">") {
      // ACK message
      Miro::Guard guard(mutex_);
      cond_.broadcast();
    } else
      // Leave evalutation to base class
      GpsImpl::handleMessage(_message);
  }

  void CtGpsImpl::setDifferentialSource(Miro::CtGps::DifferentialSource source)
	throw(Miro::EOutOfBounds, Miro::ETimeOut)
  {
    std::string s;
    switch (source) {
      case Miro::CtGps::THIS:   s = "THIS"; break;
      case Miro::CtGps::OTHER:  s = "OTHER"; break;
      case Miro::CtGps::BEACON: s = "BEACON"; break;
      case Miro::CtGps::LBAND:  s = "LBAND"; break;
      case Miro::CtGps::WAAS:   s = "WAAS"; break;
      case Miro::CtGps::NONE:   s = "NONE"; break;
      default:
	throw Miro::EOutOfBounds();
    }

    Miro::Guard guard(mutex_);

    std::ostringstream outs;
    outs << "$JDIFF," << s;
    connection_->send(outs.str());

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();
  }

  void CtGpsImpl::setAltitudeAidingNever()
	throw(Miro::ETimeOut)
  {
    Miro::Guard guard(mutex_);

    connection_->send("$JALT,NEVER,0");

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();
  }

  void CtGpsImpl::setAltitudeAidingSometimes(CORBA::Float threshold)
	throw(Miro::ETimeOut)
  {
    Miro::Guard guard(mutex_);

    std::ostringstream outs;
    outs << "$JALT,SOMETIMES," << threshold;
    connection_->send(outs.str());

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();
  }

  void CtGpsImpl::setAltitudeAidingAlways(CORBA::Float altitude)
	throw(Miro::ETimeOut)
  {
    Miro::Guard guard(mutex_);

    std::ostringstream outs;
    outs << "$JALT,ALWAYS," << altitude;
    connection_->send(outs.str());

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();
  }

  void CtGpsImpl::setSentencesDefault()
	throw(Miro::ETimeOut)
  {
    // Not using $J4STRING because it automatically saves the config.
    setSentencesOff();
    setSentenceRate(Miro::CtGps::GPGGA, 1);
    setSentenceRate(Miro::CtGps::GPGSA, 1);
  }

  void CtGpsImpl::setSentencesOff()
	throw(Miro::ETimeOut)
  {
    Miro::Guard guard(mutex_);

    connection_->send("$JOFF");

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();
  }

  void CtGpsImpl::setSentenceRate(
      Miro::CtGps::NmeaSentence msg, CORBA::Float rate)
	throw(Miro::EOutOfBounds, Miro::ETimeOut)
  {
    switch (msg) {
      case Miro::CtGps::GPGSA:
      case Miro::CtGps::GPGST:
      case Miro::CtGps::GPGSV:
      case Miro::CtGps::GPRRE:
	    if (rate != 0 && rate != 1)
	      throw Miro::EOutOfBounds();
	    break;

      case Miro::CtGps::GPGGA:
      case Miro::CtGps::GPGLL:
      case Miro::CtGps::GPRMC:
      case Miro::CtGps::GPVTG:
      case Miro::CtGps::GPZDA:
	    if (rate != 0 && rate != 0.2 && rate != 1 && rate != 5)
	      throw Miro::EOutOfBounds();
	    break;

      default:
	    throw Miro::EOutOfBounds();
    }

    Miro::Guard guard(mutex_);

    std::ostringstream outs;
    outs << "$JASC,";
    outs << ( msg == Miro::CtGps::GPGGA? "GPGGA" :
	      msg == Miro::CtGps::GPGLL? "GPGLL" :
	      msg == Miro::CtGps::GPGSA? "GPGSA" :
	      msg == Miro::CtGps::GPGST? "GPGST" :
	      msg == Miro::CtGps::GPGSV? "GPGSV" :
	      msg == Miro::CtGps::GPRMC? "GPRMC" :
	      msg == Miro::CtGps::GPRRE? "GPRRE" :
	      msg == Miro::CtGps::GPVTG? "GPVTG" : "GPZDA" );
    outs << "," << rate;
    connection_->send(outs.str());

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();
  }

  void CtGpsImpl::saveConfig()
	throw(Miro::ETimeOut)
  {
    Miro::Guard guard(mutex_);

    connection_->send("$JSAVE");

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();
  }
}

