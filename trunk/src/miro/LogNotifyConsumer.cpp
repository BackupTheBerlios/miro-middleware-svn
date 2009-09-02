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
#include "LogNotifyConsumer.h"

#include "Server.h"
#include "Exception.h"
#include "TimeHelper.h"
#include "Log.h"

#include <ace/Sample_History.h>
#include <ace/Version.h>
#if (ACE_MAJOR_VERSION > 5) || \
  ( (ACE_MAJOR_VERSION == 5) && (ACE_MINOR_VERSION > 5) ) || \
  ( (ACE_MAJOR_VERSION == 5) && (ACE_MINOR_VERSION == 5) && (ACE_BETA_VERSION >= 10) )
#  include <ace/Throughput_Stats.h>
#else
#  include <ace/Stats.h>
#endif

namespace Miro
{
  LogNotifyConsumer::LogNotifyConsumer(Server& _server,
                                       CosNotifyChannelAdmin::EventChannel_ptr _ec,
                                       std::string const& _domainName,
                                       std::string const& _fileName,
                                       LogNotifyParameters const& _parameters,
                                       bool _keepAlive) :
      Super(_ec),
      server_(_server),
      parameters_(_parameters),
      domainName_(_domainName),
      fileName_((_fileName.size() == 0) ? defaultFileName() : _fileName),
      mutex_(),
      logWriter_(fileName_, _parameters),
      keepAlive_(_keepAlive),
      history_(NULL),
      nTimes_(0)
  {
    CosNotification::EventTypeSeq added;
    added.length(parameters_.typeName.size() + parameters_.event.size());

    int index = 0;
    for (unsigned int i = 0; i < parameters_.typeName.size(); ++i, ++index) {
      added[index].domain_name =  CORBA::string_dup(domainName_.c_str());
      added[index].type_name =
        CORBA::string_dup(parameters_.typeName[i].c_str());
    }

    for (unsigned int i = 0; i < parameters_.event.size(); ++i, ++index) {
      added[index].domain_name =
        CORBA::string_dup(parameters_.event[i].domain.c_str());
      added[index].type_name =
        CORBA::string_dup(parameters_.event[i].type.c_str());
    }
    setSubscriptions(added);
  }

  LogNotifyConsumer::~LogNotifyConsumer()
  {
    delete history_;
  }

  void
  LogNotifyConsumer::push_structured_event(const CosNotification::StructuredEvent & notification
      ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
  {
    ACE_hrtime_t start = ACE_OS::gethrtime();

    Miro::Guard guard(mutex_);

    if (connected_) {
      if (!logWriter_.logEvent(ACE_OS::gettimeofday(), notification)) {
        MIRO_LOG(Log::LL_NOTICE,
                 "Disconnecting event log consumer since max file size reached.");
        connected_ = false;
        disconnect();
        if (!keepAlive_)
          server_.shutdown();
      }
    }

    // performance measurement
    if (nTimes_ > 0) {
      ACE_hrtime_t now = ACE_OS::gethrtime();
      history_->sample(now - start);

      --nTimes_;
    }
  }

  void
  LogNotifyConsumer::measureTiming(unsigned int _nTimes)
  {
    nTimes_ = _nTimes;
    delete history_;
    history_ = new ACE_Sample_History(nTimes_);
    testStart_ = ACE_OS::gethrtime();
  }


  void
  LogNotifyConsumer::evaluateTiming()
  {
    ACE_hrtime_t testEnd = ACE_OS::gethrtime();
    ACE_DEBUG((LM_DEBUG, "Logging client test finished\n"));

    ACE_DEBUG((LM_DEBUG, "High resolution timer calibration...."));
    ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor();
    ACE_DEBUG((LM_DEBUG, "done\n"));

    //history_->dump_samples ("HISTORY", gsf);

    ACE_Basic_Stats stats;
    history_->collect_basic_stats(stats);
    stats.dump_results("Total", gsf);

    ACE_Throughput_Stats::dump_throughput("Logging client total", gsf,
                                          testEnd - testStart_,
                                          stats.samples_count());
    delete history_;
    history_ = NULL;
  }

  std::string
  LogNotifyConsumer::defaultFileName() const
  {
    return path() + domainName_ + "_" + timeStringMinute() + ".log";
  }

  std::string
  LogNotifyConsumer::path()
  {
    char * miro_log = ACE_OS::getenv("MIRO_LOG");
    std::string p("./");

    if (miro_log)
      p = miro_log;

    if (p.size() > 0 && p[p.size() - 1] != '/')
      p += "/";
    return p;
  }
}
