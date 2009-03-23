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
#include "miro/Client.h"
#include "idl/OdometryC.h"
#include "idl/MotionC.h"
#include "idl/RangeSensorC.h"
#include "idl/ButtonsC.h"
#include "idl/DtlkSpeechC.h"

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <wait.h>
#include <string>

#include "miro/Ssh.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

typedef struct ServicesStatus {
  bool stanright; //can login?
  bool stanleft;  //can login?
  bool variablesSet; //MIRO_ROOT, ACE_ROOT, TAO_ROOT, NameServiceIOR
  bool odometry;     //registered at the Name Service
  bool motion;       //registered at the Name Service
  bool sonar;        //registered at the Name Service
  bool infrared;     //registered at the Name Service
  bool tactile;      //registered at the Name Service
  bool eventChannel; //registered at the Name Service
  bool buttons;      //registered at the Name Service
  bool laser;        //registered at the Name Service
  bool speech;       //registered at the Name Service
  bool video;        //registered at the Name Service
  bool b21BaseRunnable; //can be started (and keeps up for at least 5 secs)
  bool b21BaseRunning;  //is already running
  bool laserRunnable;   //can be started (and keeps up for at least 5 secs)
  bool laserRunning;     //is already running
  bool speechRunnable;  //can be started (and keeps up for at least 5 secs)
  bool speechRunning;   //is already running
  bool videoRunnable;   //can be started (and keeps up for at least 5 secs)
  bool videoRunning;    //is already running
};






