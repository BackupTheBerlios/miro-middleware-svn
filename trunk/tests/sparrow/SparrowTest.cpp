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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#include "sparrow/SparrowConnection.h"
#include "sparrow/Parameters.h"
#include "sparrow/SparrowConsumer.h"
 
#include "can/CanEventHandler.h"

#include "miro/ReactorTask.h"
#include "miro/IO.h"
#include "miro/Utils.h"

#include "miro/TimeHelper.h"
#include "miro/Angle.h"

#include <iostream>

using namespace Miro;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

double normalizeAngle(double alpha)
{
  while (alpha > M_PI)
    alpha -= 2 * M_PI;
  while (alpha < -M_PI)
    alpha += 2 * M_PI;

  return alpha;
}

struct Service
{
  ReactorTask reactorTask;
  Miro::OdometryImpl * odometryImpl;
  Sparrow::Consumer * pSparrowConsumer;
  Can::EventHandler * pCanEventHandler;
  Sparrow::Connection connection;

  Service();
};

Service::Service() :
  reactorTask(),
  odometryImpl(new Miro::OdometryImpl(NULL)),
  pSparrowConsumer(new Sparrow::Consumer(&connection, odometryImpl, NULL, NULL, NULL)),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer, Sparrow::Parameters::instance())),
  connection(reactorTask.reactor(), pCanEventHandler, pSparrowConsumer)
{

}

