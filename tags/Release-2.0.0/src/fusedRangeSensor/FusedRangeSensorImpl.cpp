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
#include "FusedRangeSensorImpl.h"

#include "miro/TimeHelper.h"
#include "miro/Angle.h"
#include "miro/Log.h"

#include <list>
#include <cmath>

using std::endl;
using std::string;
using std::list;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

namespace Miro {

  FusedRangeSensorImpl::FusedRangeSensorImpl(Miro::Client* _client, 
					     CosNotifyChannelAdmin::EventChannel_ptr _ec,
					     Miro::ScanDescriptionIDL& _description, 
					     Miro::StructuredPushSupplier * _supplier) :
    RangeSensorImpl(_description,_supplier),
    description_(_description),
    client_(_client),
    pConsumer_(new FusedPushConsumer(this, _ec)),
    interpolationMode_(Miro::FusedRangeSensor::MASKED),
    fusionMode_(Miro::FusedRangeSensor::MINIMUM)
  {
    MIRO_LOG_CTOR("FusedRangeSensorImpl");
    maxWait_=ACE_Time_Value(5); // five seconds
    subscriptions_.length(0);
  }

  FusedRangeSensorImpl::~FusedRangeSensorImpl()
  {
    delete pConsumer_;
  }

  void FusedRangeSensorImpl::addSensor(const char* name) 
    throw (Miro::EDevIO)
  {
    try {
      RangeSensor_var rangeSensor=client_->resolveName<Miro::RangeSensor>(name);
      Miro::Guard guard(mutex_);
      
      addSubscription(name);
      delete descriptionList_[name];
      descriptionList_[name]=rangeSensor->getScanDescription();
      calculateDescription();
      pConsumer_->setSubscriptions(subscriptions_);
    }
    catch(CORBA::Exception & e) {
      throw Miro::EDevIO(std::string(std::string("Could not get sensor ")+name).c_str());
    }
  }

  void FusedRangeSensorImpl::removeSensor(const char* name) 
    throw (Miro::EDevIO)
  {
    {
      Miro::Guard guard(mutex_);
      removeSubscription(name);
      pConsumer_->setSubscriptions(subscriptions_);
      descriptionList_.erase(name);
      scanList_.erase(name);
      calculateDescription();
    }
  }

  void FusedRangeSensorImpl::setFusionMode(Miro::FusedRangeSensor::FusionType mode) 
    throw (CORBA::SystemException)
  {
    MIRO_LOG_OSTR(LL_DEBUG,"Changed fusion mode to " << mode);
    fusionMode_=mode;
    calculateDescription(); //Re-Initialize scan when changing modes
  }

  void FusedRangeSensorImpl::setInterpolationMode(Miro::FusedRangeSensor::InterpolationType mode) 
    throw (CORBA::SystemException)
  {
    MIRO_LOG_OSTR(LL_DEBUG,"Changed interpolation mode to " << mode);
    interpolationMode_=mode;
    calculateDescription(); //Re-Initialize scan when changing modes
  }

  void FusedRangeSensorImpl::addSubscription(std::string name) {
    unsigned int length=subscriptions_.length();
    unsigned int i=0;    

    for (i=0; i<length; i++) {
      if (string("") == string(subscriptions_[i].type_name)) {
	break;
      }
    }
    if (i==length) {
      subscriptions_.length(length+1);
    }
    subscriptions_[i].domain_name = CORBA::string_dup(client_->namingContextName.c_str());
    subscriptions_[i].type_name = CORBA::string_dup(name.c_str());

  }

  void FusedRangeSensorImpl::removeSubscription(std::string name) {
    unsigned int length=subscriptions_.length();
    for (unsigned int i=0; i<length; i++) {
      if (name == string(subscriptions_[i].type_name)) {
	subscriptions_[i].type_name = CORBA::string_dup("");
      }
    }
    while ((length>0) && (string("") == string(subscriptions_[length-1].type_name))) {
      length--;
    }
    subscriptions_.length(length);
  }

