// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef QtRangeSensor_h
#define QtRangeSensor_h

#include "miro/Client.h"
#include "idl/RangeSensorC.h"

#include <qwidget.h>
#include <qstring.h>

#include <complex>

// forward declarations
class QMenuBar;
class QPopupMenu;

//! RangeSensorWidget
class RangeSensorWidget : public QWidget
{
  Q_OBJECT

  typedef QWidget Super;
  typedef RangeSensorWidget Self;

  typedef std::complex<double> Vector2d;
  
public:
  //! Constructor
  RangeSensorWidget(int& argc, char * argv[]);

  //! Destructor
  virtual ~RangeSensorWidget();

  void setRobot(const QString& _robot);
  void setSensor(const QString& _sensor);
  void setGroup(CORBA::UShort _group);

public slots:
  void selectRobot();
  void selectSensor();
  void selectGroup();
  void toggleCone();

protected:
  void timerEvent(QTimerEvent*);
  void paintEvent(QPaintEvent*);
  void resizeEvent(QResizeEvent*);
  void calcSize();
  void calcCaption();

  Miro::Client client_;
  CosNaming::NamingContext_var robot_;
  Miro::RangeSensor_var sensor_;
  Miro::ScanDescriptionIDL_var scanDescription_;
  Miro::RangeGroupEventIDL_var scan_;

  QString robotName_;
  QString sensorName_;
  CORBA::UShort group_;

  int x0_;
  int y0_;

  double scaling_;
  int timer_;
  bool drawCones_;

  QMenuBar*   menuBar_;
  QPopupMenu* menuFile_;
  QPopupMenu* menuView_;

  int groupIndex_;
  int coneIndex_;
};

#endif




