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
#include "QtRangeSensor.h"

#include <qapplication.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qinputdialog.h>

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

namespace 
{
  const double deg90 = M_PI_2;
};

RangeSensorWidget::RangeSensorWidget(int& argc, char * argv[]) : 
  QWidget(), 
  client_(argc, argv),
  sensor_(),
  robotName_(client_.namingContextName.c_str()),
  sensorName_("Sonar"),
  group_(0),
  timer_(0),
  drawCones_(false)
{
  // set widget size
  resize(400, 400);

  // init menu
  menuBar_  = new QMenuBar(this);
  menuFile_ = new QPopupMenu(this);
  menuView_ = new QPopupMenu(this);

  menuView_->setCheckable(true);

  menuBar_->insertItem("&Robot", menuFile_);
  menuBar_->insertItem("&View", menuView_);

  menuFile_->insertItem("Select robot", this, SLOT(selectRobot()));
  menuFile_->insertItem("Select sensor", this, SLOT(selectSensor()));
  groupIndex_ = menuFile_->insertItem("Select group", this, SLOT(selectGroup()));
  menuFile_->setItemEnabled(groupIndex_, false);

  menuFile_->insertSeparator();
  menuFile_->insertItem("Quit", qApp, SLOT(quit()));

  coneIndex_ = menuView_->insertItem("Draw Cone", this, SLOT(toggleCone()));
}

RangeSensorWidget::~RangeSensorWidget()
{
}

void 
RangeSensorWidget::setRobot(const QString& _robot)
{
  QString error;
  bool ok = true;
  
  // build the lookup string for the naming service
  CosNaming::Name name;
  name.length(1);
  name[0].id = CORBA::string_dup(_robot.latin1());
  
  // try binding the robots naming context
  try {
    CosNaming::NamingContext_var namingContext =
      client_.resolveName<CosNaming::NamingContext>(name);
  }
  catch(const CORBA::Exception& e) {
    std::ostringstream sstr;
    sstr << "Robot " << _robot << endl
	 << "Communication Failed." << endl
	 << "CORBA exception: " << e << flush;
    
    error = sstr.str().c_str();
    ok = false;
  }
  
  if (ok) {
    if (timer_) {
      killTimer(timer_);
      timer_ = 0;
    }
    menuFile_->setItemEnabled(groupIndex_, false);

    robotName_ = _robot;
    sensor_ = Miro::RangeSensor::_nil();
    scan_ = NULL;
    scanDescription_ = NULL;
    setSensor(sensorName_);
  }
  else {
    calcCaption();
    QMessageBox::warning(this, "Couln't set robot:", error);
  }
}

void
RangeSensorWidget::setSensor(const QString& _sensor)
{
  QString error;
  bool ok = true;

  // build the lookup string for the naming service
  CosNaming::Name name;
  name.length(2);
  name[0].id = CORBA::string_dup(robotName_.latin1());
  name[1].id = CORBA::string_dup(_sensor.latin1());

  try {
    sensor_ = client_.resolveName<Miro::RangeSensor>(name);
  }
  catch(const CORBA::Exception& e) {
    std::ostringstream sstr;
    sstr << "Sensor " << robotName_ << "::" << _sensor << endl
	 << "Communication Failed." << endl
	 << "CORBA exception: " << e << flush;
    
    error = sstr.str().c_str();
    ok = false;
  }

  if (ok) {
    sensorName_ = _sensor;
    scanDescription_ = sensor_->getScanDescription();

    menuFile_->setItemEnabled(groupIndex_, true);
    setGroup(0);    
  }
  else {
    calcCaption();
    QMessageBox::warning(this, "Couln't set sensor:", error);
  }
}


void
RangeSensorWidget::setGroup(CORBA::UShort _group)
{
  group_ = _group;
  scan_ = sensor_->getGroup(group_);
  calcSize();
  if (!timer_)
    timer_ = startTimer(40);  
  calcCaption();
}

void
RangeSensorWidget::selectRobot()
{
  bool ok = false;
  QString tmp = QInputDialog::getText(tr( "Select robot" ),
				      tr( "Robot name" ),
				      QLineEdit::Normal, robotName_, &ok, this );
  
  if ( ok && !tmp.isEmpty() ) {
    setRobot(tmp);
  }
}

void
RangeSensorWidget::selectSensor()
{
  bool ok = false;
  QString tmp = QInputDialog::getText(tr( "Select sensor at " ) + robotName_,
				      tr( "Sensor name" ),
				      QLineEdit::Normal, sensorName_, &ok, this );

  if ( ok && !tmp.isEmpty() ) {
    setSensor(tmp);
  }
}

void
RangeSensorWidget::selectGroup()
{
  if (scanDescription_) {
    
    bool ok = false;
    int res = QInputDialog::getInteger( tr( "Select sensor group for ") + sensorName_,
					tr( "Group numer" ), group_, 
					0, scanDescription_->group.length() -1, 1,
					&ok, this );
    if ( ok )
      setGroup(res);
  }
  else
    QMessageBox::warning(this, "Cannot set group:", "No valid sensor selected.");
}

