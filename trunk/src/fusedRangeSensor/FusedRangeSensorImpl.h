// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef FusedRangeSensorImpl_h
#define FusedRangeSensorImpl_h

#include "idl/FusedRangeSensorS.h"
#include "miro/RangeSensorImpl.h"
#include "miro/Client.h"
#include "miro/StructuredPushConsumer.h"

#include <string>

namespace Miro 
{
  class FusedPushConsumer;

  class FusedRangeSensorImpl : public virtual POA_Miro::FusedRangeSensor,
			       public virtual Miro::RangeSensorImpl
  {
  public:
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
