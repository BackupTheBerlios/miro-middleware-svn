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
#ifndef LogNotifyConsumer_h
#define LogNotifyConsumer_h

#include <ace/Mem_Map.h>
#include <tao/CDR.h>
#include "LogNotifyParameters.h"

#include "miro/StructuredPushConsumer.h"

#include <fstream>
#include <string>

// forward declarations
namespace Miro
{
  class Server;
};

class LogNotify : public Miro::StructuredPushConsumer
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
  LogNotify(Miro::Server& _server,
	    CosNotifyChannelAdmin::EventChannel_ptr _ec,
	    const std::string& domainName,
	    const std::string& _fileName,
	    const LogNotifyParameters& _parameters,
	    bool _keepAlive = false);
  /** Disconnect from the supplier. */
  virtual ~LogNotify();

  /** inherited IDL interface: StructuredPushSupplier methods */
  virtual void push_structured_event(const CosNotification::StructuredEvent & notification
				     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    throw(CORBA::SystemException, CosEventComm::Disconnected);


protected:
  Miro::Server& server_;
  const LogNotifyParameters& parameters_;
  std::string fileName_;

  Miro::Mutex mutex_;
  ACE_Mem_Map memMap_;
  TAO_OutputCDR ostr_;
  size_t total_length_;

  bool keepAlive_;
};
#endif
