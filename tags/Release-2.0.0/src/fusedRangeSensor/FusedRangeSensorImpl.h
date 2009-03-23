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
#ifndef FusedRangeSensorImpl_h
#define FusedRangeSensorImpl_h

#include "idl/FusedRangeSensorS.h"
#include "miro/RangeSensorImpl.h"
#include "miro/Client.h"
#include "miro/StructuredPushConsumer.h"

#include <string>
#include <map>

namespace Miro 
{
  class FusedPushConsumer;

  class FusedRangeSensorImpl : public virtual POA_Miro::FusedRangeSensor,
			       public virtual Miro::RangeSensorImpl
  {
  public:

    typedef std::map<std::string, Miro::RangeBunchEventIDL *> RangeBunchList;
    typedef std::map<std::string, Miro::ScanDescriptionIDL *> ScanDescriptionList;

    //! Constructor
    FusedRangeSensorImpl(Miro::Client* _client,
			 CosNotifyChannelAdmin::EventChannel_ptr _ec,
			 Miro::ScanDescriptionIDL& _description,
			 Miro::StructuredPushSupplier * _supplier = NULL);

    //! Destructor
    virtual ~FusedRangeSensorImpl();

    virtual void addSensor(const char* name) throw (Miro::EDevIO);
    virtual void removeSensor(const char* name) throw (Miro::EDevIO);
    virtual void setInterpolationMode(Miro::FusedRangeSensor::InterpolationType mode) throw (CORBA::SystemException);
    virtual void setFusionMode(Miro::FusedRangeSensor::FusionType mode) throw (CORBA::SystemException);

  protected:
    ScanDescriptionIDL& description_;

    Miro::Client *client_;

    FusedPushConsumer * pConsumer_;
    CosNotification::EventTypeSeq subscriptions_;

    RangeBunchList scanList_;
    ScanDescriptionList descriptionList_;

    Miro::FusedRangeSensor::InterpolationType interpolationMode_;
    Miro::FusedRangeSensor::FusionType fusionMode_;

    /**
     * Add name to the subscription list.
     * Does not check for duplicates
     * Does not actually subscribe
     * Use empty ("") placeholders before actually expanding the list
     */
    void addSubscription(std::string name);
    /**
     * Remove name from the subscription list.
     * Removes all instances of a sensor
     * Does not actually unsubscribe
     * Remove from list if at EOL
     *  else set type_name to empty ("")
     */
    void removeSubscription(std::string name);

    /**
     * Calculate the new sensor description from the subscribed events.
     * Connects to the corresponding servers to get their description
     * Does not check for duplicates
     */
    void calculateDescription();

    /**
     * Set array lengths on @scan according to current scan description.
     * Set all values to INVALID_RANGE
     */
    void initializeScan(RangeScanEventIDL * scan);

    /**
     * Process an event.
     * Chose appropiate fusion and interpolation method
     */
    void processStructuredEvent(const CosNotification::StructuredEvent & notification);

    friend class FusedPushConsumer;
  };

  class FusedPushConsumer : public virtual Miro::StructuredPushConsumer
  {

  public:
    FusedPushConsumer(FusedRangeSensorImpl * _impl, CosNotifyChannelAdmin::EventChannel_ptr _ec);
    virtual ~FusedPushConsumer();

    virtual void push_structured_event( const CosNotification::StructuredEvent & notification
				       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosEventComm::Disconnected);

    FusedRangeSensorImpl * impl_;

    friend class FusedRangeSensorImpl;
  };

  void fuseAverage(Miro::RangeScanEventIDL  * scan, 
		   Miro::ScanDescriptionIDL & scanDescription, 
		   int alreadyFused,
		   const Miro::RangeBunchEventIDL * newScan,
		   const Miro::ScanDescriptionIDL & newScanDescription);
  void fuseMinimum(Miro::RangeScanEventIDL  * scan, 
		   Miro::ScanDescriptionIDL & scanDescription, 
		   const Miro::RangeBunchEventIDL * newScan,
		   const Miro::ScanDescriptionIDL & newScanDescription);

  void interpolateMasked(RangeScanEventIDL * scan,
			 const ScanDescriptionIDL & scanDescription);
  void interpolateLinear(RangeScanEventIDL * scan,
			 const ScanDescriptionIDL & scanDescription);

}

#endif