  void FusedRangeSensorImpl::calculateDescription() {
    //TODO: Instead of getting all the new descriptions each time,
    // use the descriptionList_.
    // It might be useful to update it...

    RangeSensor_var rangeSensor;

    int fusedSensors=0;

    int maxResolution=360; // sensor values for 360 deg.
    int minRange=99999;
    int maxRange=0;

    int height=0;

    for (unsigned int i=0; i<subscriptions_.length(); i++) {
      if (string(subscriptions_[i].type_name)=="") 
	continue;
      MIRO_LOG_OSTR(LL_DEBUG, "Get description for " << subscriptions_[i].type_name);
      fusedSensors++;
      try {
	rangeSensor=client_->resolveName<Miro::RangeSensor>(subscriptions_[i].type_name);
	
	Miro::ScanDescriptionIDL_var desc = rangeSensor->getScanDescription();
	int resolution = desc->group[0].sensor.length();
	double minAngle=2*M_PI;
	double maxAngle=0;
	double alpha=0;
	double beta=0;
	for (int j=0; j<resolution; j++) {
	  alpha=desc->group[0].sensor[j].alpha;
	  beta=desc->group[0].sensor[j].beta;
	  if (minAngle > alpha + beta )
	    minAngle=alpha + beta;
	  if (maxAngle < alpha + beta)
	    maxAngle=alpha + beta;
	}
	resolution = int((resolution-1) * ((2*M_PI) / (maxAngle - minAngle)));
	if (resolution > maxResolution) 
	  maxResolution = resolution;
	if (minRange > desc->group[0].description.minRange)
	  minRange = desc->group[0].description.minRange;
	if (maxRange < desc->group[0].description.maxRange)
	  maxRange = desc->group[0].description.maxRange;
	height+=desc->group[0].sensor[0].height;
      }
      catch (CORBA::Exception & e) {
      }
    }
    //if fusing no sensors, return no values
    if (fusedSensors==0)
      maxResolution=0;

    description_.group[0].sensor.length(maxResolution);
    description_.group[0].description.minRange=minRange;
    description_.group[0].description.maxRange=maxRange;
    description_.group[0].description.focus=(2*M_PI)/double(maxResolution);

    for (int i=0; i<maxResolution; i++) {
      description_.group[0].sensor[i].height=height/fusedSensors;
      description_.group[0].sensor[i].distance=0;
      description_.group[0].sensor[i].alpha=i*description_.group[0].description.focus - M_PI;
      description_.group[0].sensor[i].beta=0;
      description_.group[0].sensor[i].gamma=0;
      description_.group[0].sensor[i].masked=true;
    }
    initializeScan(&scan_);
  }

  void FusedRangeSensorImpl::initializeScan(RangeScanEventIDL * scan)
  {
    scan->range.length(description_.group.length());
    for (unsigned int i = 0; i < scan->range.length(); ++i) {
      scan->range[i].length(description_.group[i].sensor.length());
      for (unsigned int j = 0; j < scan->range[i].length(); ++j) {
	scan->range[i][j] = Miro::RangeSensor::INVALID_RANGE;
      }
    }
    ACE_Time_Value timeTmp;
    timeA2C(timeTmp, scan->time);
  }

  void FusedRangeSensorImpl::processStructuredEvent(const CosNotification::StructuredEvent & notification) {
    Miro::RangeBunchEventIDL * pSensorBunch=NULL;
    Miro::RangeScanEventIDL * fusion=NULL;
    //    ACE_Time_Value currentTime;
    ACE_Time_Value scanTime;
    //    ACE_Time_Value timeTmp;
    

    MIRO_LOG(LL_PRATTLE,"Process Structured Event");
   // timeTmp.usec(1000);
    
    //  timeC2A(scan_.time,currentTime);

    string eventName=CORBA::string_dup(notification.header.fixed_header.event_type.type_name);
    
    if (notification.remainder_of_body >>= pSensorBunch) {
      MIRO_LOG_OSTR(LL_PRATTLE, "Event Received (" 
		    << eventName
		    << "); length: " 
		    << pSensorBunch->sensor.length()); 
       timeC2A(pSensorBunch->time,scanTime);
       
       //do not delete; integrateData takes ownership
       fusion=new RangeScanEventIDL();
       

       initializeScan(fusion);
       timeA2C(scanTime, fusion->time);

      delete scanList_[eventName];
      scanList_[eventName]=new RangeBunchEventIDL(*pSensorBunch);

      switch (fusionMode_) {
      case Miro::FusedRangeSensor::MINIMUM:
	for (RangeBunchList::iterator it=scanList_.begin(); it!=scanList_.end(); it++) {
	  MIRO_LOG_OSTR(LL_PRATTLE,"Min-Fusing " << it->first);
	  fuseMinimum(fusion, description_, it->second, *descriptionList_[it->first]);
	}
	break;
      case Miro::FusedRangeSensor::AVERAGE: {
	int i=0;
	for (RangeBunchList::iterator it=scanList_.begin(); it!=scanList_.end(); it++) {
	  i++;
 	  MIRO_LOG_OSTR(LL_PRATTLE,"Avg-Fusing " << it->first);
 	  fuseAverage(fusion, description_, i, it->second, *descriptionList_[it->first]);
 	}
 	break;
      }
      default:
	MIRO_LOG_OSTR(LL_ERROR, "FusedRangeSensorImpl: Unknown fusion mode (" << fusionMode_ << ")");
	break;
      }

      switch (interpolationMode_) {
      case Miro::FusedRangeSensor::MASKED:
	interpolateMasked(fusion, description_);
	break;
      case Miro::FusedRangeSensor::LINEAR:
	interpolateLinear(fusion, description_);
	break;
      default:
	MIRO_LOG_OSTR(LL_ERROR, "FusedRangeSensorImpl: Unknown interpolation mode (" << interpolationMode_ << ")");
	break;
      }
      
      integrateData(fusion);
    } else {
      MIRO_LOG_OSTR(LL_NOTICE, "Event received: " << eventName);
    }
  }





