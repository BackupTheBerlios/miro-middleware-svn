// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "LogFile.h"
#include "ChannelManager.h"
#include "EventView.h"

#include "miro/StructuredPushSupplier.h"
#include "idl/TimeC.h"
#include "miro/TimeHelper.h"

#include <cstring>

LogFile::LogFile(QString const& _name,
		 ChannelManager * _channelManager) :
  name_(_name),
  channelManager_(_channelManager),
  coursor_(timeVector_.end()),
  supplier_(NULL),
  istr_(NULL),
  parsed_(false)
{
  memoryMap_.map(_name, -1, O_RDONLY);
  if (0 == memoryMap_.handle())
    throw Miro::CException(errno, std::strerror(errno));    

  istr_= new TAO_InputCDR((char*)memoryMap_.addr(), memoryMap_.size());    
}

LogFile::~LogFile()
{
  if (supplier_)
    supplier_->disconnect();
  delete supplier_;

  delete istr_;

  memoryMap_.close();
}

unsigned int 
LogFile::parse()
{
  unsigned int rc = 0;
  Miro::TimeIDL timeIDL;
  ACE_Time_Value timeStamp;
  CosNotification::StructuredEvent event;

  while (istr_->length() != 0) {

    (*istr_) >> timeIDL;
    Miro::timeC2A(timeIDL, timeStamp);
    if (timeStamp == ACE_Time_Value(0)) {
      break;
    }
    timeVector_.push_back( TimePair( timeStamp, (char*)istr_->rd_ptr() ) );
    
    // skip event
    (*istr_) >> event;

    typeNames_.insert(QString(event.header.fixed_header.event_type.type_name));

    if (!(timeVector_.size() % 2048))
      break;
  }

  if (timeVector_.size() == 0)
    throw Miro::Exception("Logfile contains no data.");

  // EOF
  if (istr_->length() == 0 || timeStamp == ACE_Time_Value(0)) {
    coursor_ = timeVector_.begin();
    
    domainName_ = event.header.fixed_header.event_type.domain_name;
    ec_ = channelManager_->getEventChannel(domainName_);
    supplier_ = new Miro::StructuredPushSupplier(ec_.in(), domainName_.latin1());

    CosNotification::EventTypeSeq offers;
    offers.length(typeNames_.size());

    CStringSet::const_iterator typeName = typeNames_.begin();
    for (unsigned int i = 0; i < offers.length(); ++i, ++ typeName) {
      offers[i].domain_name = CORBA::string_dup(domainName_.latin1());
      offers[i].type_name = CORBA::string_dup(typeName->latin1());
    }
    supplier_->setOffers(offers);

    delete istr_;
    istr_ = NULL;
    rc = 100;
    parsed_ = true;
  }
  // End Of Work Packet
  else 
    rc = (unsigned int)((double) ((char *) istr_->rd_ptr() - 
				  (char *) memoryMap_.addr()) * 100. / 
			(double) memoryMap_.size());
  return rc;
}

void 
LogFile::sendEvent()
{
  MIRO_ASSERT(coursor_ != timeVector_.end());

  emit notifyEvent(domainName_ + " - " + typeName_);
  
  supplier_->sendEvent(event_);
}

bool
LogFile::nextEvent()
{
  MIRO_ASSERT (coursor_ != timeVector_.end());

  do {
    ++coursor_;
    if (coursor_ == timeVector_.end()) {
      return false;
    }

    TAO_InputCDR istr(coursor_->second, 
		      memoryMap_.size() - (coursor_->second - (char *)memoryMap_.addr()));
    istr >> event_;
  }
  while (!validEvent());

  return true;
}

bool
LogFile::getCurrentEvent()
{
  if (coursor_ == timeVector_.end())
    return false;

  TAO_InputCDR istr(coursor_->second, 
		    memoryMap_.size() - (coursor_->second - (char *)memoryMap_.addr()));
  istr >> event_;
  while (!validEvent()) {
    ++coursor_;
    if (coursor_ == timeVector_.end()) {
      return false;
    }
    
    TAO_InputCDR istr(coursor_->second, 
		      memoryMap_.size() - (coursor_->second - (char *)memoryMap_.addr()));
    istr >> event_;
  }

  parseEvent();

  return true;
}

bool
LogFile::prevEvent()
{
  do {
    if (coursor_ == timeVector_.begin())
      return false;
    --coursor_;

    TAO_InputCDR istr(coursor_->second, 
		      memoryMap_.size() - (coursor_->second - (char *)memoryMap_.addr()));
    istr >> event_;
  }
  while (!validEvent());
  
  return true;
}

bool
LogFile::assertBefore(ACE_Time_Value const& _t)
{
  bool valid = true;
  while(valid && coursor_->first >= _t) {
    valid = prevEvent();
  }
  return valid;
}
 
bool
LogFile::assertAfter(ACE_Time_Value const& _t)
{
  bool valid = true;
  while (valid && coursor_->first <= _t) {
    valid = nextEvent();
  }
  return valid;
}

void
LogFile::clearExclude()
{
  exclude_.clear();
}

void
LogFile::addExclude(QString const& _typeName)
{
  if (typeNames_.find(_typeName.latin1()) == typeNames_.end())
    return;

  assert(std::find(exclude_.begin(), exclude_.end(), _typeName) == exclude_.end());

  exclude_.push_back(_typeName);
}

void
LogFile::delExclude(QString const& _typeName)
{
  if (typeNames_.find(_typeName.latin1()) == typeNames_.end())
    return;

  QStringVector::iterator iter = 
    std::find(exclude_.begin(), exclude_.end(), _typeName);
  assert(iter != exclude_.end());

  exclude_.erase(iter);
}

bool
LogFile::validEvent() const
{
  // skip excluded events
  QStringVector::const_iterator first, last = exclude_.end();
  for (first = exclude_.begin(); first != last; ++first) {
    if (strcmp(first->latin1(), 
	       (char const *)event_.header.fixed_header.event_type.type_name) == 0) {
     return false;
    }
  }
  return true;
}

void
LogFile::parseEvent()
{
  // localize debug hack
  //  if (channelManager_->debugLocalize() && typeName_ == "LineSamples")
  //   event_.header.fixed_header.event_type.type_name = CORBA::string_dup( "RawLineSamples" );

  domainName_ = (char const *)event_.header.fixed_header.event_type.domain_name;
  typeName_ = (char const *)event_.header.fixed_header.event_type.type_name;
  eventName_ = (char const *)event_.header.fixed_header.event_name;

  ACE_Time_Value t = coursorTime() - timeOffset_;
  char const * null = "000000";
  QString sec;
  QString usec;
  sec.setNum(t.sec());
  usec.setNum(t.usec());
  stamp_ = (null + sec.length()) + sec + "." + (null + usec.length()) + usec;

  emit newEvent(stamp_, domainName_, typeName_, eventName_);
}
