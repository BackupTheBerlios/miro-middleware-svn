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
#include "RangeSensorImpl.h"
#include "Exception.h"
#include "idl/ExceptionC.h"
#include "StructuredPushSupplier.h"
#include "miro/Log.h"

namespace Miro
{

  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value RangeSensorDispatcher::maxWait_(0, 100000);

  RangeSensorDispatcher::RangeSensorDispatcher(const ScanDescriptionIDL& _description,
      StructuredPushSupplier * _supplier) :
      supplier_(_supplier),
      mutex_(),
      cond_(mutex_),
      notifyEvent_(1),
      eventPending_(1, false)
  {
    if (supplier_) {
      if (_description.scanType == RangeSensor::GROUPWISE) {
        notifyEvent_.resize(_description.group.length());
        eventPending_.resize(_description.group.length(), false);
      }

      for (unsigned int i = 0; i < notifyEvent_.size(); ++i) {
        // Notify Event initialization
        notifyEvent_[i].header.fixed_header.event_type.domain_name =
          CORBA::string_dup(supplier_->domainName().c_str());
        notifyEvent_[i].header.fixed_header.event_type.type_name =
          CORBA::string_dup(_description.eventName);
        notifyEvent_[i].header.fixed_header.event_name = CORBA::string_dup("");
        notifyEvent_[i].header.variable_header.length(0);   // put nothing here
        notifyEvent_[i].filterable_data.length(0);          // put nothing here
      }

      // offer declaration
      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent_[0].header.fixed_header.event_type;
      supplier_->addOffers(offers);
    }
  }


  void
  RangeSensorDispatcher::setData(RangeScanEventIDL * _data)
  {
    notifyEvent_[0].remainder_of_body <<= _data;
    eventPending_[0] = true;
  }

  void
  RangeSensorDispatcher::setData(RangeGroupEventIDL * _data)
  {
    unsigned int group = _data->group;
    notifyEvent_[group].remainder_of_body <<= _data;
    eventPending_[group] = true;
  }

  void
  RangeSensorDispatcher::setData(RangeBunchEventIDL * _data)
  {
    notifyEvent_[0].remainder_of_body <<= _data;
    eventPending_[0] = true;
  }

