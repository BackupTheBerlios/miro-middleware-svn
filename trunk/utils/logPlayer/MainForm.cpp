// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
// 
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// $Date$
// 
//////////////////////////////////////////////////////////////////////////////

#include "MainForm.h"
#include "LogFile.h"
#include "FileSet.h"

#include "../widgets/FileListDialog.h"

#include "miro/Exception.h"

#include <ace/OS.h>

#define INCLUDE_MENUITEM_DEF
#include <qapplication.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlcdnumber.h>
#include <qtimer.h>
#include <qdial.h>
#include <qprogressdialog.h>
#include <qmessagebox.h>

#include <cstring>

namespace 
{
  char const * filter[] = {
    "log files (*.log)",
    "all files (*)",
    NULL
  };
};

SubmenuEvent::SubmenuEvent(QObject * parent, char const * name) :
  Super(parent, name),
  parentId_(0)
{}

void
SubmenuEvent::action(int _id)
{
  emit activated(parentId_, _id);
}


ACE_Time_Value const MainForm::MIN_TIME(0, 20000);

MainForm::MainForm(QApplication& _app, FileSet& _fileSet,
		   QWidget *parent, const char *name ) :
  Super(parent, name),
  app_(_app),
  fileSet_(_fileSet),
  fileListDialog_(new FileListDialog(this, "File list dialog", "Log files", filter)),
  timer_(new QTimer()),
  eventMenu_(new QPopupMenu( this )),
  action_(false),
  speed_(1),
  domainNameMenuId_(0)
{
  // the menu
  QPopupMenu *fileMenu = new QPopupMenu( this );
  fileMenu->insertItem( "&Open/Close...", this, SLOT( files() ) );
  fileMenu->insertSeparator();
  fileMenu->insertItem( "&Quit", qApp, SLOT( quit() ) );

  menuBar()->insertItem( "&File", fileMenu );    
  menuBar()->insertItem( "&Events", eventMenu_ );    

  // the widgets
  QWidget * cw = new QWidget(this, "central widget");

  playButton = new QPushButton( "Play", cw, "play" );
  playButton->setMinimumSize(90, 30);

  stopButton = new QPushButton( "Stop", cw, "stop" );
  stopButton->setMinimumSize(90, 30);

  pauseButton = new QPushButton( "Pause", cw, "pause" );
  pauseButton->setMinimumSize(90, 30);

  nextButton = new QPushButton( "Next", cw, "next" );
  nextButton->setMinimumSize(90, 30);

  prevButton = new QPushButton( "Prev", cw, "prev" );
  prevButton->setMinimumSize(90, 30);
  
  minLabel = new QLCDNumber(2 , cw, "min");
  secLabel = new QLCDNumber(2, cw, "sec" );
  centiSecLabel = new QLCDNumber( 2, cw, "csec" );
  
  timeSlider = new QSlider( Horizontal, cw, "time" );
  timeSlider->setRange(0, 1000);
  timeSlider->setValue(0);

  speedDial = new QDial( -20, 20, 1, 0, cw, "speed" );
  speedDial->setWrapping(false);
  speedDial->setNotchesVisible(true);
  speedDial->setLineStep(2);
  speedDial->setPageStep(5);

  // ... add to the layout 
  QBoxLayout *topLayout = new QHBoxLayout(cw, 5);
  QBoxLayout *layout1 = new QVBoxLayout(topLayout, 5);
  QGridLayout *layout2 = new QGridLayout(layout1, 2, 3, 5);
  QBoxLayout *layout3 = new QHBoxLayout(-1, "time layout");

  layout2->addWidget(playButton, 0, 0);
  layout2->addWidget(stopButton, 0, 1);
  layout2->addWidget(pauseButton, 0, 2);
  layout2->addWidget(prevButton, 1, 0);
  layout2->addLayout(layout3, 1, 1);
  layout2->addWidget(nextButton, 1, 2);

  layout3->addWidget(minLabel);
  layout3->addWidget(secLabel);
  layout3->addWidget(centiSecLabel);

  layout1->addSpacing(10);
  layout1->addWidget(timeSlider);
  topLayout->addWidget(speedDial);

  statusBar();
  setCentralWidget(cw);

  // signals n slots
  connect( timer_, SIGNAL( timeout() ),
	   (MainForm *)this, SLOT( step() ) );

  connect( timeSlider, SIGNAL( valueChanged(int) ),
	   this, SLOT( changed(int) ) );
  connect( timeSlider, SIGNAL( sliderPressed() ),
	   (MainForm *)this, SLOT( beginAction() ) );
  connect( timeSlider, SIGNAL( sliderReleased() ),
	   (MainForm *)this, SLOT( endAction() ) );
  connect( timeSlider, SIGNAL( sliderMoved( int ) ),
	   (MainForm *)this, SLOT( timeAction( int ) ) );

  connect( &fileSet_, SIGNAL( intervalChange() ),
	   (MainForm *)this, SLOT( scaleSlider() ));
  connect( &fileSet_, SIGNAL( coursorChange() ),
	   (MainForm *)this, SLOT( setSlider() ));
  
  connect( playButton, SIGNAL( clicked() ),
	   (MainForm *)this, SLOT( play() ) );
  connect( stopButton, SIGNAL( clicked() ),
	   (MainForm *)this, SLOT( stop() ) );
  connect( pauseButton, SIGNAL( clicked() ),
	   (MainForm *)this, SLOT( pause() ) );
  connect( prevButton, SIGNAL( clicked() ),
	   (MainForm *)this, SLOT( prev() ) );
  connect( nextButton, SIGNAL( clicked() ),
	   (MainForm *)this, SLOT( next() ) );

  connect( speedDial, SIGNAL( valueChanged( int ) ),
	   (MainForm *)this, SLOT( speed( int ) ) );
  connect( speedDial, SIGNAL( dialPressed() ),
	   (MainForm *)this, SLOT( beginAction() ) );
  connect( speedDial, SIGNAL( dialReleased() ),
	   (MainForm *)this, SLOT( endAction() ) );

  enableButtons(fileSet_.size() != 0);
}

