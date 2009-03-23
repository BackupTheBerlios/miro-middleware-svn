// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "MainForm.h"
#include "LogFile.h"
#include "FileSet.h"
#include "EventView.h"
#include "ChannelManager.h"

#include "../widgets/FileListDialog.h"

#include "miro/Exception.h"
#include "miro/TimeHelper.h"

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
#include <qinputdialog.h>
#include <qfiledialog.h>
#include <qobjectlist.h>

#include <cstring>

using namespace std;

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
  eventView_(NULL),
  timer_(new QTimer(this)),
  autoStartTimer_(new QTimer(this)),
  eventMenu_(new QPopupMenu( this )),
  action_(false),
  speed_(1),
  history_(100),
  domainNameMenuId_(0),
  exitOnReplayEnd_(false),
  numConsumers_(0)
{
  // the menu
  QPopupMenu *fileMenu = new QPopupMenu( this );
  fileMenu->insertItem( "&Open/Close...", this, SLOT( files() ) );
  fileMenu->insertItem("Save &as...", this, SLOT(saveAs()));
  fileMenu->insertSeparator();
  fileMenu->insertItem( "&Quit", qApp, SLOT( quit() ) );

  QPopupMenu * editMenu = new QPopupMenu(this);
  editMenu->insertItem("Cut Front", this, SLOT(cutFront()));
  editMenu->insertItem("Cut Back", this, SLOT(cutBack()));
  editMenu->insertItem("Undo all", this, SLOT(cutUndo()));

  toolsMenu_ = new QPopupMenu(this);
  toolsMenu_->setCheckable(true);
  eventViewId_ = toolsMenu_->insertItem("&Event View", this, SLOT(toggleEventView()));

  QPopupMenu * settingsMenu = new QPopupMenu(this);
  settingsMenu->insertItem("&History", this, SLOT(setHistory()));

  menuBar()->insertItem("&File", fileMenu);
  menuBar()->insertItem("&Edit", editMenu);
  menuBar()->insertItem("E&vents", eventMenu_ );    
  menuBar()->insertItem("&Tools", toolsMenu_);    
  menuBar()->insertItem("&Settings", settingsMenu);    

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

  connect( autoStartTimer_, SIGNAL( timeout() ),
	   (MainForm *)this, SLOT( startOnConsumers() ) );

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


  connect (this, SIGNAL(excludeEvent(const QString&, const QString&)),
	   &fileSet_, SLOT(addExclude(const QString&, const QString&)));	  
  connect (this, SIGNAL(includeEvent(const QString&, const QString&)),
	   &fileSet_, SLOT(delExclude(const QString&, const QString&)));	  

  enableButtons(fileSet_.size() != 0);
}

// TODO: implement exclude from command line
void
MainForm::addExclude(QString const& _eventName)
{
  cout << "add exclude: " << eventMenu_->count() << endl;

  for (unsigned int j = 0; j < eventMenu_->count(); ++j) {
    int domainId = eventMenu_->idAt(j);
    QMenuItem * domainNameItem = eventMenu_->findItem(domainId);
    QPopupMenu * m = domainNameItem->popup();

    if (m != NULL) {
      cout << "popup menu:" << m->name() << endl;
      for (unsigned int i = 0; i < m->count(); ++i) {
	int typeId = m->idAt(i);
	if (m->text(typeId) == _eventName) {
	  cout << "disable event: " << m->text(typeId) << endl;
	  toggleExcludeEvent(domainId, typeId);	  
	}
      }
    }
  }

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
  ACE_Time_Value d = fileSet_.cutEndTime() - fileSet_.cutStartTime();
  timeSlider->setMinValue(0);
  timeSlider->setMaxValue(d.sec() * 100 + d.usec() / 10000);
  setSlider();
}

void
MainForm::setSlider()
{
  ACE_Time_Value d = fileSet_.coursorTime() - fileSet_.cutStartTime();
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
  ACE_Time_Value t = fileSet_.coursorTime() - fileSet_.cutStartTime();

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
  fileSet_.coursorTime(fileSet_.cutStartTime());
}

void MainForm::pause() 
{
  timer_->stop();
}

void
MainForm::next() 
{
  if (!timer_->isActive()) {
    fileSet_.playEvents(fileSet_.coursorTime());
  }
}

