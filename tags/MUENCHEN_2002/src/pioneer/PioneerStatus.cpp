// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "miro/TimeHelper.h"

#include "PioneerStatus.h"

#include "psos/PsosMessage.h"

#include <iostream>
#include <cmath>

#undef DEBUG

// #define VERBOSE

#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif

namespace Pioneer
{
  ACE_Time_Value Status::maxWait;

  std::ostream& 
  operator << (std::ostream& out, const ReportData& report) 
  {
    out << hex
	<< "x = 0x" << report.xPos << endl
	<< "y = 0x" << report.yPos << endl
	<< dec
	<< "heading = " << report.heading << endl;

    return out;
  }

  ReportData::ReportData() :
    xPos(0),
    yPos(0),  
    heading(0)
  {
  }

  void
  ReportData::operator <<= (const Psos::ServerInfoMessage& message)
  {
    time    = message.time();
    xPos    = message.xPos();
    yPos    = message.yPos();
    heading.setRad(message.theta() * 2.0 * M_PI / 1024);
  }

  Status::Status() :
    mutex(),
    cond(mutex),
    currentReport(),
    previousReport(),
    sinHeading(-1.0),
    cosHeading(0.0)
  {
    origin.point.x = 0;
    origin.point.y = 0;
    origin.heading = -M_PI_2;

    // maximum wait time for cond.wait calls
    maxWait = ACE_Time_Value(2, 0);
  }

  void
  Status::operator <<= (const Psos::ServerInfoMessage& message)
  {
    Miro::Guard guard(mutex);

    // save previous report
    previousReport = currentReport;

    // parse new data
    currentReport <<= message;

#ifdef VERBOSE
    cout << "message data: " << endl << currentReport << endl;

    cout << "cosHeading = " << cosHeading << endl
	 << "sinHeading = " << sinHeading << endl
	 << "origin.point.x = " << origin.point.x << endl
	 << "origin.point.y = " << origin.point.y << endl;
#endif

    // set all the data

    status.time.sec = message.time().sec();
    status.time.usec = message.time().usec();

    // get mcp position from status message
    // compute new world position of robot
    double x = ((double)currentReport.xPos) - origin.point.x;
    double y = ((double)currentReport.yPos) - origin.point.y;
  
    // rotate position by heading of origin 
    status.position.point.x = x * cosHeading - y * sinHeading;
    status.position.point.y = x * sinHeading + y * cosHeading;
  
    status.position.heading = currentReport.heading + M_PI_2 + origin.heading;

    ACE_Time_Value dt = currentReport.time - previousReport.time;
    double elapsedTime = ((double) dt.msec()) / 1000.;

    status.velocity.rotation = currentReport.heading - previousReport.heading;
    status.velocity.rotation /= elapsedTime;

    double dx = currentReport.xPos - currentReport.yPos;
    double dy = currentReport.yPos - currentReport.yPos;
    status.velocity.translation = (int)rint(sqrt(dx * dx + dy * dy) / elapsedTime);

    cond.broadcast();
  }

  void
  Status::setWorldPosition(const Miro::PositionIDL& robot) 
  {
    Miro::Guard guard(mutex);
    protectedSetPosition(robot);
  }

  void
  Status::updateWorldPosition(const Miro::PositionIDL& delta) 
  {
    Miro::Guard guard(mutex);

    Miro::PositionIDL robot(status.position);
    robot.point.x += delta.point.x;
    robot.point.y += delta.point.y;
    robot.heading += delta.heading;

    protectedSetPosition(robot);
  }

  void
  Status::protectedSetPosition(const Miro::PositionIDL& robot)
  {
    // calculate heading
    origin.heading = robot.heading - currentReport.heading;
    setTransformation();

    // calculate origin

    // pioneer position
    double xB = (double)currentReport.xPos;
    double yB = (double)currentReport.yPos;

    // commanded position of the robot
    double xR = robot.point.x;
    double yR = robot.point.y;

    // origin matching the current sparrow position to 
    // the requested robot position
    origin.point.x = xB - (xR * cosHeading + yR * sinHeading);
    origin.point.y = yB - (-xR * sinHeading + yR * cosHeading);

    cond.broadcast();
  }
};
