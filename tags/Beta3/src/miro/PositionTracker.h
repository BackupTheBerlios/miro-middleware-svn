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
#ifndef PositionTracker_h
#define PositionTracker_h

#include "Position.h"
#include "StructuredPushConsumer.h"

#include <string>

namespace Miro
{
  class PositionTracker : public StructuredPushConsumer
  {
    typedef StructuredPushConsumer Super;

  public:
    /** 
     *Initialization
     *
     * Connect the Consumer to the EventChannel.
     * Creates a new proxy supplier and connects to it.
     *
     * Registers for the events, that it wants to get pushed.
     */
    PositionTracker(CosNotifyChannelAdmin::ConsumerAdmin_ptr _consumerAdmin,
		    const std::string& domainName,
		    unsigned int historySize);
    /** Disconnect from the supplier. */
    virtual ~PositionTracker();
    
    /** inherited IDL interface: StructuredPushSupplier methods */
    virtual void push_structured_event(const CosNotification::StructuredEvent & notification,
				       CORBA::Environment &ACE_TRY_ENV = TAO_default_environment())
      throw(CORBA::SystemException, CosEventComm::Disconnected);

    /**
     * Get the last available postion.
     *
     * Local interface for the asynchronous clients.
     */
    Position getLastPosition() const;
    /**
     * Get the estimated position at the specified time.
     *
     * This value is inter- and extrapolated as needed.
     * The precision is dependend on the available amount
     * of scans.
     *
     * Local interface for the asynchronous clients
     */
    Position getPosition(const ACE_Time_Value& time) const;

  protected:
  };
};
#endif