void 
MainForm::prev() 
{
  if (!timer_->isActive()) {
    fileSet_.playBackwards();

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
    destTime = std::min(destTime, fileSet_.cutEndTime());

    fileSet_.playEvents(destTime);

    if (fileSet_.coursorTime() < fileSet_.cutEndTime()) {
      // rescedule timer
      ACE_Time_Value nextTime = ACE_OS::gettimeofday() - timeBase_;
      nextTime *= (speed_ > 0)? (double)speed_ : (1. / (double)-speed_);
      nextTime += timeCBase_;

      int interval = (fileSet_.coursorTime() <= nextTime || speed_ == 20)?
	0 :
	(fileSet_.coursorTime() - nextTime).msec();
      timer_->changeInterval(interval);
    }
    else {
      timer_->stop();
      if (exitOnReplayEnd_) {
	emit qApp->quit();
      }
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
MainForm::setStartOnConsumers(int _numConsumers)
{
  numConsumers_ = _numConsumers;
  autoStartTimer_->start(100);
}


void
MainForm::startOnConsumers()
{
  ChannelManager::ChannelMap::const_iterator first, last = fileSet_.channelManager()->channelMap().end();
  int sumConsumers = 0;
  for (first = fileSet_.channelManager()->channelMap().begin(); first != last; ++first) {
    CosNotifyChannelAdmin::AdminIDSeq_var admins =
      first->second->get_all_consumeradmins();
    sumConsumers += admins->length();
  }

  if (sumConsumers >= numConsumers_) {
    play();
    autoStartTimer_->stop();
  }
}

void
MainForm::timeAction(int hsec ) 
{
  ACE_Time_Value t(hsec / 100, (hsec % 100) * 10000);
  fileSet_.coursorTime(fileSet_.cutStartTime() + t);
}

void
MainForm::speed( int _speed) 
{
  speed_ = _speed;
  if (speed_ == 0)
    speed_ = 1;
}

void
MainForm::setSpeed(int _speed) 
{
  speed_ = _speed;
  if (speed_ == 0)
    speed_ = 1;

  speedDial->setValue((speed_ == 1)? 0 : speed_);
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
      fileSet_.coursorTime(fileSet_.cutStartTime());

      connect(file, SIGNAL(notifyEvent(const QString&)), 
	      statusBar(), SLOT(message(const QString&)));
      if (eventView_) {
	connect(this, SIGNAL(excludeEvent(const QString&, const QString&)),
		eventView_, SLOT(excludeEvent(const QString&, const QString&)));	  
	connect(this, SIGNAL(includeEvent(const QString&, const QString&)),
		eventView_, SLOT(includeEvent(const QString&, const QString&)));	  
	connect(file, SIGNAL(newEvent(const QString&,const QString&,const QString&,const QString&)), 
		eventView_, SLOT(insertEvent(const QString&,const QString&,const QString&,const QString&)));
      }
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
  catch (Miro::Exception const& e) {
    QMessageBox::warning(this, "Error parsing file:",
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
    emit includeEvent(domainName, domainNameMenu->text(_eventTypeId));
  }
  // disable event
  else {
    emit excludeEvent(domainName, domainNameMenu->text(_eventTypeId));
  }
}

void
MainForm::toggleEventView() 
{
  if (eventView_ == NULL) {
    eventView_ = new EventView(&fileSet_, history_, "event view");

    toolsMenu_->setItemChecked(eventViewId_, true);

    connect (this, SIGNAL(excludeEvent(const QString&, const QString&)),
	     eventView_, SLOT(excludeEvent(const QString&, const QString&)));	  
    connect (this, SIGNAL(includeEvent(const QString&, const QString&)),
	     eventView_, SLOT(includeEvent(const QString&, const QString&)));	  
    connect(eventView_, SIGNAL(destroyed()),
	    this, SLOT( eventViewClosed()));
  }
  else {
    delete eventView_;
  }
}

void
MainForm::eventViewClosed()
{
  eventView_ = NULL;
  toolsMenu_->setItemChecked(eventViewId_, false);
}

void
MainForm::setHistory()
{
  bool ok = FALSE;
  int res = QInputDialog::getInteger(
                tr( "LogPlayer" ),
                tr( "History Size" ), history_, 1, 10000, 10, &ok, this );
  if ( ok ) {
    history_ = res; // user entered something and pressed OK
    if (eventView_) {
      eventView_->setHistory(history_);
    }
  }
}  

void
MainForm::saveAs()
{
  QString filename;

  bool selected = false;
  while (!selected) {
    // show file dialog
    filename = QFileDialog::getSaveFileName(QString::null, "*.log", this);
	
    if (filename.isEmpty()) {
      break;
    }

    // append file extension if none is given
    QFileInfo file(filename);
    if (file.extension(false).isEmpty()) {
      filename.append(".log");
      file.setFile(filename);
    }

    // warn if file exists
    if (!file.exists()) {
      selected = true;
    } 
    else {
      int button = QMessageBox::warning(this, 
					"LogPlayer", 
					"The file " + filename + " alread exists!\n" +
					"Do you want to overwrite the existing file?",
					"&Yes", "&No", QString::null, 1);
      
      if (button == 0)
	selected = true;
    }
  }

  if (!filename.isEmpty()) {
    fileSet_.saveCut(filename);
  }
}

void
MainForm::cutFront()
{
  fileSet_.cutStart();
}

void 
MainForm::cutBack()
{
  fileSet_.cutEnd();
}

void
MainForm::cutUndo()
{
  fileSet_.cutUndo();
}

void
MainForm::exitOnReplayEnd(bool flag) {
  exitOnReplayEnd_ = flag;
}