// TODO: implement exclude from command line
void
MainForm::addExclude(QString const& /*_eventName*/)
{
}

void 
MainForm::enableButtons(bool _flag)
{
  playButton->setEnabled(_flag);
  stopButton->setEnabled(_flag);
  pauseButton->setEnabled(_flag);
  nextButton->setEnabled(_flag);
  prevButton->setEnabled(_flag);
  timeSlider->setEnabled(_flag);
  speedDial->setEnabled(_flag);

  if (!_flag) {
    timeSlider->setValue(0);
    centiSecLabel->display(0);
    secLabel->display(0);
    minLabel->display(0);
    speedDial->setValue(0);
  }

  eventMenu_->setEnabled(eventMenu_->count() != 0);
}

void
MainForm::scaleSlider()
{
  ACE_Time_Value d = fileSet_.endTime() - fileSet_.startTime();
  timeSlider->setMinValue(0);
  timeSlider->setMaxValue(d.sec() * 100 + d.usec() / 10000);
}

void
MainForm::setSlider()
{
  ACE_Time_Value d = fileSet_.coursorTime() - fileSet_.startTime();
  timeSlider->setValue(d.sec() * 100 + d.usec() / 10000);
}


void
MainForm::files()
{
  QStringList oldFiles = fileSet_.files();
  fileListDialog_->init(oldFiles);
  if (fileListDialog_->exec() &&
      fileListDialog_->modified()) {
    QStringList newFiles = fileListDialog_->result();

    QStringList::Iterator first, last = oldFiles.end();
    for (first = oldFiles.begin(); first != last; ++first) {
      if (newFiles.find(*first) == newFiles.end())
	fileSet_.delFile(*first);
    }

    last = newFiles.end();
    for (first = newFiles.begin(); first != last; ++first) {
      if (oldFiles.find(*first) == oldFiles.end()) 
	loadFile(*first);
    }
    enableButtons(fileSet_.size() != 0);
  }
}

void
MainForm::calcStartTime() 
{
  timeBase_ = ACE_OS::gettimeofday();
  timeCBase_ = fileSet_.coursorTime();
}

void 
MainForm::changed( int )
{
  ACE_Time_Value t = fileSet_.coursorTime() - fileSet_.startTime();

  minLabel->display( int(t.sec() / 60) );
  secLabel->display( int(t.sec() % 60) );
  centiSecLabel->display( int(t.usec() / 10000 ) );
}

void
MainForm::play() 
{
  calcStartTime();
  timer_->start(0);
}

void 
MainForm::stop() 
{
  timer_->stop();
  fileSet_.coursorTime(fileSet_.startTime());
}

void MainForm::pause() 
{
  timer_->stop();
}

void
MainForm::next() 
{
  if (!timer_->isActive()) {
    fileSet_.playEvent(fileSet_.coursorTime());
  }
}

// TODO: implement that
void 
MainForm::prev() 
{
  if (!timer_->isActive()) {

  }
}

