// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "pioneer/CanonCameraControlImpl.h"

#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/CanonMessage.h"
#include "pioneer/Parameters.h"

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/Angle.h"
#include "miro/TimeHelper.h"

#include <iostream>

using Miro::ZoomRangeIDL;
using Miro::TimeIDL;
using Miro::FocusRangeIDL;
using Miro::ShutterRangeIDL;
using Miro::deg2Rad;
using Miro::rad2Deg;

using std::cout;
using std::cerr;
using std::endl;

namespace Canon
{
  // maximum wait time for cond.wait calls
  ACE_Time_Value CanonCameraControlImpl::maxWait = ACE_Time_Value(0, 500000);

  CanonCameraControlImpl::CanonCameraControlImpl(Pioneer::Parameters * _parameters, 
						 Pioneer::Connection &  _connection,
						 Canon::Answer * _pAnswer) throw(Miro::Exception) : 
    Super(_parameters->cameraParams),
    parameters_(_parameters),
    connection(_connection),
    pAnswer(_pAnswer),
    initialized(false)
  {}

  CanonCameraControlImpl::~CanonCameraControlImpl() {}

  void CanonCameraControlImpl::setZoom(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!zoomParameters_.present) {
      throw Miro::EDevIO("Zoom not supported");
    }
    if (!testZoom(value)) {
      throw Miro::EOutOfBounds();
    }
    setTargetZoom(value);
   
    //change to camera coordinates
    //camera: wide = 0
    //camera: tele = 128
    //real: wide   = rangeMax;
    //real: tele   = rangeMin;
    //ranges and value are in radians
    int cameraValue=int(128-128*(value-zoomParameters_.rangeMin)/(zoomParameters_.rangeMax-zoomParameters_.rangeMin));

    bool done=false;
    char tmp[3];

    if (!initialized) initialize();

    Message zoom(SET_ZOOM_POSITION1,int2str(tmp,cameraValue,2));