int main(int argc, char * argv[])
{
  // Parameters to be passed to the services
  Sparrow::Parameters * params = Sparrow::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
  config->setSection("Sparrow99");
  config->getParameters("SparrowBoard", *params);
  delete config;

#ifdef DEBUG
    cout << "  sparrow paramters:" << endl <<* params << endl;
#endif

  // Initialize server daemon.
  Service service;

  cout << "initialized" << endl;

  ACE_Time_Value ace_time;
  unsigned short accel, power, speed, 
    threshold, a2, a1, a0, ticksL, ticksR,
    radstand,
    num;
  short left, right, trans, distance;
  int time, delta;
  char c;
  double rot;
  bool loop = true;
  
  double angle, beta;
  Miro::Angle alpha, gamma;

  Miro::PositionIDL start, current;

  service.reactorTask.open(NULL);

  ACE_OS::sleep(ACE_Time_Value(2));

  try {
    while(loop) {

      cout << endl
	   << "Befehlsauswahl:" << endl
	   << "  0 - alive" << endl
	   << "  1 - initMax" << endl
	   << "  2 - initStall" << endl
	   << "  3 - initDrive" << endl
	   << "  4 - setPower" << endl
	   << "  5 - setSpeed" << endl
	   << "  6 - setSpeedRot" << endl
	   << "  7 - kick" << endl
	   << "  8 - drive distance" << endl
	   << "  9 - turn in place" << endl
	   << "  p - get position" << endl
	   << "  d - status" << endl
	   << "  s - set servo" << endl
	   << "q - quit" << endl;
      cin >> c;
      cout << endl;

      switch (c) {
      case '0':      // alive
	cout << "motorAlive: " << service.connection.motorAlive() << endl;
	cout << "odoAlive: " << service.connection.odoAlive() << endl;
	cout << "portsAlive: " << service.connection.portsAlive() << endl;
	cout << "stallAlive: " << service.connection.stallAlive() << endl;
	cout << "servoAlive: " << service.connection.servoAlive() << endl;
	cout << "kickerAlive: " << service.connection.kickerAlive() << endl;
	cout << "irAlive: " << service.connection.infraredAlive() << endl;
	break;
      case '1':      // initMax
	cout << "macAcc (1500): " << flush;
	cin >> accel;
	cout << "maxPower (30720): " << flush;
	cin >> power;
	cout << "maxSpeed (1000): " << flush;
	cin >> speed;
	cout << "maxRotation (180°): " << flush;
	cin >> rot;
	rot = deg2Rad(rot);;
	service.connection.initMax(accel, power, speed, rot);
	break;
      case '2':      // initStall
	cout << "threshold 14: " << flush;
	cin >> threshold;
	cout << "a2 (1): " << flush;
	cin >> a2;
	cout << "a1 (100): " << flush;
	cin >> a1;
	cout << "a0 (5000): " << flush;
	cin >> a0;
	service.connection.initStall(threshold, a2, a1, a0);
	break;
      case '3':      // initDrive
	cout << "ticksL per mm (29): " << flush;
	cin >> ticksL;
	cout << "ticksR per mm (29): " << flush;
	cin >> ticksR;
	cout << "radstand(mm)* 2PI (2419): " << flush;
	cin >> radstand;
	service.connection.initDrive(ticksL, ticksR, radstand);
	break;
      case '4':      // setPower
	cout << "left power (>5000, <30720): " << flush;
	cin >> left;
	cout << "right power (>5000, <30720): " << flush;
	cin >> right;
	cout << "time (msec): " << flush;
	cin >> time;
	ace_time.msec(time);
	service.connection.setPower(left, right);
	ACE_OS::sleep(ace_time);
	service.connection.setPower(0, 0);
	break;
      case '5':      // setSpeed
	cout << "left speed (mm/s): " << flush;
	cin >> left;
	cout << "right speed (mm/s): " << flush;
	cin >> right;
	cout << "time (msec): " << flush;
	cin >> time;
	ace_time.msec(time);
	service.connection.setSpeed(left, right);
	ACE_OS::sleep(ace_time);
	service.connection.setSpeed(0, 0);
	break;
      case '6':      // setSpeedRot
	cout << "translational speed (mm/s): " << flush;
	cin >> trans;
	cout << "rotational speed (deg/s): " << flush;
	cin >> rot;
	rot = deg2Rad(rot);
	cout << "time (msec): " << flush;
	cin >> time;
	ace_time.msec(time);
	service.connection.setSpeedRot(trans, Sparrow::rad2ticks(rot));
	ACE_OS::sleep(ace_time);
	service.connection.setSpeedRot(0, 0);
	break;
      case '7':      // kick
	cout << "time (msec): " << flush;
	cin >> time;
	service.connection.kick(time);
	break;
      case '8':  // drive distance
	cout << "distance (mm): " << flush;
	cin >> distance;
	cout << "speed (mm/s): " << flush;
	cin >> trans;

	start = service.odometryImpl->getPosition();
	if (distance >= 0) {
	  service.connection.setSpeed(trans, trans);
	}
	else {
	  service.connection.setSpeed(-trans, -trans);
	  distance = -distance;
	}

	ace_time = ACE_OS::gettimeofday();
	do {
	  current = service.odometryImpl->getWaitPosition();
	  delta = (int) sqrt( (double)(current.point.x - start.point.x) * 
			      (double)(current.point.x - start.point.x) +
			      (double)(current.point.y - start.point.y) * 
			      (double)(current.point.y - start.point.y) );

	  // Notaus!
	  if ( (ACE_OS::gettimeofday() - ace_time) > ACE_Time_Value(5)) {
	    cout << "safety timout" << endl;
	    break;
	  }

	} while (delta < distance);

	service.connection.setSpeed(0, 0);
	
	ACE_OS::sleep(ACE_Time_Value(2));
		      
	current = service.odometryImpl->getWaitPosition();
	delta = (int) sqrt( (double)(current.point.x - start.point.x) * 
			    (double)(current.point.x - start.point.x) +
			    (double)(current.point.y - start.point.y) * 
			    (double)(current.point.y - start.point.y) );
	cout << "driven distance: " << delta << endl;
	break;
      case '9': // turn angle
	cout << "Anlge (deg): " << flush;
	cin >> angle;
	angle = deg2Rad(angle);
	cout << "Speed (mm/s): " << flush;
	cin >> trans;

	alpha = service.odometryImpl->getWaitPosition().heading;

	ace_time = ACE_OS::gettimeofday();

	if (angle > 0.)
	  service.connection.setSpeed(-trans, trans);
	else
	  service.connection.setSpeed(trans, -trans);

	beta = 0.;
	do {
	  gamma = alpha;
	  alpha = service.odometryImpl->getWaitPosition().heading;

	  beta += alpha - gamma;

	  // Notaus!
	  if ( (ACE_OS::gettimeofday() - ace_time) > ACE_Time_Value(5)) {
	    cout << "safety timout" << endl;
	    break;
	  }

	} while (fabs(angle) > fabs(beta));


	service.connection.setSpeed(0, 0);
	
	ACE_OS::sleep(ACE_Time_Value(2));
	
	gamma = alpha;
	alpha = service.odometryImpl->getWaitPosition().heading;
	beta += alpha - gamma;
	
	cout << "turned angle: " << rad2Deg(beta) << "°" <<endl;
	break;
      case 'p':
	cout << "current position is: " 
	     << service.odometryImpl->getPosition() << endl;
	break;
      case 'd':
	cout << "Can Status: " << endl;
	service.connection.status();

	ACE_OS::sleep(ACE_Time_Value(1));
	break;
      case 's':      // setSpeedRot
	cout << "servo number (0-1): " << flush;
	cin >> num;
	cout << "servo position (deg): " << flush;
	cin >> rot;
	rot = deg2Rad(rot);
	service.connection.setServo(num, rot);
	break;
      case 'q':
	loop = false;
	break;
      default:
	cout << "unknown command: " << c << endl;
      }
    }
  }
  catch (const Miro::Exception & e) {
    cerr << "Miro exception thrown:" << e << endl;
    return 1;
  }

  service.reactorTask.cancel();
  return 0;
}

