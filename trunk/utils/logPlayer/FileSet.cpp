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

#include "FileSet.h"
#include "ChannelManager.h"

#include <miro/TimeHelper.h>

#define QT_NO_TEXTSTREAM
#include <qtl.h>

namespace
{
  struct LFLess : public std::binary_function<LogFile const *, LogFile const *, bool>
  {
    bool operator() (LogFile const * _lhs, LogFile const * _rhs) {
      if (_lhs->coursorTime() == ACE_Time_Value(0, 0))
	return false;
      return (_lhs->coursorTime() < _rhs->coursorTime() ||
	      _rhs->coursorTime() == ACE_Time_Value(0, 0));
    }
  };


  struct LFMore : public std::binary_function<LogFile const *, LogFile const *, bool>
  {
    bool operator() (LogFile const * _lhs, LogFile const * _rhs) {
      if (_lhs->coursorTime() == ACE_Time_Value(0, 0))
	return true;
      return (_lhs->coursorTime() > _rhs->coursorTime() &&
	      _rhs->coursorTime() != ACE_Time_Value(0, 0));
    }
  };
}

FileSet::FileSet(ChannelManager * _channelManager) :
  channelManager_(_channelManager)
{}

FileSet::~FileSet()
{
  FileVector::iterator first, last = file_.end();
  for (first = file_.begin(); first != last; ++first)
    delete *first;
}

LogFile *
FileSet::addFile(QString const& _name)
{
  FileVector::const_iterator first, last = file_.end();
  for (first = file_.begin(); first != last; ++first)
    if ((*first)->name() == _name)
      break;
  assert(first == file_.end());

  LogFile * const logFile = new LogFile(_name, channelManager_);
  file_.push_back(logFile);

  return logFile;
}

void
FileSet::delFile(QString const& _name)
{
  FileVector::iterator first, last = file_.end();
  for (first = file_.begin(); first != last; ++first)
    if ((*first)->name() == _name)
      break;
  assert (first != file_.end());

  ACE_Time_Value t;
  bool parsed = (*first)->parsed();
  if (parsed)
    t = coursorTime();

  delete *first;
  file_.erase(first);
  
  if (parsed && file_.size() != 0) {
    calcStartEndTime();
    coursorTime(t);
  }
}

void
FileSet::calcStartEndTime() 
{
  if (file_.size() != 0) {
    FileVector::const_iterator first, last = file_.end();
    first = file_.begin();
    startTime_ = (*first)->startTime();
    endTime_ = (*first)->endTime();
    
    for (++first; first != last; ++first ) {
      if ((*first)->startTime() < startTime_) {
	startTime_ = (*first)->startTime();
      }
      if ((*first)->endTime() > endTime_) {
	endTime_ = (*first)->endTime();
      }
    }
    for (first = file_.begin(); first != last; ++first ) {
      (*first)->setTimeOffset(startTime_);
    }
  }

  emit intervalChange();
}

void
FileSet::coursorTime(ACE_Time_Value const& _time)
{
  FileVector::const_iterator first, last = file_.end();
  for (first = file_.begin(); first != last; ++first) {
    (*first)->coursorTime(_time);
  }

  std::make_heap(file_.begin(), file_.end(), LFLess());
  std::cout << "new time heap: " << std::endl;
  for (first = file_.begin(); first != last; ++first) {
    std::cout << (*first)->coursorTime()  - startTime_ << std::endl;
  }

  file_.front()->getCurrentEvent();

  emit coursorChange();
}

void
FileSet::coursorTimeRel(ACE_Time_Value const& _time)
{
  ACE_Time_Value t = startTime_ + _time;
  coursorTime(t);
}

void
FileSet::playEvents(ACE_Time_Value const& _time)
{
  assert(file_.size() != 0);

  // correct heap if we change direction
  FileVector::const_iterator first, last = file_.end();
  first = file_.begin();
  for (++first; first != last; ++first)
    (*first)->assertAfter(file_.front()->coursorTime());

  while (file_.front()->coursorTime() != ACE_Time_Value(0, 0) &&
	 file_.front()->coursorTime() <= _time) {
    file_.front()->sendEvent();
    file_.front()->nextEvent();

    // restore heap attibute
    std::make_heap(file_.begin(), file_.end(), LFLess());
    std::cout << "new time heap: " << std::endl;
    FileVector::const_iterator first, last = file_.end();
    for (first = file_.begin(); first != last; ++first) {
      std::cout << (*first)->coursorTime() - startTime_ << std::endl;
    }

    file_.front()->parseEvent();
  }

  emit coursorChange();
}

void
FileSet::playBackwards()
{
  assert(file_.size() != 0);

  // correct heap if we change direction
  FileVector::const_iterator first, last = file_.end();
  first = file_.begin();
  for (++first; first != last; ++first)
    (*first)->assertBefore(file_.front()->coursorTime());

  file_.front()->sendEvent();
  file_.front()->prevEvent();

  // restore heap attibute
  std::make_heap(file_.begin(), file_.end(), LFMore());
  std::cout << "new time heap: " << std::endl;
  for (first = file_.begin(); first != last; ++first) {
    std::cout << (*first)->coursorTime()  - startTime_ << std::endl;
  }

  file_.front()->parseEvent();

  emit coursorChange();
}

void 
FileSet::getEvents(ACE_Time_Value const& _time, unsigned int _num)
{
  // prepare log file set
  ACE_Time_Value now = coursorTime();
  
  // set new time
  coursorTimeRel(_time);

  while (file_.front()->coursorTime() != ACE_Time_Value(0, 0) &&
	 --_num > 0) {
    file_.front()->nextEvent();
    std::make_heap(file_.begin(), file_.end(), LFLess());
    file_.front()->parseEvent();
  }

  // restor coursor time
  coursorTime(now);
}

QStringList
FileSet::files() const
{
  QStringList l;
  FileVector::const_iterator first, last = file_.end();
  for (first = file_.begin(); first != last; ++first)
    l.append((*first)->name());
  qHeapSort(l);

  return l;
}

FileSet::DNETMap 
FileSet::typeNames() const 
{
  DNETMap tree;
  FileVector::const_iterator first, last = file_.end();
  for (first = file_.begin(); first != last; ++first) {
    DNETMap::iterator iter = tree.find((*first)->domainName());
    if (iter == tree.end())
      tree.insert(std::make_pair((*first)->domainName(), (*first)->typeNames()));
    else 
      iter->second.insert((*first)->typeNames().begin(), (*first)->typeNames().end());
  }
  
  return tree;
}

void
FileSet::clearExclude()
{
  FileVector::const_iterator first, last = file_.end();
  for (first = file_.begin(); first != last; ++first) {
    (*first)->clearExclude();
  }
}

void
FileSet::addExclude(QString const& _domainName, QString const& _typeName)
{
  FileVector::const_iterator first, last = file_.end();
  for (first = file_.begin(); first != last; ++first) {
    if ((*first)->domainName() == _domainName)
      (*first)->addExclude(_typeName);
  }
}

void
FileSet::delExclude(QString const& _domainName, QString const& _typeName)
{
  FileVector::const_iterator first, last = file_.end();
  for (first = file_.begin(); first != last; ++first) {
    if ((*first)->domainName() == _domainName)
      (*first)->delExclude(_typeName);
  }
}