    while (!done) {
      Miro::Guard guard(pAnswer->mutex); 
      pAnswer->init();
      connection.sendCamera(zoom);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }

  }
  
  float CanonCameraControlImpl::getZoom() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    bool done=false;
    short result=0;
    
    if (!initialized) initialize();

    Message zoom=Message(GET_ZOOM_POSITION1);

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      connection.sendCamera(zoom);
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout)==-1) throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }
      
      //if busy, keep trying...
      if (pAnswer->errorCode()==ERROR_BUSY) {
	continue;
      }
      if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(2);
      checkAnswer();
      if (pAnswer->errorCode()==ERROR_NO_ERROR) {
	result=str2int(pAnswer->parameter(),2);
	done=true;
      }
    }

    return (zoomParameters_.rangeMin+((zoomParameters_.rangeMax-zoomParameters_.rangeMin)/128)*(128-result));
  }

  void CanonCameraControlImpl::setFocus(short value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!focusParameters_.present) {
      throw Miro::EDevIO("Focus not supported");
    }
    Miro::FocusRangeIDL range;    
    range=getFocusRange();
    if ((value>range.max) || (value<range.min)) {
      throw Miro::EOutOfBounds("Focus value outside range for this zoom level");
    }
    setTargetFocus(value);

    bool done=false;
    char tmp[3];

    if (!initialized) initialize();

    //TODO: convert according to focusParameters
    float factor=100;

    int focus = int(value*(range.max-range.min)/factor+range.min);

    Message focusMsg(SET_FOCUS_POSITION,int2str(tmp,focus,4));

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      connection.sendCamera(focusMsg);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }

  }
  
  short CanonCameraControlImpl::getFocus() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    bool done=false;
    short result=0;

    if (!initialized) initialize();

    Miro::FocusRangeIDL range=getFocusRange();

    Message focus=Message(GET_FOCUS_POSITION,0x30);

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      connection.sendCamera(focus);
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;

	if (pAnswer->cond.wait(&timeout)==-1) throw Miro::ETimeOut();
	//wait until the error code is finished or timeout
      }
      
      //if busy, keep trying...
      if (pAnswer->errorCode()==ERROR_BUSY) {
	continue;
      }
      if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(4);
      checkAnswer();
      if (pAnswer->errorCode()==ERROR_NO_ERROR) {
	done=true;
	result=str2int(pAnswer->parameter(),4);
      }
    }

    //TODO: convert according to focusParameters

    return 100*(result-range.min)/(range.max-range.min);

  }

  FocusRangeIDL CanonCameraControlImpl::getFocusRange() throw(Miro::EDevIO, Miro::ETimeOut) {
    FocusRangeIDL result;

     bool done=false;

    if (!initialized) initialize();

    Message focus(GET_FOCUS_RANGE,0x32);

    while (!done) {
      Miro::Guard guard(pAnswer->mutex); 
      pAnswer->init();
      connection.sendCamera(focus);
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout)==-1) throw Miro::ETimeOut(); 
	//wait until the error code is finished or timeout
      }
      
      //if busy, keep trying...
      if (pAnswer->errorCode()==ERROR_BUSY) {
	continue;
      }
      if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(8);
      checkAnswer();
      if (pAnswer->errorCode()==ERROR_NO_ERROR) {
	result.min=str2int(pAnswer->parameter(),4);
	result.max=str2int(pAnswer->parameter()+4,4);
	done=true;
      }
    }

    //TODO: convert according to focusParameters.

    return result;
  }


  void CanonCameraControlImpl::setAutoFocus(CORBA::Boolean value) throw(Miro::EDevIO, Miro::ETimeOut) {
    if (!focusParameters_.autoFocus) {
      throw Miro::EDevIO("Autofocus not supported");
    }
    autoFocusSet_=value;
    bool done=false;
    if (!initialized) initialize();

    Message focus(FOCUS_AUTO,value?0x30:0x31);

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      connection.sendCamera(focus);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }

  }

  void CanonCameraControlImpl::setShutter(TimeIDL& value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!shutterParameters_.present) {
      throw Miro::EDevIO("Shutter not supported");
    }
    if (!testShutter(value)) {
      throw Miro::EOutOfBounds();
    }
    setTargetShutter(value);

    double dValue=value.sec+double(value.usec)/1e6;
    short parameter=0;
    
    if (dValue<1/10000) {
      parameter=0x19;
    } else if (dValue<1/6000) {
      parameter=0x18;
    } else if (dValue<1/4000) {
      parameter=0x17;
    } else if (dValue<1/3000) {
      parameter=0x16;
    } else if (dValue<1/2500) {
      parameter=0x15;
    } else if (dValue<1/2000) {
      parameter=0x14;
    } else if (dValue<1/1750) {
      parameter=0x13;
    } else if (dValue<1/1500) {
      parameter=0x12;
    } else if (dValue<1/1250) {
      parameter=0x11;
    } else if (dValue<1/1000) {
      parameter=0x10;
    } else if (dValue<1/850) {
      parameter=0x0F;
    } else if (dValue<1/725) {
      parameter=0x0E;
    } else if (dValue<1/600) {
      parameter=0x0D;
    } else if (dValue<1/500) {
      parameter=0x0C;
    } else if (dValue<1/425) {
      parameter=0x0B;
    } else if (dValue<1/350) {
      parameter=0x0A;
    } else if (dValue<1/300) {
      parameter=0x09;
    } else if (dValue<1/250) {
      parameter=0x08;
    } else if (dValue<1/215) {
      parameter=0x07;
    } else if (dValue<1/180) {
      parameter=0x06;
    } else if (dValue<1/150) {
      parameter=0x05;
    } else if (((parameters_->cameraParams.format=="pal") && (dValue<1/120)) ||
	       ((parameters_->cameraParams.format=="ntsc") && (dValue<1/125))) {
      parameter=0x04;
    } else if (dValue<1/100) {
      parameter=0x03;
    } else if (dValue<1/90) {
      parameter=0x02;
    } else if (((parameters_->cameraParams.format=="pal") && (dValue<1/60)) ||
	       ((parameters_->cameraParams.format=="ntsc") && (dValue<1/75))) {
      parameter=0x01;
    } else if (((parameters_->cameraParams.format=="pal") && (dValue<1/50)) ||
	       ((parameters_->cameraParams.format=="ntsc") && (dValue<1/60))) {
      parameter=0x00;
    }

    //set the value actually used
    value=shutterCanon2Time(parameter,parameters_->cameraParams.format);
    
    char tmp[4];
    tmp[0]=0x35; //first byte needs to be 0x35.
    int2str(tmp+1,parameter,2); //second and third the actual value
    
    bool done=false;
    if (!initialized) initialize();
    
    Message shutter(LIGHT_SHUTTER_SPEED,tmp);
    
    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      connection.sendCamera(shutter);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
    
  }

  TimeIDL CanonCameraControlImpl::shutterCanon2Time(short value, std::string format)
  {
    TimeIDL result;
    result.sec=0;

    double usec=0;

    switch (value) {
    case 0x00:
      usec=(format=="ntsc")?1/60:1/50;
      break;
    case 0x01:
      usec=(format=="ntsc")?1/75:1/60;
      break;
    case 0x02:
      usec=1/90;
      break;
    case 0x03:
      usec=1/100;
      break;
    case 0x04:
      usec=(format=="ntsc")?1/125:1/1200;
      break;
    case 0x05:
      usec=1/150;
      break;
    case 0x06:
      usec=1/180;
      break;
    case 0x07:
      usec=1/215;
      break;
    case 0x08:
      usec=1/250;
      break;
    case 0x09:
      usec=1/300;
      break;
    case 0x0A:
      usec=1/350;
      break;
    case 0x0B:
      usec=1/425;
      break;
    case 0x0C:
      usec=1/500;
      break;
    case 0x0D:
      usec=1/600;
      break;
    case 0x0E:
      usec=1/725;
      break;
    case 0x0F:
      usec=1/850;
      break;
    case 0x10:
      usec=1/1000;
      break;
    case 0x11:
      usec=1/1250;
      break;
    case 0x12:
      usec=1/1500;
      break;
    case 0x13:
      usec=1/1750;
      break;
    case 0x14:
      usec=1/2000;
      break;
    case 0x15:
      usec=1/2500;
      break;
    case 0x16:
      usec=1/3000;
      break;
    case 0x17:
      usec=1/4000;
      break;
    case 0x18:
      usec=1/6000;
      break;
    case 0x19:
      usec=1/10000;
      break;
    default:
      usec=0;
    }

    result.usec=int(usec*1e6);
    return result;
  }  

  TimeIDL CanonCameraControlImpl::getShutter() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    bool done=false;
    TimeIDL result;

    if (!initialized) initialize();

    Message shutter=Message(LIGHT_SHUTTER_SPEED,0x36);

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      connection.sendCamera(shutter);
      while (!pAnswer->errorCode()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;

	if (pAnswer->cond.wait(&timeout)==-1) throw Miro::ETimeOut();
	//wait until the error code is finished or timeout
      }
      
      //if busy, keep trying...
      if (pAnswer->errorCode()==ERROR_BUSY) {
	continue;
      }
      if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(4);
      checkAnswer();
      if (pAnswer->errorCode()==ERROR_NO_ERROR) {
	done=true;
	result=shutterCanon2Time(str2int(pAnswer->parameter(),2),parameters_->cameraParams.format);
      }
    }
    return result;

  }

  void
  CanonCameraControlImpl::setAELock(CORBA::Boolean value) throw(Miro::EDevIO, Miro::ETimeOut)
  {
    bool done=false;
    char tmp[3];
    if (!initialized) initialize();

    Message aeLock(LIGHT_AE,int2str(tmp,value?0x41:0x40,2));
    // 0x40 -> lock off
    // 0x41 -> lock on

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      connection.sendCamera(aeLock);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }


  //-------------------------------------------------------------------
  // auxiliary functions
  //-------------------------------------------------------------------

  void
  CanonCameraControlImpl::initialize()
  {
    if (!initialized) {
      Miro::Guard guard(pAnswer->mutex);
      Canon::Message msg(HOST_CONTROL_MODE,0x30);
      pAnswer->init();
      connection.sendCamera(msg);
      checkAnswer();
    }
    initialized=true;
  }

  void CanonCameraControlImpl::checkAnswer() throw (Miro::EDevIO,Miro::EOutOfBounds, Miro::ETimeOut)
  {
    while (!pAnswer->isValid()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout)==-1) throw Miro::ETimeOut(); 
	//wait until the answer is finished or timeout
      }

    if (pAnswer->header()!=ANSWER_HEADER) throw Miro::EDevIO("[CanonCameraControlImpl] Erroneous header in answer");
    if (pAnswer->errorCode()==ERROR_PARAMETER) throw Miro::EOutOfBounds("[CanonCameraControlImpl] Parameter Error");
    if (pAnswer->errorCode()==ERROR_MODE) throw Miro::EDevIO("[CanonCameraControlImpl] Mode Error");
  }

};
