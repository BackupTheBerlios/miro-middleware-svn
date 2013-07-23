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
#ifndef LogNotifyConsumer_h
#define LogNotifyConsumer_h

#include "StructuredPushConsumer.h"
#include "miro/SvcParameters.h"

#include "LogWriter.h"

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
    std::string getFileName();

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

    //! The log device.
    LogWriter logWriter_;

    //! End the server if logfile is full.
    /** Default is false. */
    bool keepAlive_;

    ACE_Sample_History * history_;
    int nTimes_;
    ACE_hrtime_t testStart_;
  };
}
#endif