void doAnalysis(struct ServicesStatus& services)
{
  cout << endl << "--------------------------------------------------" << endl;
  
  cout << "Test results:" << endl << endl;


  /////////////////
  // Stanleft
  /////////////////

  if (services.stanleft) 
  {
    cout << "stanleft is responding" << endl << endl;
    if (!services.variablesSet)
    {
      cout << "The environement variables required for the robot to run are not properly set" << endl;
      if (services.odometry || services.motion || services.buttons || services.eventChannel || services.infrared || services.tactile || services.sonar || services.laser || services.video || services.speech)
	cout << "Although there are some services registered at the Naming Service, they might not work properly without those variables" << endl;
    } else {
      cout << "The required environement variables are properly set on stanleft" << endl;
    } 

    //odometry
    if (services.odometry)
    {
      if (services.b21BaseRunning) {
	cout << "The odometry server is running and registered at the Naming Service" << endl;
	cout << "You can acces the robot's odometry" << endl;
      } else {
	cout << "The odometry server is registered at the Naming service, but the server is not running." << endl;
	cout << "This is usually due to a crash in the base server." << endl;
	cout << "You should restart the base server with the -MiroRebindIOR argument" << endl;
      }
    } else {
      cout << "The odometry server is not registered at the Naming Service" << endl;
      cout << "You will not be able to get any odometry readings" << endl;
      if (services.odometry || services.motion || services.buttons || services.eventChannel || services.infrared || services.tactile || services.sonar)
	cout << "There are other base services registered. This indicates a problem in the base module or the Naming Service" << endl;
      if (services.b21BaseRunning) {
	cout << "The base server is running but not registered at the Naming Service" << endl;
	cout << "You should restart it. If the problem persists, it is a problem with the Naming Service" << endl;
      } else if (services.b21BaseRunnable) {
	cout << "The base server is not running, but should be startable without problems" << endl;
      } else {
	cout << "The base server is not running and cannot be started" << endl;
	cout << "It might be a bug in the software" << endl;
	cout << "It must be started at stanleft" << endl;
      }
    }

    //motion
    if (services.motion)
    {
      if (services.b21BaseRunning) {
	cout << "The motion server is running and registered at the Naming Service" << endl;
	cout << "You can make the robot move" << endl;
      } else {
	cout << "The motion server is registered at the Naming service, but the server is not running." << endl;
	cout << "This is usually due to a crash in the base server." << endl;
	cout << "You should restart the base server with the -MiroRebindIOR argument" << endl;
      }
    } else {
      cout << "The motion server is not registered at the Naming Service" << endl;
      cout << "You will not be able to make the robot move" << endl;
      if (services.odometry || services.motion || services.buttons || services.eventChannel || services.infrared || services.tactile || services.sonar)
	cout << "There are other base services registered. This indicates a problem in the base module or the Naming Service" << endl;
      if (services.b21BaseRunning) {
	cout << "The base server is running but not registered at the Naming Service" << endl;
	cout << "You should restart it. If the problem persists, it is a problem with the Naming Service" << endl;
      } else if (services.b21BaseRunnable) {
	cout << "The base server is not running, but should be startable without problems" << endl;
      } else {
	cout << "The base server is not running and cannot be started" << endl;
	cout << "It might be a bug in the software" << endl;
	cout << "It must be started at stanleft" << endl;
      }
    }


    //sonar
    if (services.sonar)
    {
      if (services.b21BaseRunning) {
	cout << "The sonar server is running and registered at the Naming Service" << endl;
      } else {
	cout << "The sonar server is registered at the Naming service, but the server is not running." << endl;
	cout << "This is usually due to a crash in the base server." << endl;
	cout << "You should restart the base server with the -MiroRebindIOR argument" << endl;
      }
    } else {
      cout << "The sonar server is not registered at the Naming Service" << endl;
      if (services.odometry || services.motion || services.buttons || services.eventChannel || services.infrared || services.tactile || services.sonar)
	cout << "There are other base services registered. This indicates a problem in the base module or the Naming Service" << endl;
      if (services.b21BaseRunning) {
	cout << "The base server is running but not registered at the Naming Service" << endl;
	cout << "You should restart it. If the problem persists, it is a problem with the Naming Service" << endl;
      } else if (services.b21BaseRunnable) {
	cout << "The base server is not running, but should be startable without problems" << endl;
      } else {
	cout << "The base server is not running and cannot be started" << endl;
	cout << "It might be a bug in the software" << endl;
	cout << "It must be started at stanleft" << endl;
      }
    }


    //buttons
    if (services.buttons)
    {
      if (services.b21BaseRunning) {
	cout << "The buttons server is running and registered at the Naming Service" << endl;
      } else {
	cout << "The buttons server is registered at the Naming service, but the server is not running." << endl;
	cout << "This is usually due to a crash in the base server." << endl;
	cout << "You should restart the base server with the -MiroRebindIOR argument" << endl;
      }
    } else {
      cout << "The buttons server is not registered at the Naming Service" << endl;
      if (services.odometry || services.motion || services.buttons || services.eventChannel || services.infrared || services.tactile || services.sonar)
	cout << "There are other base services registered. This indicates a problem in the base module or the Naming Service" << endl;
      if (services.b21BaseRunning) {
	cout << "The base server is running but not registered at the Naming Service" << endl;
	cout << "You should restart it. If the problem persists, it is a problem with the Naming Service" << endl;
      } else if (services.b21BaseRunnable) {
	cout << "The base server is not running, but should be startable without problems" << endl;
      } else {
	cout << "The base server is not running and cannot be started" << endl;
	cout << "It might be a bug in the software" << endl;
	cout << "It must be started at stanleft" << endl;
      }
    }


    //infrared
    if (services.infrared)
    {
      if (services.b21BaseRunning) {
	cout << "The infrared server is running and registered at the Naming Service" << endl;
      } else {
	cout << "The infrared server is registered at the Naming service, but the server is not running." << endl;
	cout << "This is usually due to a crash in the base server." << endl;
	cout << "You should restart the base server with the -MiroRebindIOR argument" << endl;
      }
    } else {
      cout << "The infrared server is not registered at the Naming Service" << endl;
      if (services.odometry || services.motion || services.buttons || services.eventChannel || services.infrared || services.tactile || services.sonar)
	cout << "There are other base services registered. This indicates a problem in the base module or the Naming Service" << endl;
      if (services.b21BaseRunning) {
	cout << "The base server is running but not registered at the Naming Service" << endl;
	cout << "You should restart it. If the problem persists, it is a problem with the Naming Service" << endl;
      } else if (services.b21BaseRunnable) {
	cout << "The base server is not running, but should be startable without problems" << endl;
      } else {
	cout << "The base server is not running and cannot be started" << endl;
	cout << "It might be a bug in the software" << endl;
	cout << "It must be started at stanleft" << endl;
      }
    }


    //tactile
    if (services.tactile)
    {
      if (services.b21BaseRunning) {
	cout << "The tactile server is running and registered at the Naming Service" << endl;
      } else {
	cout << "The tactile server is registered at the Naming service, but the server is not running." << endl;
	cout << "This is usually due to a crash in the base server." << endl;
	cout << "You should restart the base server with the -MiroRebindIOR argument" << endl;
      }
    } else {
      cout << "The tactile server is not registered at the Naming Service" << endl;
      if (services.odometry || services.motion || services.buttons || services.eventChannel || services.infrared || services.tactile || services.sonar)
	cout << "There are other base services registered. This indicates a problem in the base module or the Naming Service" << endl;
      if (services.b21BaseRunning) {
	cout << "The base server is running but not registered at the Naming Service" << endl;
	cout << "You should restart it. If the problem persists, it is a problem with the Naming Service" << endl;
      } else if (services.b21BaseRunnable) {
	cout << "The base server is not running, but should be startable without problems" << endl;
      } else {
	cout << "The base server is not running and cannot be started" << endl;
	cout << "It might be a bug in the software" << endl;
	cout << "It must be started at stanleft" << endl;
      }
    }


    //laser
    if (services.laser)
    {
      if (services.laserRunning) {
	cout << "The laser server is running and registered at the Naming Service" << endl;
	if (!services.odometry) 
        {
	  cout << "The odometry server is not running" << endl;
	  cout << "This might mean that the base server crashed" << endl;
	  cout << "You should restart both the base server and the laser server" << endl;
	}
      } else {
	cout << "The laser server is registered at the Naming service, but the server is not running." << endl;
	cout << "This is usually due to a crash in the server." << endl;
	cout << "You should restart the laser server with the -MiroRebindIOR argument" << endl;
      }
    } else {
      cout << "The laser server is not registered at the Naming Service" << endl;
      if (services.laserRunning) {
	cout << "The laser server is running but not registered at the Naming Service" << endl;
	cout << "You should restart it. If the problem persists, it is a problem with the Naming Service" << endl;
      } else if (services.laserRunnable) {
	cout << "The laser server is not running, but should be startable without problems" << endl;
      } else {
	cout << "The laser server is not running and cannot be started" << endl;
	cout << "It might be a bug in the software" << endl;
	cout << "It must be started at stanleft" << endl;
	cout << "Check that the server has the SUID bit and is owned by root" << endl;
      }
    }
    
  } else {
    cout << "stanleft is not responding; probably the robot is shut down" << endl;
    cout << "If the robot does not respond, it is pointless to try loading any further services" << endl;
    cout << "Try switching it on first" << endl;
    cout << "It can also be a network/password problem" << endl << endl;
  }


  ///////////////
  //  Stanright
  ///////////////


  if (services.stanright) 
  {
    cout << endl << "stanright is responding" << endl << endl;

    //speech
    if (services.speech)
    {
      if (services.speechRunning) {
	cout << "The speech server is running and registered at the Naming Service" << endl;
	cout << "If the robot does not speak, you should check the connections" << endl;
      } else {
	cout << "The speech server is registered at the Naming service, but the server is not running." << endl;
	cout << "This is usually due to a crash in the server." << endl;
	cout << "You should restart the speech server with the -MiroRebindIOR argument" << endl;
      }
    } else {
      if (services.speechRunning) {
	cout << "The speech server is running but not registered at the Naming Service" << endl;
	cout << "You should restart it. If the problem persists, it is a problem with the Naming Service" << endl;
      } else if (services.speechRunnable) {
	cout << "The speech server is not running, but should be startable without problems" << endl;
      } else {
	cout << "The speech server is not running and cannot be started" << endl;
	cout << "It might be a bug in the software" << endl;
	cout << "It must be started at stanright" << endl;
      }
    }
  } else {
    if (!services.stanleft)
    {
      cout << "stanright is not responding; probably the robot is shut down" << endl;
      cout << "If the robot does not respond, it is pointless to try loading any further services" << endl;
      cout << "Try switching it on first" << endl;
      cout << "It can also be a network/password problem" << endl;
    } else {
      cout << "stanright is not responding." << endl;
      cout << "It might be shut down" << endl;
      cout << "stanleft does respond; you should try to reboot all the robot if you need the speech or video servers" << endl;
      cout << "otherwise you can still work with the robot" << endl;
    }
  }


}










