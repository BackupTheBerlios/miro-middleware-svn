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
#ifndef PayloadConsumer_h
#define PayloadConsumer_h

#include "miro/StructuredPushConsumer.h"

class PayloadConsumer : public Miro::StructuredPushConsumer
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
  PayloadConsumer(CosNotifyChannelAdmin::EventChannel_ptr _ec,
		  unsigned int _id);
  /** Disconnect from the supplier. */
  virtual ~PayloadConsumer();

  /** inherited IDL interface: StructuredPushSupplier methods */
  virtual void push_structured_event(const CosNotification::StructuredEvent & notification)
    throw(CORBA::SystemException, CosEventComm::Disconnected);

  void results();

protected:
  unsigned int id_;
  unsigned int received_;
};
#endif
