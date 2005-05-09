// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "pioneer/CanonCameraImpl.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/CanonMessage.h"
#include "pioneer/Parameters.h"

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

  using std::cout;
  using std::cerr;
  using std::endl;

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
    send(msg);
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
      send(zoom);
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
      send(zoom);
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
    factor=factor*int(range.max-range.min)/100+int(range.min);

    Message focus(SET_FOCUS_POSITION,int2str(tmp,factor,4));
    cout << focus << endl;  				// only test
    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(focus);
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
      send(focus);
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

    return int(100*(result-range.min)/(range.max-range.min));
  }
  
  void
  CanonCameraImpl::autoFocus() throw(Miro::EDevIO, Miro::ETimeOut)
  {
    bool done=false;
    if (!initialized) initialize();

    Message focus(FOCUS_AUTO,0x30);
    cout << focus << endl;

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(focus);
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
      send(focus);
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



  void
  CanonCameraImpl::setAEoff() throw(Miro::EDevIO, Miro::ETimeOut)
  {
    bool done=false;
    char tmp[3];
    if (!initialized) initialize();

    //Message aeLock(LIGHT_AE,0x40);
    Message aeLock(LIGHT_AE,int2str(tmp,64,2));
    cout << aeLock << endl;
    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(aeLock);
      checkAnswer();
      //keep trying...
      cout <<"antwort" <<pAnswer->errorCode()<< endl;
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }

  void
  CanonCameraImpl::setAEon() throw(Miro::EDevIO, Miro::ETimeOut)
  {
    bool done=false;
    char tmp[3];
    if (!initialized) initialize();

    Message aeLock(LIGHT_AE,int2str(tmp,65,2));

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(aeLock);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }

  void
  CanonCameraImpl::setAE(short value) throw(Miro::EDevIO, Miro::ETimeOut)
  {
    char tmp[4];
    char fixChar[4];

    strcpy(fixChar , ";");   // set preparameter 3Bh
    if (!initialized) initialize();
    if (value < 16) value = 16;
    if (value > 255) value = 255;
    Message aeValue(LIGHT_AE,strcat(fixChar, int2str(tmp,value,2)));
cout << aeValue<< endl;
send(aeValue);
checkAnswer();
cout << " error" << pAnswer->errorCode() << endl;
/*

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(aeValue);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }*/
  }

  void
  CanonCameraImpl::setIrisAssignment(short value) throw(Miro::EDevIO, Miro::ETimeOut)
  {
    char tmp[3];
    char fixChar[4];

    strcpy(fixChar , "9"); // set preparameter 3Bh
    if (!initialized) initialize();
    if (value < 2) value = 2;
    if (value > 16) value = 16;
    Message aeIris(LIGHT_AE,strcat(fixChar, int2str(tmp,value,2)));
cout << aeIris << endl;
send(aeIris);
checkAnswer();
cout << "Error:" <<pAnswer->errorCode() << endl;

/*    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(aeIris);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }*/
  }

  void
  CanonCameraImpl::setAGCGain(short value)
  {
    char tmp[4];
    char fixChar[4];

    strcpy(fixChar , "7");   // set preparameter 37h
    if (!initialized) initialize();
    if (value < 0) value = 0;
    if (value > 255) value = 255;
    Message gainValue(LIGHT_AE,strcat(fixChar, int2str(tmp,value,3)));
send(gainValue);
checkAnswer();
cout << "error" << pAnswer->errorCode()<< endl;

/*    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(gainValue);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }*/
  }

  void
  CanonCameraImpl::setShutterSpeed(short value)
  {
    char tmp[4];
    char fixChar[4];

    strcpy(fixChar , "5");   // set preparameter 35h
    if (!initialized) initialize();
    if (value < 0) value = 0;
    if (value > 25) value = 25;
    Message shutterSpeedValue(LIGHT_AE,strcat(fixChar, int2str(tmp,value,3)));

send(shutterSpeedValue);
checkAnswer();
cout << "error" <<pAnswer->errorCode()<< endl;
/*    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(shutterSpeedValue);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }*/
  }

  void CanonCameraImpl::setWBauto()
  {
    bool done=false;
    if (!initialized) initialize();

    Message aeLock(WHITE_BALANCE,0x30);

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(aeLock);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }

  void CanonCameraImpl::setWBmanual()
  {
    bool done=false;
    if (!initialized) initialize();

    Message aeLock(WHITE_BALANCE,0x32);

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(aeLock);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }
  void CanonCameraImpl::setWBlock()
  {
    bool done=false;
    if (!initialized) initialize();

    Message wbLock(WHITE_BALANCE,0x31);

    while (!done) {
      Miro::Guard guard(pAnswer->mutex);
      pAnswer->init();
      send(wbLock);
      checkAnswer();
      //keep trying...
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
  }

  void CanonCameraImpl::setWBvalue(short value)
  {
    char tmp[4];
    char fixChar[4];

    strcpy(fixChar , "4");   // set preparameter 34h
    if (!initialized) initialize();
    if (value < 0) value = 0;
    if (value > 255) value = 255;
    Message wbValue(WHITE_BALANCE,strcat(fixChar, int2str(tmp,value,2)));
cout << wbValue<< endl;
send(wbValue);
checkAnswer();
cout << " error" << pAnswer->errorCode() << endl;
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
      send(msg);
      checkAnswer();
    }
    initialized=true;
  }

  void
  CanonCameraImpl::send(const Canon::Message &msg)
  {
    connection.getCamera(0); //flush buffer
    connection.sendCamera(msg);
    connection.getCamera(6); //get minimum answer
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
