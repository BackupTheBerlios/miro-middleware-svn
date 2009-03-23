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
#include "pioneer/CanonPanTiltImpl.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/CameraMessage.h"
#include "pioneer/CanonMessage.h"
#include "pioneer/Parameters.h"

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/Angle.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif


namespace Canon
{
  using std::cout;
  using std::cerr;
  using std::endl;

  using ::operator<<;

  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;
  using Miro::Exception;
  using Miro::rad2Deg;
  using Miro::deg2Rad;
  using Miro::CanonPanTiltSpdAccIDL;
  using Miro::PanTiltLimitsIDL;
  using Miro::PanLimitsIDL;
  using Miro::TiltLimitsIDL;

  // maximum wait time for cond.wait calls
  ACE_Time_Value CanonPanTiltImpl::maxWait = ACE_Time_Value(0, 500000);

  CanonPanTiltImpl::CanonPanTiltImpl(Pioneer::Connection& _connection,
				     Answer * _pAnswer,
				     Miro::PanTiltParameters _panTiltParameters,
				     bool _upsideDown) throw(Exception) :
    PanTiltImpl(_panTiltParameters),
    pAnswer(_pAnswer),
    connection(_connection),
    initialized(false),
    panPulseRatio(0.1125),
    tiltPulseRatio(0.1125),
    currentPan(0),
    currentTilt(0),
    panMinSpeed(-1),
    panMaxSpeed(-1),
    tiltMinSpeed(-1),
    tiltMaxSpeed(-1),
    minPan(-1e10),
    maxPan(-1e10),
    minTilt(-1e10),
    maxTilt(-1e10),
    upsideDown(_upsideDown)
  {
    DBG(cout << "Constructing CanonPanTiltImpl" << endl);
  }
  
  CanonPanTiltImpl::~CanonPanTiltImpl()
  {
    DBG(cout << "Destructing CanonPanTiltImpl" << endl);
    Canon::Message msg(LOCAL_CONTROL_MODE,0x31);
    send(msg);
  }
  
  // 
  // IDL interface implementation


  //-------------------------------------------------------------------------
  // from pan.idl
  //-------------------------------------------------------------------------
  void
  CanonPanTiltImpl::setPan(CORBA::Float angle) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL dest;

    //if camera is inverse mounted, change sign for current saved value
    //as it will be changed again on setPosition
    dest.panValue = angle;
    dest.tiltValue = (upsideDown? -currentTilt : currentTilt);

