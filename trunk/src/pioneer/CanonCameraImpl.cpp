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


#include "pioneer/CanonCameraImpl.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/PioneerParameters.h"
#include "pioneer/CanonMessage.h"

#include "miro/Synch.h"
#include "miro/Exception.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif


namespace Canon
{
  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;
  using Miro::Exception;
  //  using Miro::rad2Deg;
  //  using Miro::deg2Rad;

  // maximum wait time for cond.wait calls
  ACE_Time_Value CanonCameraImpl::maxWait = ACE_Time_Value(0, 500000);

  CanonCameraImpl::CanonCameraImpl(Pioneer::Connection& _connection,
			 Pioneer::Consumer& _consumer,Canon::Answer *_pAnswer) throw(Exception) :
    pAnswer(_pAnswer),
    connection(_connection),
    consumer(_consumer),
    initialized(false)
  {
    DBG(cout << "Constructing CanonCameraImpl" << endl);
  }

  CanonCameraImpl::~CanonCameraImpl()
  {
    DBG(cout << "Destructing CanonCameraImpl" << endl);
  }

  void
  CanonCameraImpl::done() 
  {
    Canon::Message msg(LOCAL_CONTROL_MODE,0x31);
    connection.sendCamera(msg);
  }

  // 
  // IDL interface implementation


  //-------------------------------------------------------------------------
  // from pan.idl
  //-------------------------------------------------------------------------
  void
  CanonCameraImpl::setZoom(short value) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    value=value*128/100; //change to camera coordinates
    bool done=false;
    char tmp[3];

    if (!initialized) initialize();

    Message zoom(SET_ZOOM_POSITION1,int2str(tmp,value,2));
    while (!done) {
      pAnswer->init();
      connection.sendCamera(zoom);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }
  
  short
  CanonCameraImpl::getZoom() throw(Miro::EDevIO)
  {
    bool done=false;

    if (!initialized) initialize();

    Message zoom=Message(GET_ZOOM_POSITION1);

    while (!done) {
      pAnswer->init();
      connection.sendCamera(zoom);
      while (!pAnswer->errorCode()) usleep(1000);
      
      //if busy, keep trying...
      if (pAnswer->errorCode()==ERROR_BUSY) continue;
      if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(2);
      checkAnswer();
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }

    return 100*str2int(pAnswer->parameter(),2)/128;
  }
  
  void 
  CanonCameraImpl::setFocus(short factor) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    bool done=false;
    char tmp[3];

    if (!initialized) initialize();

    Miro::FocusRangeIDL range;    
    range=getFocusRange();
    factor=factor*(range.max-range.min)/100+range.min;
    
    Message focus(SET_FOCUS_POSITION,int2str(tmp,factor,4));
    while (!done) {
      pAnswer->init();
      connection.sendCamera(focus);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }

  short
  CanonCameraImpl::getFocus() throw(Miro::EDevIO)
  {
    bool done=false;

    if (!initialized) initialize();

    Miro::FocusRangeIDL range=getFocusRange();

    Message focus=Message(GET_FOCUS_POSITION,0x30);

    while (!done) {
      pAnswer->init();
      connection.sendCamera(focus);
      while (!pAnswer->errorCode()) usleep(1000);
      
      //if busy, keep trying...
      if (pAnswer->errorCode()==ERROR_BUSY) continue;
      if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(4);
      checkAnswer();
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }

    return 100*(str2int(pAnswer->parameter(),4)-range.min)/(range.max-range.min);
  }
  
