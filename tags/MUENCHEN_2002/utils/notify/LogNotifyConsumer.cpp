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


#include "LogNotifyConsumer.h"

#include "miro/Server.h"
#include "miro/Exception.h"
#include "miro/TimeHelper.h"

#include <ace/Date_Time.h>

#include <map>

using std::cout;
using std::cerr;
using std::string;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

LogNotify::LogNotify(Miro::Server& _server,
		     EventChannel_ptr _ec,
		     const string& domainName,
		     const string& _fileName,
		     const LogNotifyParameters& _parameters,
		     bool _keepAlive) :
  Super(_ec),
  server_(_server),
  parameters_(_parameters),
  fileName_(_fileName),
  mutex_(),
  memMap_(fileName_.c_str(), parameters_.maxFileSize,
	  O_RDWR | O_CREAT | O_TRUNC, ACE_DEFAULT_FILE_PERMS, PROT_RDWR, ACE_MAP_SHARED),
  ostr_((char*)memMap_.addr(), memMap_.size()),
  total_length_(0),
  keepAlive_(_keepAlive)
{
  if (memMap_.addr() == MAP_FAILED)
    throw Miro::CException(errno, std::strerror(errno));

  EventTypeSeq added(parameters_.typeNames.size());
  EventTypeSeq removed(1);
  added.length(parameters_.typeNames.size());
  removed.length(1);

  for (unsigned int i = 0; i < parameters_.typeNames.size(); ++i) {
    added[i].domain_name =  CORBA::string_dup(domainName.c_str());
    added[i].type_name =
      CORBA::string_dup(parameters_.typeNames[i].c_str());
  }
  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumerAdmin_->subscription_change(added, removed);
}

LogNotify::~LogNotify()
{
  memMap_.close();

  if (ACE_OS::truncate(fileName_.c_str(), total_length_) == -1)
    throw Miro::CException(errno, std::strerror(errno));
}

void
LogNotify::push_structured_event(const StructuredEvent & notification
				 ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  // cout << "received event" << endl;

  Miro::TimeIDL time;
  Miro::timeA2C(ACE_OS::gettimeofday(), time);

  Miro::Guard guard(mutex_);

  if (connected_) {
    total_length_ = ostr_.total_length();
    
    ostr_ << time;
    ostr_ << notification;

    if (ostr_.total_length() > parameters_.maxFileSize) {
      cout << "disconnecting since max file size reached:" 
           << total_length_ << endl;
      connected_ = false;
      disconnect();
      if (!keepAlive_)
	server_.shutdown();
    }
  }
}