void
MainForm::step() 
{
  if (!action_) {
    
    ACE_Time_Value destTime = ACE_OS::gettimeofday() + MIN_TIME - timeBase_;
    destTime *= (speed_ > 0)? (double)speed_ : (1. / (double)-speed_);
    destTime += timeCBase_;
    destTime = std::min(destTime, fileSet_.coursorTime() + ACE_Time_Value(0, 200000));
   
    fileSet_.playEvent(destTime);

    if (fileSet_.coursorTime() != fileSet_.endTime()) {
      // rescedule timer
      ACE_Time_Value nextTime = ACE_OS::gettimeofday() - timeBase_;
      nextTime *= (speed_ > 0)? (double)speed_ : (1. / (double)-speed_);
      nextTime += timeCBase_;

      // TODO fix that too
      int interval = (fileSet_.coursorTime() <= nextTime)?
	0 :
	(fileSet_.coursorTime() - nextTime).msec();
      timer_->changeInterval(interval);
    }
    else {
      timer_->stop();
    }
  }
}

void
MainForm::beginAction() 
{
  action_ = true;
}

void 
MainForm::endAction()
{
  calcStartTime();
  action_ = false;
}

void
MainForm::timeAction(int hsec ) 
{
  ACE_Time_Value t(hsec / 100, (hsec % 100) * 10000);
  fileSet_.coursorTime(fileSet_.startTime() + t);
}

void
MainForm::speed( int _speed) 
{
  speed_ = _speed;
  if (speed_ == 0)
    speed_ = 1;
}

void 
MainForm::loadFile(QString const & _name ) 
{
  bool rc = true;
  try {
    LogFile * file = fileSet_.addFile(_name);
    try {
      QProgressDialog progress("Parsing log file " + _name, "Cancel", 100,
			       this, "progress", TRUE);
      progress.setProgress(0);
      int percent;

      while ((percent = file->parse()) != 100) {
	progress.setProgress(percent);
	qApp->processEvents();

	if (progress.wasCancelled())
	  throw Miro::Exception("canceled");
      }

      if (file->endTime() > ACE_OS::gettimeofday())
	throw Miro::Exception("Clock screw detected:\nEnd time of file lies in the future.");

      statusBar()->message(QString("loaded file: ") + _name, 5000);
      fileSet_.calcStartEndTime();
      fileSet_.coursorTime(fileSet_.coursorTime());

      connect(file, SIGNAL(notifyEvent(const QString&)), 
	      statusBar(), SLOT(message(const QString&)));
    }
    catch (Miro::Exception const& e) {
      fileSet_.delFile(_name);
      if (strcmp("canceled", e.what()) == 0)
	statusBar()->message("loading file canceled.", 5000);
      else
	QMessageBox::warning(this, "Error parsing file:",
			     QString("File ") + _name + QString(":\n") +
			     QString(e.what()));
      rc = false;
    }
  }
  catch (Miro::CException const& e) {
    QMessageBox::warning(this, "Error loading file:",
			 QString("File ") + _name + QString(":\n") +
			 QString(e.what()));
    rc = false;
  }
  createEventMenu();
  enableButtons(fileSet_.size() != 0);
}

void
MainForm::createEventMenu()
{
  FileSet::DNETMap m = fileSet_.typeNames();

  fileSet_.clearExclude();
  eventMenu_->clear();

  FileSet::DNETMap::const_iterator first, last = m.end();
  for (first = m.begin(); first != last; ++first) {
    QPopupMenu *typeNames = new QPopupMenu(eventMenu_);
    SubmenuEvent *submenuEvent = new SubmenuEvent(typeNames, "subEvent");

    connect(typeNames, SIGNAL(activated(int)), submenuEvent, SLOT(action(int)));
    connect(submenuEvent, SIGNAL(activated(int, int)), this, SLOT(toggleExcludeEvent(int, int)));

    typeNames->setCheckable(true);
    LogFile::CStringSet::const_iterator f, l = first->second.end();
    for (f = first->second.begin(); f != l; ++f) {
      int id = typeNames->insertItem(QString(*f));
      typeNames->setItemChecked(id, true);
    }

    int id = eventMenu_->insertItem(first->first, typeNames);
    submenuEvent->setParentId(id);
  }
}

void
MainForm::toggleExcludeEvent(int _domainNameId, int _eventTypeId)
{
  QString domainName = eventMenu_->text(_domainNameId);
  QMenuItem * domainNameItem = eventMenu_->findItem(_domainNameId);
  QPopupMenu * domainNameMenu = domainNameItem->popup();

  domainNameMenu->setItemChecked(_eventTypeId, 
				 !domainNameMenu->isItemChecked(_eventTypeId));
  // enable event
  if (domainNameMenu->isItemChecked(_eventTypeId)) {
    fileSet_.delExclude(domainName, domainNameMenu->text(_eventTypeId));
  }
  // disable event
  else {
    fileSet_.addExclude(domainName, domainNameMenu->text(_eventTypeId));
  }
}