  void
  CanonCameraImpl::autoFocus() throw(Miro::EDevIO)
  {
    bool done=false;
    if (!initialized) initialize();

    Message focus(FOCUS_AUTO,0x30);
    while (!done) {
      pAnswer->init();
      connection.sendCamera(focus);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }

  Miro::FocusRangeIDL 
  CanonCameraImpl::getFocusRange() throw(Miro::EDevIO)
  {
    Miro::FocusRangeIDL result;
    bool done=false;

    if (!initialized) initialize();

    Message focus(GET_FOCUS_RANGE,0x32);

    while (!done) {
      pAnswer->init();
      connection.sendCamera(focus);
      while (!pAnswer->errorCode()) usleep(1000);
      
      //if busy, keep trying...
      if (pAnswer->errorCode()==ERROR_BUSY) continue;
      if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(8);
      checkAnswer();
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
    result.min=str2int(pAnswer->parameter(),4);
    result.max=str2int(pAnswer->parameter()+4,4);
    
    return result;
  }

  /*
    CanonCameraImpl::waitCompletion() throw(Miro::EDevIO)
    {
    Miro::PanTiltPositionIDL goal,dest;
    
    goal.panvalue=currentPan;
    goal.tiltvalue=currentTilt;
    
    //if the camera is inverse mounted, must change the sign
    if (upsideDown) {
    goal.panvalue=-goal.panvalue;
    goal.tiltvalue=-goal.tiltvalue;
    }
    
    do {
    //test if finished
    dest=getPosition();
    } while ((fabs(goal.panvalue-dest.panvalue)>0.01)||(fabs(goal.tiltvalue-dest.tiltvalue)>0.01));
    //0.01 rad tolerance (aprox 0.6 deg)
    }
  */

  /*
    CanonCameraLimitsIDL 
    CanonCameraImpl::getLimits() throw(Miro::EDevIO, Miro::ETimeOut)
    {
    CanonCameraLimitsIDL result;
    if (!initialized) initialize();
    
    Message getMinPan(GET_ANGLE_LIMITS,0x30);
    Message getMaxPan(GET_ANGLE_LIMITS,0x31);
    Message getMinTilt(GET_ANGLE_LIMITS,0x32);
    Message getMaxTilt(GET_ANGLE_LIMITS,0x33);

    if (minPan<=-1e10) {
    //if not initialized, get it from the camera;
    pAnswer->init();
    connection.sendCamera(getMinPan);
    //wait for error code completion
    while (!pAnswer->errorCode()) usleep(1000);
    //get the rest
    if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(4);
    checkAnswer(); //wait for completion
    minPan=deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000)*panPulseRatio);
    }
    
    if (minTilt<=-1e10) {
    //if not initialized, get it from the camera
    pAnswer->init();
    connection.sendCamera(getMinTilt);
    //wait for error code completion
    while (!pAnswer->errorCode()) usleep(1000);
    //get the rest
    if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(4);
    checkAnswer(); //wait for completion
    minTilt=deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000)*tiltPulseRatio);
    }
    
    if (maxPan<=-1e10) {
    //if not initialized, get it from the camera
    pAnswer->init();
    connection.sendCamera(getMaxPan);
    //wait for error code completion
    while (!pAnswer->errorCode()) usleep(1000);
    //get the rest
    if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(4);
    checkAnswer(); //wait for completion
    maxPan=deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000)*panPulseRatio);
    }
    
    if (maxTilt<=-1e10) {
    //if not initialized, get it from the camera
    pAnswer->init();
    connection.sendCamera(getMaxTilt);
    //wait for error code completion
    while (!pAnswer->errorCode()) usleep(1000);
    //get the rest
    if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(4);
    checkAnswer(); //wait for completion
    maxTilt=deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000)*tiltPulseRatio);
    }
    
    //if in inverse mounting, the max and min values must be switched
    result.minpanposition=(upsideDown?-maxPan:minPan);
    result.maxpanposition=(upsideDown?-minPan:maxPan);
    result.mintiltposition=(upsideDown?-maxTilt:minTilt);
    result.maxtiltposition=(upsideDown?-minTilt:maxTilt);
    
    waitInitialize(false,true); //don't force reinit but wait
    return result;
    }
  */

  //-------------------------------------------------------------------
  // auxiliary functions
  //-------------------------------------------------------------------

  void
  CanonCameraImpl::initialize()
  {
    if (!initialized) {
      Canon::Message msg(HOST_CONTROL_MODE,0x30);
      pAnswer->init();
      connection.sendCamera(msg);
      checkAnswer();
    }
    initialized=true;
  }
  
  void
  CanonCameraImpl::waitInitialize(bool force,bool forceWait)
  {
    bool done;
    
    //if it is already initialized,
    //   no reinitialization asked
    //   and no need to wait
    // then exit
    if (!forceWait&&!force&&initialized) return;

    //initialize only if not initialized
    //or asked for initialization
    if (force||!initialized) initialize();
    do {
      done=true;
      try {
	usleep(1000);
	//	waitCompletion(); //wait until initialization is done
      }
      catch (Miro::EDevIO & e) {
	// avoid "mode" errors
	done=false;
      }
    } while(!done);
  }

  void 
  CanonCameraImpl::addAnswer(unsigned char val)
  {
    pAnswer->add(val);
  }

  void CanonCameraImpl::checkAnswer() throw (Miro::EDevIO,Miro::EOutOfBounds)
  {
    while (!pAnswer->isValid()) usleep(1000); // wait to get the whole answer
    if (pAnswer->header()!=ANSWER_HEADER) throw Miro::EDevIO("[CanonCameraImpl] Erroneous header in answer");
    if (pAnswer->errorCode()==ERROR_PARAMETER) throw Miro::EOutOfBounds("[CanonCameraImpl] Parameter Error");
    if (pAnswer->errorCode()==ERROR_MODE) throw Miro::EDevIO("[CanonCameraImpl] Mode Error");
  }
};
