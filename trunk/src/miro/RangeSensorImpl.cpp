//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "RangeSensorImpl.h"
#include "Exception.h"
#include "ExceptionC.h"
#include "StructuredPushSupplier.h"

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Miro
{
  ACE_Time_Value RangeSensorImpl::maxWait(0, 500000);

  RangeSensorImpl::RangeSensorImpl(const ScanDescriptionIDL& _description,
				   StructuredPushSupplier * _supplier) :
    supplier_(_supplier),
    description_(_description),
    masking_(false),
    mutex_(),
    condition_(mutex_),
    scan_()
  {
    DBG(cout << "Constructing Miro::RangeSensorImpl." << endl);

    if (_description.group.length() == 0) 
      throw Exception("Empty Scan Description");

    scan_.range.length(description_.group.length());
    for (unsigned int i = 0; i < scan_.range.length(); ++i) {
      scan_.range[i].length(description_.group[i].sensor.length());
      for (unsigned int j = 0; j < scan_.range[i].length(); ++j) {
	scan_.range[i][j] = Miro::RangeSensor::INVALID_RANGE;

	if (description_.group[i].sensor[j].masked)
	  masking_ = true;
      }
    }

    if (supplier_) {
      // Notify Event initialization
      notifyEvent.header.fixed_header.event_type.domain_name =
	CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent.header.fixed_header.event_type.type_name = 
	CORBA::string_dup(_description.eventName);
      notifyEvent.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent.header.variable_header.length(0);   // put nothing here
      notifyEvent.filterable_data.length(0);          // put nothing here
    }
  }

  RangeSensorImpl::~RangeSensorImpl()
  {
    DBG(cout << "Destructing RangeSensorImpl." << endl);

    // unlock waiting threads
    condition_.broadcast();
  }

  void
  RangeSensorImpl::integrateData(RangeScanEventIDL * _data)
  {
    if (masking_)
      for (int i = _data->range.length() - 1; i >= 0; --i)
	for (int j = _data->range[i].length() - 1; j >= 0; --j)
	  if (description_.group[i].sensor[j].masked)
	    _data->range[i][j] = Miro::RangeSensor::INVALID_RANGE;

    Guard guard(mutex_);
    scan_ = *_data;

    if (supplier_) {
      notifyEvent.remainder_of_body <<= _data;
      supplier_->sendEvent(notifyEvent);
    }
    condition_.broadcast();
  }

  void
  RangeSensorImpl::integrateData(RangeGroupEventIDL * _data)
  {
    if (masking_)
      for (int j = _data->range.length() - 1; j >= 0; --j)
	if (description_.group[_data->group].sensor[j].masked)
	  _data->range[j] = Miro::RangeSensor::INVALID_RANGE;

    Guard guard(mutex_);
    scan_.range[_data->group] = _data->range;

    if (supplier_) {
      notifyEvent.remainder_of_body <<= _data;
      supplier_->sendEvent(notifyEvent);
    }
    condition_.broadcast();
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
	  cout << "RangeSensor: integrated data beyond buffer boundaries: " 
	       << group << " " << index << endl;
      }
      condition_.broadcast();
    }
    if (supplier_) {
      notifyEvent.remainder_of_body <<= _data;
      supplier_->sendEvent(notifyEvent);
    }
  }

  ScanDescriptionIDL *
  RangeSensorImpl::getScanDescription() throw()
  {
    return new ScanDescriptionIDL(description_);
  }

  RangeGroupEventIDL *
  RangeSensorImpl::getGroup(CORBA::Long id) throw()
  {
    Guard guard(mutex_);
    RangeGroupEventIDL_var event = new RangeGroupEventIDL();
    
    event->time = scan_.time;
    event->group = id;
    event->range = scan_.range[id];

    return event._retn();
  }

  RangeGroupEventIDL *
  RangeSensorImpl::getWaitGroup(CORBA::Long id) throw(Miro::ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait;

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
    timeout += maxWait;

    if (condition_.wait(&timeout) == -1)
      throw ETimeOut();

    return new RangeScanEventIDL(scan_);
  }
};
