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