  int
  RangeSensorDispatcher::svc()
  {
    while (!canceled()) {
      Guard guard(mutex_);
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
  RangeSensorDispatcher::dispatch()
  {
    for (unsigned int i = 0; i < notifyEvent_.size(); ++i) {
      if (eventPending_[i]) {
        supplier_->sendEvent(notifyEvent_[i]);
        eventPending_[i] = false;
      }
    }
  }

  void
  RangeSensorDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }

  /** Maximum wait time for condition_.wait calls. */
  ACE_Time_Value RangeSensorImpl::maxWait_(0, 500000);

  /**
   * Implementation skeleton constructor.
   *
   * @param _description The description of the range sensor instance.
   * All sensor position, the sensors minimum and maximum measurable
   * distance as well as the scanning mode.
   *
   * @param _supplier A pointer to the StructuredPushSupplier to use
   * for event emittion. If a NULL pointer is passed, the RangeSensorImpl
   * will not any events.
   */
  RangeSensorImpl::RangeSensorImpl(const ScanDescriptionIDL& _description,
                                   StructuredPushSupplier * _supplier,
                                   bool _asynchDispatching) :
      supplier_(_supplier),
      description_(_description),
      masking_(false),
      mutex_(),
      condition_(mutex_),
      scan_(),
      asynchDispatching_(_asynchDispatching),
      dispatcherThread_(_description, _supplier)
  {
    MIRO_DBG_OSTR(MIRO, LL_CTOR_DTOR, "Constructing Miro::RangeSensorImpl. " << description_.eventName << std::endl);

    if (_description.group.length() == 0)
      throw Exception("RangeSensorImpl: Empty Scan Description");

    scan_.range.length(description_.group.length());
    for (unsigned int i = 0; i < scan_.range.length(); ++i) {
      scan_.range[i].length(description_.group[i].sensor.length());
      for (unsigned int j = 0; j < scan_.range[i].length(); ++j) {
        scan_.range[i][j] = Miro::RangeSensor::INVALID_RANGE;

        if (description_.group[i].sensor[j].masked)
          masking_ = true;
      }
    }
    if (asynchDispatching_)
      dispatcherThread_.detach(1);
  }

  RangeSensorImpl::~RangeSensorImpl()
  {
    MIRO_DBG_OSTR(MIRO, LL_CTOR_DTOR, "Destructing Miro::RangeSensorImpl." << std::endl);

    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  void
  RangeSensorImpl::integrateData(RangeScanEventIDL * _data)
  {
    if (masking_)
      for (int i = _data->range.length() - 1; i >= 0; --i)
        for (int j = _data->range[i].length() - 1; j >= 0; --j)
          if (description_.group[i].sensor[j].masked)
            _data->range[i][j] = Miro::RangeSensor::INVALID_RANGE;

    { // scope for guard
      Guard guard(mutex_);
      scan_ = *_data;
      condition_.broadcast();
    }

    if (supplier_) {
      if (asynchDispatching_) {
        Guard guard(dispatcherThread_.mutex_);
        dispatcherThread_.setData(_data);
        dispatcherThread_.cond_.broadcast();
      }
      else {
        dispatcherThread_.setData(_data);
        dispatcherThread_.dispatch();
      }
    }
    else {
      // as we own the data, we have to take care of it
      delete _data;
    }
  }

  void
  RangeSensorImpl::integrateData(RangeGroupEventIDL * _data)
  {
    if (masking_)
      for (int j = _data->range.length() - 1; j >= 0; --j)
        if (description_.group[_data->group].sensor[j].masked)
          _data->range[j] = Miro::RangeSensor::INVALID_RANGE;

    { // scope for guard
      Guard guard(mutex_);
      scan_.range[_data->group] = _data->range;
      scan_.time = _data->time;
      condition_.broadcast();
    }



    if (supplier_) {
      if (asynchDispatching_) {
        Guard guard(dispatcherThread_.mutex_);
        dispatcherThread_.setData(_data);
        dispatcherThread_.cond_.broadcast();
      }
      else {
        dispatcherThread_.setData(_data);
        dispatcherThread_.dispatch();
      }
    }
    else {
      // as we own the data, we have to take care of it
      delete _data;
    }
  }

  void
  RangeSensorImpl::integrateData(RangeBunchEventIDL * _data)
  {
    { // scope for guard
      Guard guard(mutex_);
      for (int i = _data->sensor.length() - 1; i >= 0; --i) {
        unsigned int group = _data->sensor[i].group;
        unsigned int index = _data->sensor[i].index;
        if (group < scan_.range.length() && index < scan_.range[group].length()) {
          if (masking_ && description_.group[group].sensor[index].masked)
            _data->sensor[i].range = Miro::RangeSensor::INVALID_RANGE;
          scan_.range[group][index] = _data->sensor[i].range;
        }
        else
          MIRO_LOG_OSTR(LL_WARNING, "RangeSensor: integrated data beyond buffer boundaries: "
                        << group << " " << index << std::endl);
      }
      condition_.broadcast();
    }

    if (supplier_) {
      if (asynchDispatching_) {
        Guard guard(dispatcherThread_.mutex_);
        dispatcherThread_.setData(_data);
        dispatcherThread_.cond_.broadcast();
      }
      else {
        dispatcherThread_.setData(_data);
        dispatcherThread_.dispatch();
      }
    }
    else {
      // as we own the data, we have to take care of it
      delete _data;
    }
  }

  void
  RangeSensorImpl::cancel()
  {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  ScanDescriptionIDL *
  RangeSensorImpl::getScanDescription() throw()
  {
    return new ScanDescriptionIDL(description_);
  }

  RangeGroupEventIDL *
  RangeSensorImpl::getGroup(CORBA::ULong id) throw(EOutOfBounds)
  {
    if (id >= description_.group.length())
      throw EOutOfBounds();

    Guard guard(mutex_);
    RangeGroupEventIDL_var event = new RangeGroupEventIDL();

    event->time = scan_.time;
    event->group = id;
    event->range = scan_.range[id];

    return event._retn();
  }

  RangeGroupEventIDL *
  RangeSensorImpl::getWaitGroup(CORBA::ULong id) throw(EOutOfBounds, ETimeOut)
  {
    if (id >= description_.group.length())
      throw EOutOfBounds();

    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;

    if (condition_.wait(&timeout) == -1)
      throw ETimeOut();

    RangeGroupEventIDL_var event = new RangeGroupEventIDL();

    event->time = scan_.time;
    event->group = id;
    event->range = scan_.range[id];

    return event._retn();
  }

  RangeScanEventIDL *
  RangeSensorImpl::getFullScan() throw()
  {
    Guard guard(mutex_);
    return new RangeScanEventIDL(scan_);
  }

  RangeScanEventIDL *
  RangeSensorImpl::getWaitFullScan() throw(Miro::ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait_;

    if (condition_.wait(&timeout) == -1)
      throw ETimeOut();

    return new RangeScanEventIDL(scan_);
  }
}
