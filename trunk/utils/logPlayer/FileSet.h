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

#include <qstring.h>
#include <qstringlist.h>

#include <vector>
#include <map>

// forward declarations
class ChannelManager;

class FileSet : public QObject
{
  Q_OBJECT

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------
  typedef QObject Super;

public:
  //----------------------------------------------------------------------------
  // public types
  //----------------------------------------------------------------------------
  typedef std::vector<LogFile *> FileVector;
  typedef std::map<QString, LogFile::CStringSet> DNETMap; // domain_name type_came map

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------
  FileSet(ChannelManager * _channelManager);
  virtual ~FileSet();

  LogFile * addFile(QString const& _name);
  void delFile(QString const& _name);

  unsigned int size() const;
  QStringList files() const;

  void calcStartEndTime();
  ACE_Time_Value const& startTime() const;
  ACE_Time_Value const& endTime() const;


  //! Play events till specified time
  void playEvents(ACE_Time_Value const& _t);
  void playBackwards();

  //! Get @ref _num events from specified time.
  void getEvents(ACE_Time_Value const& _t, unsigned int _num);

  DNETMap typeNames() const;
  void clearExclude();

  FileVector& fileVector() { return file_; }

  //! Current coursor event time.
  ACE_Time_Value const& coursorTime() const;
  void coursorTime(ACE_Time_Value const& _t);
  void coursorTimeRel(ACE_Time_Value const& _t);

  //----------------------------------------------------------------------------
  // public slots
  //----------------------------------------------------------------------------
public slots:
  void addExclude(QString const& _domainName, QString const& _typeName);
  void delExclude(QString const& _domainName, QString const& _typeName);

  //----------------------------------------------------------------------------
  // signals
  //----------------------------------------------------------------------------
signals:
  void intervalChange();
  void coursorChange();

  //----------------------------------------------------------------------------
  // private data
  //----------------------------------------------------------------------------
private:
  //! Reference to the channel manager.
  ChannelManager * const channelManager_;
  //! The set of log files to work on.
  FileVector file_;

  //! The base time of all log files.
  ACE_Time_Value startTime_;
  //! The ent time of all log files.
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