  FusedPushConsumer::FusedPushConsumer(FusedRangeSensorImpl * _impl,CosNotifyChannelAdmin::EventChannel_ptr _ec) :
    StructuredPushConsumer(_ec),
    impl_(_impl)
  {
    MIRO_LOG_CTOR("FusedPushConsumer");
  }

  FusedPushConsumer::~FusedPushConsumer()
  {
  }

  void FusedPushConsumer::push_structured_event(const StructuredEvent & notification 
						   ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosEventComm::Disconnected)
  {
    impl_->processStructuredEvent(notification);
  }

}

namespace Miro {
  int getIndex(const ScanDescriptionIDL& description, double angle) {
    int index;
    double alpha=0;

    //Assume all beta's are 0 on the description.

    for (index=0; index<(long int)description.group[0].sensor.length(); index++) {
      alpha=description.group[0].sensor[index].alpha;
      if (alpha >= angle)
	break;
    }
    if ((index <(long int)description.group[0].sensor.length()) &&  
	(fabs(alpha - angle) < deg2Rad(1))) { 
      //1 deg. tolerance.
      // should not be a problem for greater resolutions, as the for will break
      // at the closest bigger or equal value.
      // And 1 degree precission loss is not such a big deal anyway (2 mm at 8 m!).
      return index;
    }
    //otherwise, not found; return error;
    return -1;
    
  }

  void fuseAverage(RangeScanEventIDL * scan,
		   ScanDescriptionIDL & scanDescription,
		   int alreadyFused,
		   const RangeBunchEventIDL * newScan,
		   const ScanDescriptionIDL & newScanDescription) {
  
    double angle;
    double distance;
    int distanceFromCenter;
    int index;
    double alpha;
    double beta;

    //By definition, newScan should contain same or fewer readings than scan
    //otherwise, there is an error while adding/removing subscriptions
    for (unsigned int i=0; i<newScan->sensor.length(); i++) {
      //TODO: It could be more than one group present.
      if (newScan->sensor[i].group >0)
	continue;
      //Ignore masked/invalid readings
      if (newScan->sensor[i].range<0)
	continue;
      alpha=newScanDescription.group[newScan->sensor[i].group].sensor[newScan->sensor[i].index].alpha;
      beta=newScanDescription.group[newScan->sensor[i].group].sensor[newScan->sensor[i].index].beta;
      distanceFromCenter=newScanDescription.group[newScan->sensor[i].group].sensor[newScan->sensor[i].index].distance;

      if (beta != 0) {
	int a=newScan->sensor[i].range;
	int b=distanceFromCenter;
	double C=M_PI+beta; 
	//calculate distance using the cosinus rule
	// c² = a²+b²-2 a b cos(C)
	distance=sqrt(a*a+b*b-2*a*b*cos(C));
	//Calculate angle using the sinus rule
	//sin(A)/a = sin(B)/b = sin(C)/c
	angle=alpha - asin(a*sin(C)/distance);
      } else {
	angle = alpha;
	distance=newScan->sensor[i].range+distanceFromCenter;
      }

      index=getIndex(scanDescription, angle);

      if (index==-1) {
	MIRO_LOG_OSTR(LL_WARNING, "No sensor found for angle " << angle);
      } else if (scan->range[0][index]>=0) {
	scan->range[0][index]=(scan->range[0][index]*alreadyFused + int(distance))/(alreadyFused+1);
      } else { //not yet initialized.
	scan->range[0][index]=int(distance);
	scanDescription.group[0].sensor[index].masked=false;
      }
    }
  }		   

