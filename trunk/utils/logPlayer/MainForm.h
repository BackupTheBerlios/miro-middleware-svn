// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// $Date$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Mainform_h
#define MainForm_h

#include <ace/Time_Value.h>

#include <qmainwindow.h>

// forward declarations
class FileSet;
class FileListDialog;
class EventView;

class QApplication;
class QPopupMenu;
class QTimer;
class QSlider;
class QLCDNumber;
class QDial;


class SubmenuEvent : public QObject
{
  Q_OBJECT

private:
  typedef QObject Super;

public:
  SubmenuEvent(QObject * parent, char const * name);
  void setParentId(int _id);

public slots:
  void action(int);

signals:
  void activated(int, int);

protected:
  int parentId_;
};

inline
void
SubmenuEvent::setParentId(int _id) {
  parentId_ = _id;
}

class MainForm : public QMainWindow
{
  Q_OBJECT

  typedef QMainWindow Super;
public:
  MainForm(QApplication& _app, FileSet& _fileSet,
	   QWidget * parent = 0, const char * name = 0 );

  void loadFile(QString const & _name);
  void calcStartTime();

  void addExclude(QString const & _eventName);

public slots:
  void pause();
  void play();
  void stop();
  void next();
  void prev();
  void files();
  void changed( int );
  void step();
  void beginAction();
  void endAction();
  void timeAction( int );
  void speed( int );

  void scaleSlider();
  void setSlider();
  void setHistory();

  void toggleExcludeEvent(int, int);

  void toggleEventView();
  void eventViewClosed();

  void saveAs();

  void cutFront();
  void cutBack();
  void cutUndo();

signals:
  void excludeEvent(const QString &, const QString&);
  void includeEvent(const QString &, const QString&);

protected:
  void enableButtons(bool _flag);
  void createEventMenu();

  QApplication& app_;
  FileSet& fileSet_;
  FileListDialog * fileListDialog_;
  EventView * eventView_;
  QTimer * timer_;

  QPopupMenu *eventMenu_;
  QPopupMenu * toolsMenu_;

  QWidget *playButton;
  QWidget *stopButton;
  QWidget *pauseButton;
  QWidget *nextButton;
  QWidget *prevButton;
  QSlider     *timeSlider;
  QLCDNumber  *centiSecLabel;
  QLCDNumber  *secLabel;
  QLCDNumber  *minLabel;
  QDial       *speedDial;

  bool action_;
  int speed_;
  int history_;
  ACE_Time_Value timeBase_;
  ACE_Time_Value timeCBase_;

  int domainNameMenuId_;
  int eventViewId_;

  static ACE_Time_Value const MIN_TIME;
};

#endif