    setPosition(dest);
  }
  
  CORBA::Float
  CanonPanTiltImpl::getPan() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL pos = getPosition();
    return pos.panValue;
    
  }
  
  //-------------------------------------------------------------------------
  // from tilt.idl
  //-------------------------------------------------------------------------
  void 
  CanonPanTiltImpl::setTilt(CORBA::Float angle) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL dest;
    //if camera is not inverse mounted, change sign for current saved value
    //as it will be changed again on setPosition
    //to maintain coherence with other PanTilt implementations it must
    //work opposite to camera default.
    dest.panValue=(!upsideDown?-currentPan:currentPan);
    dest.tiltValue=angle;
    setPosition(dest);
  }

  CORBA::Float 
  CanonPanTiltImpl::getTilt() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL pos = getPosition();
    return pos.tiltValue;

  }
  
  //-------------------------------------------------------------------------
  // from panTilt.idl
    //-------------------------------------------------------------------------
  Miro::PanTiltPositionIDL 
  CanonPanTiltImpl::getPosition() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL result;
    bool done = false;

    waitInitialize();

    Canon::Message getPanTilt(PAN_TILT_GET_POSITION);

    while(!done) {
      Miro::Guard guard(pAnswer->mutex);       
      pAnswer->init();
      send(getPanTilt);

      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;

	if (pAnswer->cond.wait(&timeout) == -1) 
	  throw ETimeOut(); 
	    //wait until the error code is finished or timeout
      }
      //if camera is initializing, wait and try again
      if (pAnswer->errorCode() == ERROR_BUSY) {
	continue;
      }

      if (pAnswer->errorCode() == ERROR_NO_ERROR) {
	connection.getCamera(8); //get the rest of the answer
      }
      
      checkAnswer(); //wait for completion and check
      if (pAnswer->errorCode() == ERROR_NO_ERROR)
	done = true;
    }

    result.panValue =
      deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000) * panPulseRatio);
    result.tiltValue =
      deg2Rad(double(str2int(pAnswer->parameter()+4,4)-0x8000) * tiltPulseRatio);

    if (upsideDown) {
      //if inverse mounting, switch directions
      result.tiltValue = -result.tiltValue;
    } else {
      //Miro's horitzontal standart is opposite to camera's !!!
      result.panValue = -result.panValue;
    }

    return result;
  }

  void 
  CanonPanTiltImpl::setPosition(const Miro::PanTiltPositionIDL & dest) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    bool done = false;
    double panTmp = currentPan;
    double tiltTmp = currentTilt; //to reset value in case of error

    waitInitialize();

    //if camera in inverse position, change sign for pan/tilt
    //Miro's pan works opposite to camera's!!!
    currentPan = dest.panValue * (!upsideDown? -1 : 1);
    currentTilt = dest.tiltValue* (upsideDown? -1 : 1);
    
    char angleASCII[9];
    Canon::Message panTiltValue(PAN_TILT_SET_POSITION,
				int2str(
					angleASCII,
					int(rad2Deg(currentPan)/panPulseRatio+0x8000),
					int(rad2Deg(currentTilt)/tiltPulseRatio+0x8000),
					4));

    while (!done) {
      { //scope for mutex
	Miro::Guard guard(pAnswer->mutex); 
	pAnswer->init();
	send(panTiltValue);
	try {
	  while (!pAnswer->errorCode()) {
	    ACE_Time_Value timeout(ACE_OS::gettimeofday());
	    timeout += maxWait;
	    
	    if (pAnswer->cond.wait(&timeout) == -1) 
	      throw ETimeOut(); 
	    //wait until the error code is finished or timeout
	  }
	  
	  //check if the format is OK and no errors are returned
	  //if there is an error, an exception will be thrown
	  checkAnswer();
	}
	catch (Miro::ETimeOut& e) {
	  currentPan = panTmp; //reset old values in case of error
	  currentTilt = tiltTmp;
	  throw (e);
	}
	catch (Miro::EOutOfBounds& e) {
	  currentPan = panTmp; //reset old values in case of error
	  currentTilt = tiltTmp;
	  throw (e);
	}
	catch (Miro::EDevIO& e) {
	  currentPan = panTmp; //reset old values in case of error
	  currentTilt = tiltTmp;
	  throw (e);
	}
	catch (Miro::Exception& e) {
	  currentPan = panTmp; //reset old values in case of error
	  currentTilt = tiltTmp;
	  throw (e);
	}
      }//scope for mutex end
      if (pAnswer->errorCode() == ERROR_NO_ERROR) 
	done = true;
      if (pAnswer->errorCode() == ERROR_BUSY) {
	Miro::Guard guard(pAnswer->mutex); 
	pAnswer->init();
	Message stop(PAN_TILT_STOP,0x30);
	send(stop);
	checkAnswer();
      }
    }
    setTargetPosition(dest);
  }
  
  //-------------------------------------------------------------------------
  // from canonPanTilt.idl
  //-------------------------------------------------------------------------

  void
  CanonPanTiltImpl::waitCompletion() throw(Miro::EDevIO, Miro::ETimeOut)
  {
    Miro::PanTiltPositionIDL goal,dest;

    goal.panValue = currentPan;
    goal.tiltValue = currentTilt;

    //if the camera is inverse mounted, must change the sign
    if (upsideDown) {
      goal.tiltValue = -goal.tiltValue;
    } else {
      //Miro's pan standart is opposite to camera's !!!
      goal.panValue = -goal.panValue;
    }

    do {
      //test if finished
      dest = getPosition();
    } 
    while ( (fabs(goal.panValue-dest.panValue) > 0.01) || 
	    (fabs(goal.tiltValue-dest.tiltValue) > 0.01) );
      //0.01 rad tolerance (aprox 0.6 deg)
  }
  
  void
  CanonPanTiltImpl::setWaitPosition(const Miro::PanTiltPositionIDL & dest) 
    throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut)
  {
    setPosition(dest);
    waitCompletion();
  }
  
  void
  CanonPanTiltImpl::setSpdAcc(const CanonPanTiltSpdAccIDL & dest) 
    throw(Miro::ETimeOut, Miro::EDevIO, Miro::EOutOfBounds)
  {
    waitInitialize();
    int panspeed, tiltspeed;

    panspeed = int(rad2Deg(dest.targetpanspeed) / panPulseRatio);
    tiltspeed = int(rad2Deg(dest.targettiltspeed) / tiltPulseRatio);
    
    char tmp[4];

    Message setPanSpeed(SET_PAN_SPEED, int2str(tmp, panspeed, 3));
    Message setTiltSpeed(SET_TILT_SPEED, int2str(tmp, tiltspeed, 3));

    {//scope for mutex
      Miro::Guard guard(pAnswer->mutex); 
      
      pAnswer->init();
      send(setPanSpeed);
      //wait for completion
      checkAnswer();
    }//scope for mutex end

    {//scope for mutex
      Miro::Guard guard(pAnswer->mutex); 

      pAnswer->init();
      send(setTiltSpeed);
      //wait for completion
      checkAnswer();
    }//scope for mutex end
  }
  
  CanonPanTiltSpdAccIDL 
  CanonPanTiltImpl::getSpdAcc() throw(Miro::EDevIO, Miro::ETimeOut)
  {
    CanonPanTiltSpdAccIDL result;
    if (!initialized)
      initialize();

    Message getPanSpeed(GET_PAN_SPEED,0x30);
    Message getTiltSpeed(GET_TILT_SPEED,0x31);
    Message minPanSpeed(GET_PAN_MIN_SPEED,0x30);
    Message maxPanSpeed(GET_PAN_MAX_SPEED,0x31);
    Message minTiltSpeed(GET_TILT_MIN_SPEED,0x32);
    Message maxTiltSpeed(GET_TILT_MAX_SPEED,0x33);

    {//scope for mutex
      Miro::Guard guard(pAnswer->mutex); 

      pAnswer->init();
      send(getPanSpeed);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout += maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1)
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }
      //get the rest
      if (pAnswer->errorCode() == ERROR_NO_ERROR)
	connection.getCamera(3);
      checkAnswer(); //wait for completion
      result.targetpanspeed = deg2Rad(double(str2int(pAnswer->parameter(),3))*panPulseRatio);
    }//scope for mutex

    {//Scope for mutex
      Miro::Guard guard(pAnswer->mutex); 

      pAnswer->init();
      send(getTiltSpeed);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1) 
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }

      //get the rest
      if (pAnswer->errorCode() == ERROR_NO_ERROR)
	connection.getCamera(3);
      checkAnswer(); //wait for completion
      result.targettiltspeed = deg2Rad(double(str2int(pAnswer->parameter(),3))*tiltPulseRatio);
    }//scope for mutex

    if (panMinSpeed<0) {
      Miro::Guard guard(pAnswer->mutex); 

      //if not initialized, get it from the camera;
      pAnswer->init();
      send(minPanSpeed);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout += maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1)
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }
      //get the rest
      if (pAnswer->errorCode() == ERROR_NO_ERROR)
	connection.getCamera(3);
      checkAnswer(); //wait for completion
      panMinSpeed=deg2Rad(double(str2int(pAnswer->parameter(),3))*panPulseRatio);
    }
    result.panminspeed=panMinSpeed;
 
    if (tiltMinSpeed<0) {
      Miro::Guard guard(pAnswer->mutex); 
      //if not initialized, get it from the camera
      pAnswer->init();
      send(minTiltSpeed);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1)
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }

      //get the rest
      if (pAnswer->errorCode() == ERROR_NO_ERROR)
	connection.getCamera(3);
      checkAnswer(); //wait for completion
      tiltMinSpeed =
	deg2Rad(double(str2int(pAnswer->parameter(),3))*tiltPulseRatio);
    }
    result.tiltminspeed=tiltMinSpeed;


    if (panMaxSpeed<0) {
      Miro::Guard guard(pAnswer->mutex); 
      //if not initialized, get it from the camera
      pAnswer->init();
      send(maxPanSpeed);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1) 
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }

      //get the rest
      if (pAnswer->errorCode() == ERROR_NO_ERROR)
	connection.getCamera(3);
      checkAnswer(); //wait for completion
      panMaxSpeed = deg2Rad(double(str2int(pAnswer->parameter(),3))*panPulseRatio);
    }
    result.panmaxspeed = panMaxSpeed;

    if (tiltMaxSpeed<0) {
      Miro::Guard guard(pAnswer->mutex); 
      //if not initialized, get it from the camera
      pAnswer->init();
      send(maxTiltSpeed);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1) 
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }

      //get the rest
      if (pAnswer->errorCode() == ERROR_NO_ERROR)
	connection.getCamera(3);
      checkAnswer(); //wait for completion
      tiltMaxSpeed=deg2Rad(double(str2int(pAnswer->parameter(),3))*tiltPulseRatio);
    }
    result.tiltmaxspeed=tiltMaxSpeed;

    waitInitialize(false,true); //don't force reinit but wait

    return result;
  }

  PanTiltLimitsIDL 
  CanonPanTiltImpl::getPanTiltLimits() throw()
  {
    PanTiltLimitsIDL result;
    if (!initialized) 
      initialize();

    Message getMinPan(GET_ANGLE_LIMITS,0x30);
    Message getMaxPan(GET_ANGLE_LIMITS,0x31);
    Message getMinTilt(GET_ANGLE_LIMITS,0x32);
    Message getMaxTilt(GET_ANGLE_LIMITS,0x33);

    if (minPan<=-1e10) {
      Miro::Guard guard(pAnswer->mutex); 
      //if not initialized, get it from the camera;
      pAnswer->init();
      send(getMinPan);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout += maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1)
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }
      //get the rest
      if (pAnswer->errorCode( )== ERROR_NO_ERROR)
	connection.getCamera(4);
      checkAnswer(); //wait for completion
      minPan=deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000)*panPulseRatio);
    }
 
    if (minTilt<=-1e10) {
      Miro::Guard guard(pAnswer->mutex); 
      //if not initialized, get it from the camera
      pAnswer->init();
      send(getMinTilt);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1)
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }
      //get the rest
      if (pAnswer->errorCode() == ERROR_NO_ERROR)
	connection.getCamera(4);
      checkAnswer(); //wait for completion
      minTilt=deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000)*tiltPulseRatio);
    }

    if (maxPan<=-1e10) {
      Miro::Guard guard(pAnswer->mutex); 
      //if not initialized, get it from the camera
      pAnswer->init();
      send(getMaxPan);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1)
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }
      //get the rest
      if (pAnswer->errorCode() == ERROR_NO_ERROR)
	connection.getCamera(4);
      checkAnswer(); //wait for completion
      maxPan=deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000)*panPulseRatio);
    }

    if (maxTilt<=-1e10) {
      Miro::Guard guard(pAnswer->mutex); 
      //if not initialized, get it from the camera
      pAnswer->init();
      send(getMaxTilt);
      //wait for error code completion
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout) == -1)
	  throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }
      //get the rest
      if (pAnswer->errorCode() == ERROR_NO_ERROR) 
	connection.getCamera(4);
      checkAnswer(); //wait for completion
      maxTilt=deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000)*tiltPulseRatio);
    }

    //if in inverse mounting, the max and min values must be switched
    //remember that Miro's pan standart is opposite to camera's own
    result.pan.minAngle =(!upsideDown?-maxPan:minPan);
    result.pan.maxAngle =(!upsideDown?-minPan:maxPan);
    result.tilt.minAngle =(upsideDown?-maxTilt:minTilt);
    result.tilt.maxAngle =(upsideDown?-minTilt:maxTilt);

    waitInitialize(false,true); //don't force reinit but wait
    return result;
  }

  PanTiltLimitsIDL CanonPanTiltImpl::getLimits() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    cerr << "PanTilt::getLimits() is Deprecated" << endl;
    cerr << "Use PanTilt::getPanTiltLimits" << endl;
    return getPanTiltLimits();
  }

  PanLimitsIDL CanonPanTiltImpl::getPanLimits() throw()
  {
    PanTiltLimitsIDL panTiltLimits=getPanTiltLimits();
    return panTiltLimits.pan;
  }

  TiltLimitsIDL CanonPanTiltImpl::getTiltLimits() throw()
  {
    PanTiltLimitsIDL panTiltLimits=getPanTiltLimits();
    return panTiltLimits.tilt;
  }

  
  //-------------------------------------------------------------------
  // auxiliary functions
  //-------------------------------------------------------------------
  double
  CanonPanTiltImpl::getPanPulseRatio()
  {
    panPulseRatio=0.1125;

    Canon::Message getRatio(GET_PAN_ANGLE_RATIO,0x30);

    Miro::Guard guard(pAnswer->mutex); 
   
    pAnswer->init();
    send(getRatio);
    while (!pAnswer->errorCode()) {
      ACE_Time_Value timeout(ACE_OS::gettimeofday());
      timeout += maxWait;
      
      if (pAnswer->cond.wait(&timeout) == -1) 
	throw Miro::ETimeOut(); 
    }
    
    //if there is no error, read the rest of the parameters
    if (pAnswer->errorCode() == ERROR_NO_ERROR)
      connection.getCamera(4);

    //check if the format is OK and no errors are returned
    //if there is an error, an exception will be thrown
    checkAnswer();

    panPulseRatio=double(str2int(pAnswer->parameter(),4))/100000.0;
    
    return panPulseRatio;
  }

  double 
  CanonPanTiltImpl::getTiltPulseRatio()
  {
    tiltPulseRatio=0.1125;

    Canon::Message getRatio(GET_TILT_ANGLE_RATIO,0x31);

    Miro::Guard guard(pAnswer->mutex); 

    pAnswer->init();
    send(getRatio);
    while (!pAnswer->errorCode()) {
      ACE_Time_Value timeout(ACE_OS::gettimeofday());
      timeout += maxWait;
      
      if (pAnswer->cond.wait(&timeout) == -1) 
	throw Miro::ETimeOut(); 
    }
    //if there is no error, read the rest of the parameters
    if (pAnswer->errorCode() == ERROR_NO_ERROR)
      connection.getCamera(4);

    //check if the format is OK and no errors are returned
    //if there is an error, an exception will be thrown
    checkAnswer();

    tiltPulseRatio = double(str2int(pAnswer->parameter(),4))/100000.0;
    
    return tiltPulseRatio;
  }

  void
  CanonPanTiltImpl::initialize()
  {
    if (!initialized) {
      Miro::Guard guard(pAnswer->mutex);

      Canon::Message msg(HOST_CONTROL_MODE,0x30);
      pAnswer->init();
      send(msg);
      checkAnswer();

      Canon::Message initMsg(INITIALIZE1,0x030);
      pAnswer->init();
      send(initMsg);
      checkAnswer();
    }//scope for mutex

    if (!initialized) {
      getPanPulseRatio();
      getTiltPulseRatio();
    }
    initialized = true;
  }
  
  void
  CanonPanTiltImpl::waitInitialize(bool force, bool forceWait)
  {
    bool done;
    
    //if it is already initialized,
    //   no reinitialization asked
    //   and no need to wait
    // then exit
    if (!forceWait && !force && initialized)
      return;

    //initialize only if not initialized
    //or asked for initialization
    if (force || !initialized) 
      initialize();

    do {
      done = true;
      try {
	usleep(1000);
	waitCompletion(); //wait until initialization is done
      }
      catch (Miro::EDevIO & e) {
	// avoid "mode" errors
	done = false;
      }
    } while(!done);
  }

  void
  CanonPanTiltImpl::send(const Canon::Message &msg)
  {
    connection.getCamera(0); //flush buffer
    connection.sendCamera(msg);
    connection.getCamera(6); //get minimum answer
  }

  void 
  CanonPanTiltImpl::checkAnswer() 
    throw (Miro::EDevIO, Miro::EOutOfBounds, Miro::ETimeOut)
  {
    while (!pAnswer->isValid()) { //try to get the whole answer
      ACE_Time_Value timeout(ACE_OS::gettimeofday());
      timeout += maxWait;
      
      if (pAnswer->cond.wait(&timeout) == -1) 
	throw Miro::ETimeOut(); 
      //wait until the answer is finished or timeout
    }
    
    if (pAnswer->header() != ANSWER_HEADER) 
      throw Miro::EDevIO("[CanonPanTiltImpl] Erroneous header in answer");
    if (pAnswer->errorCode() == ERROR_PARAMETER) 
      throw Miro::EOutOfBounds("[CanonPanTiltImpl] Parameter Error");
    if (pAnswer->errorCode() == ERROR_MODE)
      throw Miro::EDevIO("[CanonPanTiltImpl] Mode Error");
  }
};
