// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BAP_tests_TransitionMonitor_h
#define BAP_tests_TransitionMonitor_h

#include "miro/StructuredPushConsumer.h"

#include <string>

class TransitionMonitor : public Miro::StructuredPushConsumer
{
  typedef Miro::StructuredPushConsumer Super;

public:
  /** 
   *Initialization
   *
   * Connect the Consumer to the EventChannel.
   * Creates a new proxy supplier and connects to it.
   *
   * Registers for the events, that it wants to get pushed.
   */
  TransitionMonitor(CosNotifyChannelAdmin::EventChannel_ptr _ec,
		    const std::string& domainName);
  /** Disconnect from the supplier. */
  virtual ~TransitionMonitor();

  /** inherited IDL interface: StructuredPushSupplier methods */
  virtual void push_structured_event(const CosNotification::StructuredEvent & notification
				     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    throw(CORBA::SystemException, CosEventComm::Disconnected);
};

#endif // BAP_tests_TransitionMonitor_h
