// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "LogFile.h"
#include "ChannelManager.h"

#include "miro/StructuredPushSupplier.h"
#include "miro/TimeC.h"
#include "miro/TimeHelper.h"

ACE_Time_Value const LogFile::T_NULL(0, 0);

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
LogFile::playEvent() 
{
  assert(coursor_ != timeVector_.end());

  TAO_InputCDR istr(coursor_->second, 
		    memoryMap_.size() - (coursor_->second - (char *)memoryMap_.addr()));
   
  CosNotification::StructuredEvent event;
  istr >> event;
  
  char const * const s = event.header.fixed_header.event_type.type_name;
  QString const eventName = s;
  QStringVector::const_iterator first, last = exclude_.end();
  for (first = exclude_.begin(); first != last; ++first) {
    if (*first == eventName) {
      ++coursor_;
      return;
    }
  }

#ifdef DEBUG_LOCALIZATION  
  if (eventName == "LineSamples")
    event.header.fixed_header.event_type.type_name = CORBA::string_dup( "RawLineSamples" );
#endif // DEBUG_LOCALIZATION

  emit notifyEvent(QString(event.header.fixed_header.event_type.domain_name) +
		   QString(" - ") +
		   QString(event.header.fixed_header.event_type.type_name));
  
  supplier_->sendEvent(event);

  ++coursor_;
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