  void fuseMinimum(RangeScanEventIDL  * scan, 
		   ScanDescriptionIDL & scanDescription, 
		   const RangeBunchEventIDL * newScan,
		   const ScanDescriptionIDL & newScanDescription) {
    double angle;
    double distance;
    int distanceFromCenter;
    int index;
    double alpha;
    double beta;

    //By definition, newScan should contain same or fewer readings than scan
    //otherwise, there is an error while adding/removing subscriptions
    for (unsigned int i=0; i<newScan->sensor.length(); i++) {
      //TODO: It could be more than one group present.
      if (newScan->sensor[i].group >0)
	continue;
      //Ignore masked/invalid readings
      if (newScan->sensor[i].range<0)
	continue;
      alpha=newScanDescription.group[newScan->sensor[i].group].sensor[newScan->sensor[i].index].alpha;
      beta=newScanDescription.group[newScan->sensor[i].group].sensor[newScan->sensor[i].index].beta;
      distanceFromCenter=newScanDescription.group[newScan->sensor[i].group].sensor[newScan->sensor[i].index].distance;

      if (beta != 0) {
	int a=newScan->sensor[i].range;
	int b=distanceFromCenter;
	double C=M_PI+beta; 
	//calculate distance using the cosinus rule
	// c² = a²+b²-2 a b cos(C)
	distance=sqrt(a*a+b*b-2*a*b*cos(C));
	//Calculate angle using the sinus rule
	//sin(A)/a = sin(B)/b = sin(C)/c
	angle=alpha - asin(a*sin(C)/distance);
      } else {
	angle = alpha;
	distance=newScan->sensor[i].range+distanceFromCenter;
      }

      index=getIndex(scanDescription, angle);

      if (index==-1) {
	MIRO_LOG_OSTR(LL_WARNING, "No sensor found for angle " << angle);
      } else if (scan->range[0][index]>=0) {
	scan->range[0][index]=std::min(scan->range[0][index],int(distance));
      } else { //not yet initialized.
	scan->range[0][index]=int(distance);
	scanDescription.group[0].sensor[index].masked=false;
      }
    }
  }

  void interpolateMasked(RangeScanEventIDL * /*scan*/,
			 const ScanDescriptionIDL & /*scanDescription*/) {
    /* The default initialization puts a "MASKED" value on every sensor. */
    /* Thus this function does nothing */
    /* Provided only for consistence */
  }

  void interpolateLinear(RangeScanEventIDL * scan,
			 const ScanDescriptionIDL & /*scanDescription*/) {

    int lastKnown=0;
    int nextKnown=0;
    int currentValue=Miro::RangeSensor::INVALID_RANGE;
    int maskedValues=0;
    int currentMasked=0;

    unsigned int i=0;

    if (scan->range[0][0]<0) {
      maskedValues++;
      //if the first value is masked, find the last value.
      for (i=scan->range[0].length()-1; i>0 ; i--) {
	currentValue=scan->range[0][i];
	if (currentValue<0) {
	  maskedValues++;
	  currentMasked++;
	} else {
	  lastKnown=currentValue;
	  for (unsigned int j=1; j<scan->range[0].length(); j++) {
	    if (scan->range[0][j]<0) {
	      maskedValues++;
	    } else {
	      nextKnown=scan->range[0][j];
	      break;
	    }
	  }
	  if (nextKnown<=0) {
	    //Everything masked or error => return;
	    return;
	  }
	}
      }
    }

    for ( i=0; i<scan->range[0].length(); i++) {
      currentValue=scan->range[0][i];
      if (currentValue>=0) {
	lastKnown=currentValue;
	currentMasked=0;
	maskedValues=0;
      } else {
	if (maskedValues==0) {
	  unsigned int j;
	  for (j=i; j<scan->range[0].length() && scan->range[0][j]<0 ; j++) {
	    maskedValues++;
	  }
	  if (j<scan->range[0].length()) {
	    nextKnown=scan->range[0][j];
	  } else {
	    for (j=0; j<i && scan->range[0][j]<0 ; j++) {
	      maskedValues++;
	    }
	    if (j<i) {
	      nextKnown=scan->range[0][j];
	    } else {
	      //no unmasked either before or after?!?!
	      // everything masked; cannot interpolate
	      return;
	    }
	  }
	}
	currentMasked++;
	scan->range[0][i]=lastKnown+((nextKnown-lastKnown)*(currentMasked))/(maskedValues+1);
      }
    }
    
  }
}
