// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// $Date$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef LogPlayer_hh
#define LogPlayer_hh

#include "miro/StructuredPushSupplier.h"

#include <ace/Mem_Map.h>
#include <ace/Time_Value.h>

#include <qapplication.h>
#include <qpushbutton.h>
#include <qfont.h>
#include <qslider.h>
#include <qlcdnumber.h>
#include <qmenubar.h>
#include <qtimer.h>
#include <qdial.h>

#include <vector>
#include <string>

class MyWidget : public QWidget
{
  Q_OBJECT
public:
  MyWidget( CosNotifyChannelAdmin::EventChannel_ptr _ec, 
	    const std::string& _context,
	    QWidget *parent=0, const char *name = 0 );

  void loadFile( const char * const);
  void send();
  void calcStartTime();

  void addExclude(const std::string& eventName);

public slots:
  void pause();
  void play();
  void stop();
  void next();
  void prev();
  void load();
  void changed( int );
  void step();
  void beginAction();
  void endAction();
  void timeAction( int );
  void speed( int );

public:
  QMenuBar *menuBar;
  Miro::StructuredPushSupplier supplier;

  QPushButton *playButton;
  QPushButton *stopButton;
  QPushButton *pauseButton;
  QPushButton *nextButton;
  QPushButton *prevButton;
  QSlider     *timeSlider;
  QLCDNumber  *centiSecLabel;
  QLCDNumber  *secLabel;
  QDial       *speedDial;

  QTimer      timer;

  ACE_Time_Value startTime;

  typedef std::pair< ACE_Time_Value, char* > TimePair;
  typedef std::vector< TimePair > TimeVector;
  typedef std::vector< std::string > StringVector;
  TimeVector timeVector;
  unsigned int timeVectorPos;
  int timeSpeed;
  bool action;
  ACE_Mem_Map memoryMap;
  std::string context;

  StringVector exclude;
};

#endif
