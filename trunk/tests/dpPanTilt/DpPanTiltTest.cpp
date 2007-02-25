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
#include "idl/DirectedPerceptionPanTiltC.h"
#include "miro/Client.h"

#include <iostream>

using namespace Miro;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
    // Reference to the server object
    DirectedPerceptionPanTilt_var 
      pantilt = client.resolveName<DirectedPerceptionPanTilt>("PanTilt");

    PanTiltSpdAccIDL spdacc, spdacc2;
    PanTiltLimitsIDL limits;
    PanTiltPowersIDL powers, powers2;

    try {
      try {
	spdacc = pantilt->getSpdAcc();
	limits = pantilt->getPanTiltLimits();
	powers = pantilt->getPowers();

      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }

    cout << endl << endl << "original settings...." << endl;
    cout << "Speed and Accelerations :" << endl;
    cout << "targetpanspeed  [rad/s]:" << spdacc.targetpanspeed << endl;
    cout << "targettiltspeed [rad/s]:" << spdacc.targettiltspeed << endl;
    cout << "panbasespeed    [rad/s]:" << spdacc.panbasespeed << endl;
    cout << "panminspeed     [rad/s]:" << spdacc.panminspeed << endl;
    cout << "panmaxspeed     [rad/s]:" << spdacc.panmaxspeed << endl;
    cout << "tiltbasespeed   [rad/s]:" << spdacc.tiltbasespeed << endl;
    cout << "tiltminspeed    [rad/s]:" << spdacc.tiltminspeed << endl;
    cout << "tiltmaxspeed    [rad/s]:" << spdacc.tiltmaxspeed << endl;
    cout << "panacc          [rad/(s^2)]:" << spdacc.panacc << endl;
    cout << "tiltacc         [rad/(s^2)]:" << spdacc.tiltacc << endl;
    cout << "Limits :" << endl;
    cout << "pan.minAngle  [rad]:" << limits.pan.minAngle << endl;
    cout << "pan.maxAngle  [rad]:" << limits.pan.maxAngle << endl;
    cout << "tilt.minAngle [rad]:" << limits.tilt.minAngle << endl;
    cout << "tilt.maxAngle [rad]:" << limits.tilt.maxAngle << endl;
    cout << " Powers :" << endl;
    cout << "panhold         :" << powers.panhold << endl;
    cout << "tilthold        :" << powers.tilthold << endl;
    cout << "panmove         :" << powers.panmove << endl;
    cout << "tiltmove        :" << powers.tiltmove << endl;

    spdacc2 = spdacc;
    powers2 = powers;

    spdacc2.targetpanspeed += 0.050;
    spdacc2.targettiltspeed += 0.050;
    spdacc2.panbasespeed += 0.050;
    spdacc2.panminspeed += 0.050;
    spdacc2.panmaxspeed -= 0.050;
    spdacc2.tiltbasespeed += 0.050;
    spdacc2.tiltminspeed += 0.050;
    spdacc2.tiltmaxspeed += 0.050;
    spdacc2.panacc += 0.050;
    spdacc2.tiltacc += 0.050;

    powers2.panhold  = 0;
    powers2.tilthold  = 0;
    powers2.panmove = 0;
    powers2.tiltmove = 0;

    cout << endl << endl << "what we want to set...." << endl;
    cout << "Speed and Accelerations :" << endl;
    cout << "targetpanspeed  [rad/s]:" << spdacc2.targetpanspeed << endl;
    cout << "targettiltspeed [rad/s]:" << spdacc2.targettiltspeed << endl;
    cout << "panbasespeed    [rad/s]:" << spdacc2.panbasespeed << endl;
    cout << "panminspeed     [rad/s]:" << spdacc2.panminspeed << endl;
    cout << "panmaxspeed     [rad/s]:" << spdacc2.panmaxspeed << endl;
    cout << "tiltbasespeed   [rad/s]:" << spdacc2.tiltbasespeed << endl;
    cout << "tiltminspeed    [rad/s]:" << spdacc2.tiltminspeed << endl;
    cout << "tiltmaxspeed    [rad/s]:" << spdacc2.tiltmaxspeed << endl;
    cout << "panacc          [rad/(s^2)]:" << spdacc2.panacc << endl;
    cout << "tiltacc         [rad/(s^2)]:" << spdacc2.tiltacc << endl;
    cout << " Powers :" << endl;
    cout << "panhold         :" << powers2.panhold << endl;
    cout << "tilthold        :" << powers2.tilthold << endl;
    cout << "panmove         :" << powers2.panmove << endl;
    cout << "tiltmove        :" << powers2.tiltmove << endl;

    char c;
    cout << endl << "press char + return" << endl <<endl;
    cin >> c;

    try {
      try {
	pantilt->setSpdAcc( spdacc2 );
	pantilt->setPowers( powers2 );
      
      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }

    cout << endl << "press char + return" << endl <<endl;
    cin >> c;

    try {
      try {
	spdacc2 = pantilt->getSpdAcc();
	powers2 = pantilt->getPowers();

      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }

    cout << endl << endl << "our settings...." << endl;
    cout << "Speed and Accelerations :" << endl;
    cout << "targetpanspeed  [rad/s]:" << spdacc2.targetpanspeed << endl;
    cout << "targettiltspeed [rad/s]:" << spdacc2.targettiltspeed << endl;
    cout << "panbasespeed    [rad/s]:" << spdacc2.panbasespeed << endl;
    cout << "panminspeed     [rad/s]:" << spdacc2.panminspeed << endl;
    cout << "panmaxspeed     [rad/s]:" << spdacc2.panmaxspeed << endl;
    cout << "tiltbasespeed   [rad/s]:" << spdacc2.tiltbasespeed << endl;
    cout << "tiltminspeed    [rad/s]:" << spdacc2.tiltminspeed << endl;
    cout << "tiltmaxspeed    [rad/s]:" << spdacc2.tiltmaxspeed << endl;
    cout << "panacc          [rad/(s^2)]:" << spdacc2.panacc << endl;
    cout << "tiltacc         [rad/(s^2)]:" << spdacc2.tiltacc << endl;
    cout << " Powers :" << endl;
    cout << "panhold         :" << powers2.panhold << endl;
    cout << "tilthold        :" << powers2.tilthold << endl;
    cout << "panmove         :" << powers2.panmove << endl;
    cout << "tiltmove        :" << powers2.tiltmove << endl;

    cout << endl << "press char + return" << endl <<endl;
    cin >> c;

    try {
      try {
	// reset original settings 
	pantilt->setSpdAcc( spdacc );
	pantilt->setPowers( powers );
      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }

    cout << endl << "press char + return" << endl <<endl;
    cin >> c;

    try {
      try {
	// check our (re)settings
	spdacc = pantilt->getSpdAcc();
	limits = pantilt->getPanTiltLimits();
	powers = pantilt->getPowers();

      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }

    cout << endl << endl << "restored original settings...." << endl;

    cout << "Speed and Accelerations :" << endl;
    cout << "targetpanspeed  [rad/s]:" << spdacc.targetpanspeed << endl;
    cout << "targettiltspeed [rad/s]:" << spdacc.targettiltspeed << endl;
    cout << "panbasespeed    [rad/s]:" << spdacc.panbasespeed << endl;
    cout << "panminspeed     [rad/s]:" << spdacc.panminspeed << endl;
    cout << "panmaxspeed     [rad/s]:" << spdacc.panmaxspeed << endl;
    cout << "tiltbasespeed   [rad/s]:" << spdacc.tiltbasespeed << endl;
    cout << "tiltminspeed    [rad/s]:" << spdacc.tiltminspeed << endl;
    cout << "tiltmaxspeed    [rad/s]:" << spdacc.tiltmaxspeed << endl;
    cout << "panacc          [rad/(s^2)]:" << spdacc.panacc << endl;
    cout << "tiltacc         [rad/(s^2)]:" << spdacc.tiltacc << endl;

    cout << "Limits :" << endl;
    cout << "pan.minAngle  [rad/s]:" << limits.pan.minAngle << endl;
    cout << "pan.maxAngle  [rad/s]:" << limits.pan.maxAngle << endl;
    cout << "tilt.minAngle [rad/s]:" << limits.tilt.minAngle << endl;
    cout << "tilt.maxAngle [rad/s]:" << limits.tilt.maxAngle << endl;

    cout << " Powers :" << endl;
    cout << "panhold         :" << powers.panhold << endl;
    cout << "tilthold        :" << powers.tilthold << endl;
    cout << "panmove         :" << powers.panmove << endl;
    cout << "tiltmove        :" << powers.tiltmove << endl;

    cout << endl << "press char + return" << endl <<endl;
    cin >> c;

    PanTiltPositionIDL ptdata;

    ptdata.panValue = 4.95;
    ptdata.tiltValue = 0.45;
    try {
      try {
	cout << "destination was 4.95/0.45 (which should produce an error)" << endl;
	pantilt->setWaitPosition( ptdata );
	ptdata = pantilt->getPosition( );
	cout << "reached :" << ptdata.panValue << "," << ptdata.tiltValue << endl;
      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }

    ptdata.panValue = -1.35;
    ptdata.tiltValue = -0.45;
    try {
      try {
	cout << "destination was -1.35,-0.45" << endl;
	pantilt->setWaitPosition( ptdata );
	ptdata = pantilt->getPosition(  );
	cout << "reached :" << ptdata.panValue << "," << ptdata.tiltValue << endl;
      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }

    ptdata.panValue = 0;
    ptdata.tiltValue = 0;

    try {
      try {
	cout << "destination was 0,0" << endl;
	pantilt->setPosition( ptdata );
	ptdata = pantilt->getPosition(  );
	cout << "reached :" << ptdata.panValue << "," << ptdata.tiltValue << endl;

      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }
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




