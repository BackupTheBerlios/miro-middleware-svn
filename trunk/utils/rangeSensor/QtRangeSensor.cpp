// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "QtRangeSensor.h"

#include <qapplication.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qmenubar.h>

#include <iostream>
#include <cmath>

namespace 
{
  double deg90 = M_PI_2;
};

RangeSensorWidget::RangeSensorWidget(Miro::RangeSensor_ptr _sensor, CORBA::UShort _group) : 
  QWidget(), 
  sensor_(Miro::RangeSensor::_duplicate(_sensor)),
  group_(_group),
  drawCones_(false)
{
  scanDescription_ = sensor_->getScanDescription();
  scan_ = sensor_->getGroup(group_);

  // set widget size
  resize(400, 400);

  // init menu
  QMenuBar*   menuBar    = new QMenuBar(this);
  QPopupMenu* menuFile = new QPopupMenu(this);
  QPopupMenu* menuView = new QPopupMenu(this);

  menuBar->insertItem("&File", menuFile);
  menuBar->insertItem("&View", menuView);

  menuFile->insertSeparator();
  menuFile->insertItem("Quit", qApp, SLOT(quit()));

  menuView->insertItem("Draw Cone", this, SLOT(toggleCone()));
  
  calcSize();

  // call timer event as often as possible //
  startTimer(0);  
}

RangeSensorWidget::~RangeSensorWidget()
{
}

void
RangeSensorWidget::toggleCone()
{
  drawCones_ = !drawCones_;
}


void RangeSensorWidget::timerEvent(QTimerEvent*) 
{
  scan_ = sensor_->getGroup(group_);
  update();
}



void RangeSensorWidget::paintEvent(QPaintEvent*)
{
  QPainter p;
  p.begin(this);

  // draw new lines
  p.setPen(Qt::red);
  p.setBrush(Qt::blue);

  for (unsigned int j = 0; j < scanDescription_->group[group_].sensor.length(); ++j) {
    
    if (scan_->range[j] >= 0) {

      if (drawCones_) {
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


void RangeSensorWidget::resizeEvent(QResizeEvent*)
{
  calcSize();                         // set Size according to new window size
  update();
}


void RangeSensorWidget::calcSize()
{
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


  cout << "minX: " << minX << endl;
  cout << "maxX: " << maxX << endl;
  cout << "minY: " << minY << endl;
  cout << "maxY: " << maxY << endl;

  cout << "width: " << width() << endl;
  cout << "height: " << height() << endl;

  cout << "x0: " << x0_ << " y0: " << y0_ << endl;
  cout << "scaling: " << scaling_ << endl;

  cout << endl;
}

int main(int argc, char *argv[])
{
  int group = 0;

  // Initialize ORB.
  Miro::Client client(argc, argv);

  // Initialize GUI loop.
  QApplication App(argc, argv);

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <sensor name> <group number>" << std::endl;
    return 1;
  }

  if (argc >= 2) 
    group = atoi(argv[2]);

  // server proxy //
  Miro::RangeSensor_var sensor = client.resolveName<Miro::RangeSensor>(argv[1]);

  RangeSensorWidget sensorWidget(sensor.in(), group);
  
  App.setMainWidget(&sensorWidget);
  sensorWidget.show();

  try {
    // Initialize server daemon.
    App.exec();
    
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }
  return 0;
}
