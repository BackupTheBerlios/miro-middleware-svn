// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/OdometryC.h"
#include "idl/B21MotionC.h"
#include "miro/Client.h"
#include "miro/Task.h"
#include "miro/IO.h"
#include "miro/Synch.h"

#include <iostream>
#include <cmath>

using namespace Miro;


class MyTask : public Task
{
  int num;
  Odometry_var pOdometry;

public:

  MyTask(int _num, Odometry_ptr _pOdometry) : 
    num(_num),
    pOdometry(Odometry::_duplicate(_pOdometry))
    {}

  int open(void *) {
    activate(THR_NEW_LWP, num);
    return 0;
  }

  int svc () {
    // Initialize server daemon.
    MotionStatusIDL data;

    cout << "Thread started." << endl;

    try {
      for (int i = 0; i < 120; ++i) {
	ACE_OS::sleep(1);
	data = pOdometry->getStatus();
	cout << "MotionStatus: " << data << endl;
      }
    }
    catch (const Miro::ETimeOut & e) {
      cerr << "Time out exception on server." << endl;
      return 1;
    }
    catch (const Miro::EOutOfBounds & e) {
      cerr << "Out of bounds exception on server." << endl;
      return 1;
    }
    catch (const Miro::EDevIO & e) {
      cerr << "Device IO exception on server." << endl;
      return 1;
    }
    catch (const CORBA::Exception & e) {
      cerr << "Uncaught CORBA exception: " << e << endl;
      return 1;
    }
    catch (...) {
      cerr << "Uncaught exception: " << endl;
      return 1;
    }
    cout << "Thread exiting." << endl;

    return 0;
  }
};


int main(int argc, char *argv[])
{
  int drive = 1;
  int num = 1;

  if (argc > 1) {
    drive = atoi(argv[1]);
    cout << "drive: " << drive << endl;
  }
  if (argc > 2) {
    num = atoi(argv[2]);
    cout << "Starting " << num << " threads." << endl;
  }

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    B21Motion_var motion = client.resolveName<B21Motion>("Motion");
    Odometry_var odometry = client.resolveName<Odometry>("Odometry");

    MyTask task(num, odometry.in());
    task.open(NULL);

    if (drive) {
      cout << "Testing Defaults:" << endl;

      ACE_OS::sleep(1);

      cout << "set rotation speed to PI/6" << endl;
      motion->setRotateVelocity(M_PI/6);
      cout << "waiting for motion to stop" << endl;
      motion->waitRotateHalt();
      cout << "rotate left 90°" << endl;
      motion->rotateRelative(M_PI/2);
      cout << "waiting for base to stop" << endl;
      motion->waitRotateHalt();

      cout << "rotate right continuous" << endl;
      motion->rotateRight();
      ACE_OS::sleep(3);
      cout << "limp" << endl;
      motion->rotateLimp();

      cout << "rotate right 90°" << endl;
      motion->rotateRelative(-M_PI/2);
      cout << "waiting for base to stop" << endl;
      motion->waitRotateHalt();

      cout << "rotate left continuous" << endl;
      motion->rotateLeft();
      ACE_OS::sleep(3);
      cout << "limp" << endl;
      motion->rotateLimp();

      cout << "set translation speed to 250 mm/s" << endl;
      motion->setTranslateVelocity(250);
      cout << "waiting for base to stop" << endl;
      motion->waitTranslateHalt();
      cout << "translate forward 500mm" << endl;
      motion->translateRelative(500);
      cout << "waiting for base to stop" << endl;
      motion->waitTranslateHalt();

      cout << "translate backward continuous" << endl;
      motion->translateBackward();
      ACE_OS::sleep(2);
      cout << "limp" << endl;
      motion->translateLimp();

      cout << "translate backwar 500mm" << endl;
      motion->translateRelative(-500);
      cout << "waiting for base to stop" << endl;
      motion->waitTranslateHalt();

      cout << "translate forward continuous" << endl;
      motion->translateForward();
      ACE_OS::sleep(2);
      cout << "limp" << endl;
      motion->translateLimp();

      cout << "rotateCurrent: " << flush;
      cout << motion->getWaitRotateCurrent() << endl;
      cout << "translateCurrent: " << flush;
      cout << motion->getWaitTranslateCurrent() << endl;
      cout << "batteryVoltage: " << flush;
      cout << motion->getWaitBatteryVoltage() << endl;

#ifdef saf
      cout << "Default rotational speed" << endl;
      motion->rotateRelative(2 * M_PI);
      motion->waitRotateHalt();

      cout << "Default translational speed" << endl;
      motion->translateRelative(500);
      motion->waitTranslateHalt();
#endif
    }
    ACE_Thread_Manager::instance()->wait();

  }
  catch (const Miro::ETimeOut & e) {
    cerr << "Time out exception on server." << endl;
    return 1;
  }
  catch (const Miro::EOutOfBounds & e) {
    cerr << "Out of bounds exception on server." << endl;
    return 1;
  }
  catch (const Miro::EDevIO & e) {
    cerr << "Device IO exception on server." << endl;
    return 1;
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