void
RangeSensorWidget::toggleCone()
{
  drawCones_ = !drawCones_;
  menuView_->setItemChecked(coneIndex_, drawCones_);
}


void RangeSensorWidget::timerEvent(QTimerEvent*) 
{
  scan_ = sensor_->getGroup(group_);
  update();
}

void RangeSensorWidget::paintEvent(QPaintEvent*)
{
  if (scan_ && scanDescription_) {

    QPainter p;
    p.begin(this);

    // draw new lines
    p.setPen(Qt::red);
    p.setBrush(Qt::blue);

    for (unsigned int j = 0; j < scanDescription_->group[group_].sensor.length(); ++j) {
    
      if (scan_->range[j] >= 0) {

	if (!drawCones_) {
	  Vector2d a(scanDescription_->group[group_].sensor[j].distance);
	  a *= Vector2d(-sin(scanDescription_->group[group_].sensor[j].alpha),
			cos(scanDescription_->group[group_].sensor[j].alpha));
      
	  Vector2d b(scan_->range[j], 0.);
	  b *= Vector2d(-sin(scanDescription_->group[group_].sensor[j].alpha +
			     scanDescription_->group[group_].sensor[j].beta),
			cos(scanDescription_->group[group_].sensor[j].alpha +
			    scanDescription_->group[group_].sensor[j].beta));
      
	  Vector2d c(a + b);
      
	  p.drawLine(x0_ + (int) rint(a.real() * scaling_), y0_ - (int) rint(a.imag() * scaling_), 
		     x0_ + (int) rint(c.real() * scaling_), y0_ - (int) rint(c.imag() * scaling_));
	}
	else {
	  int delta = (int) rint(scan_->range[j] * scaling_);
	  int angle = (int) rint( ((double) (scanDescription_->group[group_].sensor[j].alpha +
					     scanDescription_->group[group_].sensor[j].beta) *
				   180. / M_PI + 90.) 
				  * 16. );
	  int apex = (int) rint( ( (double) (scanDescription_->group[group_].description.focus) *
				   16. * 180. / M_PI) 
				 / 2.);
			      
      
	  p.drawPie(x0_ - delta, y0_ - delta,
		    2 * delta, 2 * delta,
		    angle - apex, 
		    2 * apex);
	}
      }
    }

    p.end();
  }
}


void RangeSensorWidget::resizeEvent(QResizeEvent*)
{
  calcSize();                         // set Size according to new window size
  update();
}


void RangeSensorWidget::calcSize()
{
  if (! CORBA::is_nil(sensor_.in()) && scanDescription_) {
    double minX = 0.;
    double minY = 0.;
    double maxX = 0.;
    double maxY = 0.;

    for (unsigned int j = 0; j < scanDescription_->group[group_].sensor.length(); ++j) {
    
      Vector2d a(scanDescription_->group[group_].sensor[j].distance);
      a *= Vector2d(-sin(scanDescription_->group[group_].sensor[j].alpha),
		    cos(scanDescription_->group[group_].sensor[j].alpha));
      Vector2d b(scanDescription_->group[group_].description.maxRange, 0.);
      b *= Vector2d(-sin(scanDescription_->group[group_].sensor[j].alpha +
			 scanDescription_->group[group_].sensor[j].beta),
		    cos(scanDescription_->group[group_].sensor[j].alpha +
			scanDescription_->group[group_].sensor[j].beta));
    
      Vector2d p(a + b);
    
      maxX = std::max(maxX, p.real());
      minX = std::min(minX, p.real());
      maxY = std::max(maxY, p.imag());
      minY = std::min(minY, p.imag());
    }
    // set Size to minimum of width and height //
    scaling_ = std::min((double)width() / (maxX - minX),
			(double)height() / (maxY - minY));
    x0_ = (int) rint(-minX * scaling_);
    y0_ = height() - (int) rint(-minY * scaling_);

#ifdef ASDF
    cout << "minX: " << minX << endl;
    cout << "maxX: " << maxX << endl;
    cout << "minY: " << minY << endl;
    cout << "maxY: " << maxY << endl;

    cout << "width: " << width() << endl;
    cout << "height: " << height() << endl;

    cout << "x0: " << x0_ << " y0: " << y0_ << endl;
    cout << "scaling: " << scaling_ << endl;

    cout << endl;
#endif
  }
}

void
RangeSensorWidget::calcCaption()
{
  QString name("RangeSensor " + robotName_ + " - " + sensorName_ + " - (");
  QString group;
  group.setNum(group_);
  setCaption(name + group + ")");
}

int main(int argc, char *argv[])
{
  // Initialize GUI loop.
  QApplication App(argc, argv);

  //   if (argc < 2) {
  //     std::cerr << "Usage: " << argv[0] << " <sensor name> [group number]" << std::endl;
  //     return 1;
  //   }

  RangeSensorWidget sensorWidget(argc, argv);

  App.setMainWidget(&sensorWidget);
  sensorWidget.show();

  try {
    if (argc > 1) {
      sensorWidget.setSensor(argv[1]);
    }
    if (argc > 2) {
      sensorWidget.setGroup(atoi(argv[2]));
    }
    
    App.exec();
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
