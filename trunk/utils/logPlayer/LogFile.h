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
#ifndef LogFile_h
#define LogFile_h

#include <orbsvcs/CosNotifyChannelAdminC.h>

#include <ace/Mem_Map.h>
#include <ace/Time_Value.h>

#include <qobject.h>
#include <qstring.h>

#include <vector>
#include <algorithm>
#include <functional>
#include <utility>

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

public:
  LogFile(QString const& _name,
	  ChannelManager * _channelManager);
  ~LogFile();

  QString const& name() const;
  ACE_Time_Value const& startTime() const;
  ACE_Time_Value const& endTime() const;

  ACE_Time_Value const& coursorTime() const;
  void coursorTime(ACE_Time_Value const& _t);

  void playEvent();
  unsigned int parse();
  bool parsed() const;

signals:
  void notifyEvent(const QString&);
 
protected:
  typedef std::pair< ACE_Time_Value, char * > TimePair;
  typedef std::vector< TimePair > TimeVector;
  typedef std::vector< QString > QStringVector;

  QString name_;
  ChannelManager * const channelManager_;

  ACE_Mem_Map memoryMap_;

  TimeVector timeVector_;
  TimeVector::const_iterator coursor_;

  QString domainName_;
  CosNotifyChannelAdmin::EventChannel_var ec_;
  Miro::StructuredPushSupplier * supplier_;

  QStringVector exclude_;

  TAO_InputCDR * istr_;    
  bool parsed_;

  static const ACE_Time_Value T_NULL;

  struct TVLess : public std::binary_function<TimePair const&, TimePair const &, bool>
  {
    bool operator() (TimePair const & _lhs, TimePair const & _rhs) {
      return _lhs.first < _rhs.first;
    }
  };
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
  return (coursor_ != timeVector_.end())? coursor_->first : T_NULL;
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
#endif
