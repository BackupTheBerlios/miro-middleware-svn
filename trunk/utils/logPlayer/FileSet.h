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
#ifndef FileManager_h
#define FileManager_h

#include "LogFile.h"

#include <qstringlist.h>

#include <vector>

// forward declarations
class ChannelManager;

class FileSet : public QObject
{
  Q_OBJECT

  typedef QObject Super;
public:
  FileSet(ChannelManager * _channelManager);
  ~FileSet();

  LogFile * addFile(QString const& _name);
  void delFile(QString const& _name);

  unsigned int size() const;
  QStringList files() const;

  void calcStartEndTime();
  ACE_Time_Value const& startTime() const;
  ACE_Time_Value const& endTime() const;

  ACE_Time_Value const& coursorTime() const;
  void coursorTime(ACE_Time_Value const& _t);

  void playEvent(ACE_Time_Value const& _t);
  void playLast();

signals:
  void intervalChange();
  void coursorChange();

protected:
  typedef std::vector<LogFile *> FileVector;

  ChannelManager * const channelManager_;
  FileVector file_;

  ACE_Time_Value startTime_;
  ACE_Time_Value endTime_;
};

inline
unsigned int 
FileSet::size() const {
  return file_.size();
}

inline
ACE_Time_Value const&
FileSet::startTime() const {
  assert(file_.size() != 0);
  return startTime_;
}

inline
ACE_Time_Value const&
FileSet::endTime() const {
  assert(file_.size() != 0);
  return endTime_;
}

inline
ACE_Time_Value const&
FileSet::coursorTime() const {
  assert(file_.size() != 0);
  return (file_.front()->coursorTime() != ACE_Time_Value(0, 0))?
    file_.front()->coursorTime() : endTime_;
}


#endif // FileManager_h
