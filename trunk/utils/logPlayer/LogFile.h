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
#ifndef LogFile_h
#define LogFile_h

#include "miro/Log.h"

#include <orbsvcs/CosNotifyChannelAdminC.h>

#include <ace/Mem_Map.h>
#include <ace/Time_Value.h>

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>

#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <utility>
#include <cstring>

// forward declaration
class TAO_InputCDR ;
class ChannelManager;
namespace Miro
{
  class StructuredPushSupplier;
};

class LogFile : public QObject
{
  Q_OBJECT

private:
  typedef QObject Super;

protected:
  typedef std::pair< ACE_Time_Value, char * > TimePair;
  typedef std::vector< TimePair > TimeVector;
  typedef std::vector< QString > QStringVector;

  struct LtStr : public std::binary_function<char const *, char const *, bool>
  {
    bool operator()(char const * s1, char const * s2) const {
      return strcmp(s1, s2) < 0;
    }
  };

  struct TVLess : public std::binary_function<TimePair const&, TimePair const &, bool>
  {
    bool operator() (TimePair const & _lhs, TimePair const & _rhs) {
      return _lhs.first < _rhs.first;
    }
  };

public:
  //  typedef std::set< char const *, LtStr > CStringSet;
  typedef std::set< QString > CStringSet;

public:
  LogFile(QString const& _name,
	  ChannelManager * _channelManager);
  ~LogFile();

  QString const& name() const;
  ACE_Time_Value const& startTime() const;
  ACE_Time_Value const& endTime() const;

  ACE_Time_Value const& coursorTime() const;
  void coursorTime(ACE_Time_Value const& _t);

  void sendEvent();
  bool nextEvent();
  bool prevEvent();
  bool getCurrentEvent();

  unsigned int parse();
  bool parsed() const;
  void parseEvent();

  void clearExclude();
  void addExclude(QString const& _typeName);
  void delExclude(QString const& _typeName);

  QString const& domainName() const;
  CStringSet const& typeNames() const;

  void setTimeOffset(ACE_Time_Value const& _offset);
  bool assertBefore(ACE_Time_Value const& _t);
  bool assertAfter(ACE_Time_Value const& _t);

signals:
  void notifyEvent(QString const&);
  void newEvent(QString const&,QString const&,QString const&,QString const&);

protected:
  bool validEvent() const;

  QString name_;
  ChannelManager * const channelManager_;

  ACE_Mem_Map memoryMap_;

  CosNotification::StructuredEvent event_;
  QString stamp_;
  QString domainName_;
  QString typeName_;
  QString eventName_;

  ACE_Time_Value timeOffset_;
  TimeVector timeVector_;
  TimeVector::const_iterator coursor_;

  CStringSet typeNames_;

  CosNotifyChannelAdmin::EventChannel_var ec_;
  Miro::StructuredPushSupplier * supplier_;

  QStringVector exclude_;

  TAO_InputCDR * istr_;    
  bool parsed_;
};

inline
QString const&
LogFile::name() const
{
  return name_;
}

inline
ACE_Time_Value const&
LogFile::startTime() const
{
  assert(timeVector_.size() != 0);
  return timeVector_.front().first;
}

inline
ACE_Time_Value const&
LogFile::endTime() const
{
  assert(timeVector_.size() != 0);
  return timeVector_.back().first;
}

inline
ACE_Time_Value const&
LogFile::coursorTime() const
{
  return (coursor_ != timeVector_.end())? coursor_->first : ACE_Time_Value::zero;
}

inline
void
LogFile::coursorTime(ACE_Time_Value const& _t)
{
  coursor_ = std::lower_bound(timeVector_.begin(), timeVector_.end(), TimePair(_t, NULL), TVLess());
}

inline
bool
LogFile::parsed() const
{
  return parsed_;
}

inline
QString const&
LogFile::domainName() const
{
  return domainName_;
}

inline
LogFile::CStringSet const&
LogFile::typeNames() const
{
  return typeNames_;
}

inline
void
LogFile::setTimeOffset(ACE_Time_Value const& _offset) {
  timeOffset_ = _offset;
}
#endif
