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

#include "LogNotifyConsumer.h"

#include "Server.h"
#include "Exception.h"
#include "TimeHelper.h"
#include "Log.h"

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
    fileName_((_fileName.size() == 0)? defaultFileName() : _fileName),
    mutex_(),
    memMap_(fileName_.c_str(), parameters_.maxFileSize,
	    O_RDWR | O_CREAT | O_TRUNC, ACE_DEFAULT_FILE_PERMS, PROT_RDWR, 
	    ACE_MAP_SHARED),
    ostr_((char*)memMap_.addr(), memMap_.size()),
    totalLength_(0),
    keepAlive_(_keepAlive)
  {
    if (memMap_.addr() == MAP_FAILED)
      throw CException(errno, std::strerror(errno));

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
    // close the memory mapped file
    memMap_.close();

    // truncate the file to the actual length
    if (ACE_OS::truncate(fileName_.c_str(), totalLength_) == -1)
      throw CException(errno, std::strerror(errno));
  }

  void
  LogNotifyConsumer::push_structured_event(const CosNotification::StructuredEvent & notification
					   ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosEventComm::Disconnected)
  {
    TimeIDL time;
    timeA2C(ACE_OS::gettimeofday(), time);

    Guard guard(mutex_);

    if (connected_) {
      totalLength_ = ostr_.total_length();
    
      ostr_ << time;
      ostr_ << notification;

      if (ostr_.total_length() > parameters_.maxFileSize) {
	MIRO_LOG_OSTR(Log::LL_NOTICE, 
		      "Disconnecting event log consumer since max file size reached:" << 
		      totalLength_ );
	connected_ = false;
	disconnect();
	if (!keepAlive_)
	  server_.shutdown();
      }
    }
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
