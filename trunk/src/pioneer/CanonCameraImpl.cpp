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

  // maximum wait time for cond.wait calls
  ACE_Time_Value CanonCameraImpl::maxWait = ACE_Time_Value(0, 500000);

  CanonCameraImpl::CanonCameraImpl(Pioneer::Connection& _connection,
				   Canon::Answer *_pAnswer) throw(Exception) :
    pAnswer(_pAnswer),
    connection(_connection),
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
  // from CanonCamera.idl
  //-------------------------------------------------------------------------
  void
  CanonCameraImpl::setZoom(short value) throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut)
  {
    value=value*128/100; //change to camera coordinates
    bool done=false;
    char tmp[3];

    if (!initialized) initialize();

    Message zoom(SET_ZOOM_POSITION1,int2str(tmp,value,2));

    while (!done) {
      Miro::Guard guard(pAnswer->mutex); 
      pAnswer->init();
      connection.sendCamera(zoom);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }
  
  short
  CanonCameraImpl::getZoom() throw(Miro::EDevIO, Miro::ETimeOut)
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

    return 100*result/128;
  }
  
  void 
  CanonCameraImpl::setFocus(short factor) throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut)
  {
    bool done=false;
    char tmp[3];

    if (!initialized) initialize();

    Miro::FocusRangeIDL range;    
    range=getFocusRange();
    factor=factor*(range.max-range.min)/100+range.min;

    Message focus(SET_FOCUS_POSITION,int2str(tmp,factor,4));
    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      connection.sendCamera(focus);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }

  short
  CanonCameraImpl::getFocus() throw(Miro::EDevIO, Miro::ETimeOut)
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

    return 100*(result-range.min)/(range.max-range.min);
  }
  
  void
  CanonCameraImpl::autoFocus() throw(Miro::EDevIO, Miro::ETimeOut)
  {
    bool done=false;
    if (!initialized) initialize();

    Message focus(FOCUS_AUTO,0x30);

    while (!done) {
      Miro::Guard guard(pAnswer->mutex); 
      pAnswer->init();
      connection.sendCamera(focus);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }

  Miro::FocusRangeIDL 
  CanonCameraImpl::getFocusRange() throw(Miro::EDevIO, Miro::ETimeOut)
  {
    Miro::FocusRangeIDL result;
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
    
    return result;
  }


  //-------------------------------------------------------------------
  // auxiliary functions
  //-------------------------------------------------------------------

  void
  CanonCameraImpl::initialize()
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
  
  void CanonCameraImpl::checkAnswer() throw (Miro::EDevIO,Miro::EOutOfBounds, Miro::ETimeOut)
  {
    while (!pAnswer->isValid()) {
	ACE_Time_Value timeout(ACE_OS::gettimeofday());
	timeout+=maxWait;
	
	if (pAnswer->cond.wait(&timeout)==-1) throw Miro::ETimeOut(); 
	//wait until the answer is finished or timeout
      }

    if (pAnswer->header()!=ANSWER_HEADER) throw Miro::EDevIO("[CanonCameraImpl] Erroneous header in answer");
    if (pAnswer->errorCode()==ERROR_PARAMETER) throw Miro::EOutOfBounds("[CanonCameraImpl] Parameter Error");
    if (pAnswer->errorCode()==ERROR_MODE) throw Miro::EDevIO("[CanonCameraImpl] Mode Error");
  }
};
