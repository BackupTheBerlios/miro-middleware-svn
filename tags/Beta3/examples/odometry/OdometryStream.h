// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef OdometryStream_h
#define OdometryStream_h

#include "miro/StructuredPushConsumer.h"

#include <string>

class OdometryStream : public Miro::StructuredPushConsumer
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
  OdometryStream(CosNotifyChannelAdmin::EventChannel_ptr _ec,
		 const std::string& domainName);
  /** Disconnect from the supplier. */
  virtual ~OdometryStream();

  /** inherited IDL interface: StructuredPushSupplier methods */
  virtual void push_structured_event(const CosNotification::StructuredEvent & notification
				     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    throw(CORBA::SystemException, CosEventComm::Disconnected);
};
#endif
