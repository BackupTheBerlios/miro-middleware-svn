// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef LogNotifyConsumer_h
#define LogNotifyConsumer_h

#include "StructuredPushConsumer.h"
#include "SvcParameters.h"

#include <ace/Mem_Map.h>
#include <ace/High_Res_Timer.h>
#include <tao/CDR.h>

#include <string>

// forward declarations
class ACE_Sample_History;

namespace Miro
{
  // forward declarations
  class Server;

  class LogNotifyConsumer : public Miro::StructuredPushConsumer
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
    LogNotifyConsumer(Server& _server,
		      CosNotifyChannelAdmin::EventChannel_ptr _ec,
		      std::string const& _domainName,
		      std::string const& _fileName = std::string(),
		      LogNotifyParameters const& _parameters = *LogNotifyParameters::instance(),
		      bool _keepAlive = true);

    //! Disconnect from the supplier.
    virtual ~LogNotifyConsumer();

    //! Inherited IDL interface: StructuredPushSupplier method
    virtual void push_structured_event(const CosNotification::StructuredEvent & notification
				       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosEventComm::Disconnected);

    //! Default method to generate a file name for a log file.
    /**
     * The default fully qualified file path + name looks the following:
     *
     * <$MIRO_LOG>/<DomainName>_<timeString()>.log
     *
     * If MIRO_LOG is not set, the current directory will be used for the log file.
     * DomainName is per convention the name of the robot.
     */
    std::string defaultFileName() const;

    void measureTiming(unsigned int _nTimes);
    void evaluateTiming();

  protected:
    //! The default location for log files.
    /** 
     * The default location is defined by the environment variable MIRO_LOG.
     * If this variable is not defined, the current directory is used.
     */
    static std::string path();

    //! Reference to the server.
    /** Used only for shutdown signalling (see @keepAlive_). */
    Server& server_;
    //! Reference to the parameters.
    LogNotifyParameters const& parameters_;
    //! The default domain name of the events.
    /** Per convention thi is the name of the robot. */
    std::string domainName_;
    //! The name of the log file.
    std::string fileName_;

    Miro::Mutex mutex_;
    //! Memory mapped file, holding the log.
    ACE_Mem_Map memMap_;
    //! CDR stream to log to.
    TAO_OutputCDR ostr_;
    size_t totalLength_;

    //! End the server if logfile is full.
    /** Default is false. */
    bool keepAlive_;

    ACE_Sample_History * history_;
    int nTimes_;
    ACE_hrtime_t testStart_;
  };
}
#endif
