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
#include "miro/RangeSensorC.h"

#include <qwidget.h>

#include <complex>

/** RangeSensorWidget */
class RangeSensorWidget : public QWidget
{
  Q_OBJECT

  typedef QWidget Super;
  typedef RangeSensorWidget Self;

  typedef std::complex<double> Vector2d;
  
public:
  /** Constructor */
  RangeSensorWidget(Miro::RangeSensor_ptr _sensor, CORBA::UShort _group);

  /** Destructor */
  virtual ~RangeSensorWidget();

public slots:
  void toggleCone();

protected:
  void timerEvent(QTimerEvent*);
  void paintEvent(QPaintEvent*);
  void resizeEvent(QResizeEvent*);
  void calcSize();

  Miro::RangeSensor_var sensor_;
  Miro::ScanDescriptionIDL_var scanDescription_;
  CORBA::UShort group_;
  Miro::RangeGroupEventIDL_var scan_;

  int x0_;
  int y0_;

  double scaling_;
  bool drawCones_;
};

#endif




