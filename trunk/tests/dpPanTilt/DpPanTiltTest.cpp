// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
	limits = pantilt->getLimits();
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
    cout << "minpanposition  [rad]:" << limits.minpanposition << endl;
    cout << "maxpanposition  [rad]:" << limits.maxpanposition << endl;
    cout << "mintiltposition [rad]:" << limits.mintiltposition << endl;
    cout << "maxtiltposition [rad]:" << limits.maxtiltposition << endl;
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
	limits = pantilt->getLimits();
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
    cout << "minpanposition  [rad/s]:" << limits.minpanposition << endl;
    cout << "maxpanposition  [rad/s]:" << limits.maxpanposition << endl;
    cout << "mintiltposition [rad/s]:" << limits.mintiltposition << endl;
    cout << "maxtiltposition [rad/s]:" << limits.maxtiltposition << endl;

    cout << " Powers :" << endl;
    cout << "panhold         :" << powers.panhold << endl;
    cout << "tilthold        :" << powers.tilthold << endl;
    cout << "panmove         :" << powers.panmove << endl;
    cout << "tiltmove        :" << powers.tiltmove << endl;

    cout << endl << "press char + return" << endl <<endl;
    cin >> c;

    PanTiltPositionIDL ptdata;

    ptdata.panvalue = 4.95;
    ptdata.tiltvalue = 0.45;
    try {
      try {
	cout << "destination was 4.95/0.45 (which should produce an error)" << endl;
	pantilt->setWaitPosition( ptdata );
	ptdata = pantilt->getPosition( );
	cout << "reached :" << ptdata.panvalue << "," << ptdata.tiltvalue << endl;
      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }

    ptdata.panvalue = -1.35;
    ptdata.tiltvalue = -0.45;
    try {
      try {
	cout << "destination was -1.35,-0.45" << endl;
	pantilt->setWaitPosition( ptdata );
	ptdata = pantilt->getPosition(  );
	cout << "reached :" << ptdata.panvalue << "," << ptdata.tiltvalue << endl;
      } catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << endl;
      }
    } catch (const EDevIO& e) {
      cerr << "Device IO exception :" << e << endl;
    }

    ptdata.panvalue = 0;
    ptdata.tiltvalue = 0;

    try {
      try {
	cout << "destination was 0,0" << endl;
	pantilt->setPosition( ptdata );
	ptdata = pantilt->getPosition(  );
	cout << "reached :" << ptdata.panvalue << "," << ptdata.tiltvalue << endl;

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