using namespace Miro;

int main(int argc, char *argv[])
{
  struct ServicesStatus services;

  SSH stanleft(0,0),stanright(0,0);

  signal(SIGCHLD, killZombies);
 
  Client client(argc,argv);

  services.eventChannel=false;

  try
  {

    ///////////////////////
    //
    //  Trying to log in
    //
    ///////////////////////

    cout << "Trying stanleft..." << endl;
    stanleft.open("stanleft");
    services.stanleft=stanleft.ping();
    if (services.stanleft) 
      cout << "OK" << endl;
    else 
      cout << "stanleft is unreachable or you have no access" << endl;

    cout << "Trying stanright..." << endl;
    stanright.open("stanright");
    services.stanright=stanright.ping();
    if (services.stanright) 
      cout << "OK" << endl;
    else 
      cout << "stanright is unreachable or you have no access" << endl;


    ///////////////////////
    //
    //  Checking if environement variables are set in stanleft
    //
    ///////////////////////

    if (services.stanleft) {
      cout << "Checking variables..." << endl;
      string var="";
      services.variablesSet=true;
      var=stanleft.exec("echo $ACE_ROOT",5);
      services.variablesSet&=(var!="\n")&&(var!="");
      var=stanleft.exec("echo $MIRO_ROOT",5);
      services.variablesSet&=(var!="\n")&&(var!="");
      var=stanleft.exec("echo $TAO_ROOT",5);
      services.variablesSet&=(var!="\n")&&(var!="");
      var=stanleft.exec("echo $NameServiceIOR",5);
      services.variablesSet&=(var!="\n")&&(var!="");
      if (services.variablesSet) cout << "OK" << endl;
      else cout << "Some variable(s) are not properly defined" << endl;
    } else {
      services.variablesSet=false;
    }


    ///////////////////////
    //
    //  Checking services in Naming Service
    //
    ///////////////////////


    cout << "Trying Odometry..." << endl;
    try {
      Odometry_var odometry(client.resolveName<Odometry>("Odometry"));
      services.odometry=true;
      cout << "OK" << endl;
    }
    catch (...)
    {
      services.odometry=false;
    }

    cout << "Trying Motion..." << endl;
    try {
      Motion_var motion(client.resolveName<Motion>("Motion"));
      services.motion=true;
      cout << "OK" << endl;
    }
    catch (...)
    {
      services.motion=false;
    }

    cout << "Trying Sonar..." << endl;
    try {
      RangeSensor_var sonar(client.resolveName<RangeSensor>("Sonar"));
      services.sonar=true;
      cout << "OK" << endl;
    }
    catch (...)
    {
      services.sonar=false;
    }

    cout << "Trying Tactile..." << endl;
    try {
      RangeSensor_var tactile(client.resolveName<RangeSensor>("Tactile"));
      services.tactile=true;
      cout << "OK" << endl;
    }
    catch (...)
    {
      services.tactile=false;
    }

    cout << "Trying Infrared..." << endl;
    try {
      RangeSensor_var infrared(client.resolveName<RangeSensor>("Infrared"));
      services.infrared=true;
      cout << "OK" << endl;
    }
    catch (...)
    {
      services.infrared=false;
    }

    cout << "Trying Buttons..." << endl;
    try {
      Buttons_var buttons(client.resolveName<Buttons>("Buttons"));
      services.buttons=true;
      cout << "OK" << endl;
    }
    catch (...)
    {
      services.buttons=false;
    }

    cout << "Trying Laser..." << endl;
    try {
      RangeSensor_var laser(client.resolveName<RangeSensor>("Laser"));
      services.laser=true;
      cout << "OK" << endl;
    }
    catch (...)
    {
      services.laser=false;
    }

    cout << "Trying Speech..." << endl;
    try {
      DtlkSpeech_var speech(client.resolveName<DtlkSpeech>("Speech"));
      services.speech=true;
      cout << "OK" << endl;
    }
    catch (...)
    {
      services.speech=false;
    }

    ///////////////////////
    //
    //  Trying to start the services
    //
    ///////////////////////

    if (services.variablesSet) 
    {
      int pid=0;
    
      if (services.stanleft)
      {
	cout << "Checking if b21Base is startable..." << endl;
	pid=stanleft.isRunning("b21Base");
	if (pid==0) 
	{
	  services.b21BaseRunning=false;
	  stanleft.exec("cd ${MIRO_ROOT}/bin",2);
	  stanleft.exec("b21Base&",5);
	  sleep(5);
	  pid=stanleft.isRunning("b21Base");
	  services.b21BaseRunnable=(pid!=0);
	  stanleft.kill("b21Base");
	} else {
	  services.b21BaseRunnable=true;
	  services.b21BaseRunning=true;
	}
	if (services.b21BaseRunnable)
	  cout << "OK" << endl;
	else 
	  cout << "b21Base is not runnable or it is unstable" << endl;
      } else {
	services.b21BaseRunning=false;
	services.b21BaseRunnable=false;
      }

      if (services.stanleft)
      {
	cout << "Checking if sickLaserService is startable..." << endl;
	pid=stanleft.isRunning("sickLaserService");
	if (pid==0) 
	{
	  services.laserRunning=false;
	  stanleft.exec("cd ${MIRO_ROOT}/bin",2);
	  stanleft.exec("sickLaserService&",5);
	  sleep(5);
	  pid=stanleft.isRunning("sickLaserService");
	  services.laserRunnable=(pid!=0);
	  stanleft.kill("sickLaserService");
	} else {
	  services.laserRunning=true;
	  services.laserRunnable=true;
	}
	if (services.laserRunnable)
	  cout << "OK" << endl;
	else 
	  cout << "sickLaserService is not runnable or it is unstable" << endl;
      } else {
	services.laserRunning=false;
	services.laserRunnable=false;
      }
	
      if (services.stanright)
      {
	cout << "Checking if dtlkSpeech is startable..." << endl;
	pid=stanright.isRunning("dtlkSpeech");
	if (pid==0) 
	{
	  services.speechRunning=false;
	  stanright.exec("cd ${MIRO_ROOT}/bin",2);
	  stanright.exec("dtlkSpeech&",5);
	  sleep(5);
	  pid=stanright.isRunning("dtlkSpeech");
	  services.speechRunnable=(pid!=0);
	  stanright.kill("dtlkSpeech");
	} else {
	  services.speechRunning=true;
	  services.speechRunnable=true;
	}
	if (services.speechRunnable)
	  cout << "OK" << endl;
	else 
	  cout << "dtlkSpeech is not runnable or it is unstable" << endl;
      } else {
	services.speechRunnable=false;
	services.speechRunning=false;
      }

    } else {
      services.b21BaseRunnable=false;
      services.laserRunnable=false;
      services.speechRunnable=false;
      services.videoRunnable=false;
    }





    doAnalysis(services);

  }
  catch (string &s)
  {
    cerr << "Exception: " << s << endl;
  }  
  catch (...)
  {
    cerr << "Uncaught exception: " << endl;

    killZombies();
    stanleft.close();
    stanright.close();
    throw;
  }

  stanleft.close();
  stanright.close();

  // wait for all children to finish
  killZombies();


}
