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


#include "pioneer/CanonPanTiltImpl.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/PioneerParameters.h"
#include "pioneer/CanonPanTiltMessage.h"

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
  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;
  using Miro::Exception;
  using Miro::rad2Deg;
  using Miro::deg2Rad;
  using Miro::CanonPanTiltSpdAccIDL;
  using Miro::CanonPanTiltLimitsIDL;

  // maximum wait time for cond.wait calls
  ACE_Time_Value CanonPanTiltImpl::maxWait = ACE_Time_Value(0, 500000);

  CanonPanTiltImpl::CanonPanTiltImpl(Pioneer::Connection& _connection,
			 Pioneer::Consumer& _consumer, bool _upsideDown) throw(Exception) :
    pAnswer(new Answer()),
    connection(_connection),
    consumer(_consumer),
    initialized(false),
    panPulseRatio(0.1125),
    tiltPulseRatio(0.1125),
    currentPan(0),
    currentTilt(0),
    upsideDown(_upsideDown)
  {
    DBG(cout << "Constructing CanonPanTiltImpl" << endl);
  }

  CanonPanTiltImpl::~CanonPanTiltImpl()
  {
    DBG(cout << "Destructing CanonPanTiltImpl" << endl);
    Canon::Message msg(LOCAL_CONTROL_MODE,0x31);
    connection.sendCamera(msg);
  }

  // 
  // IDL interface implementation


  //-------------------------------------------------------------------------
  // from pan.idl
  //-------------------------------------------------------------------------
  void
  CanonPanTiltImpl::setPan(double angle) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL dest;
    dest.panvalue=angle;
    dest.tiltvalue=currentTilt;
    setPosition(dest);
  }
  
  double
  CanonPanTiltImpl::getPan() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL pos=getPosition();
    return pos.panvalue;
    
}
  
  //-------------------------------------------------------------------------
  // from tilt.idl
  //-------------------------------------------------------------------------
  void 
  CanonPanTiltImpl::setTilt(double angle) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL dest;
    dest.panvalue=currentPan;
    dest.tiltvalue=angle;
    setPosition(dest);
  }

  double 
  CanonPanTiltImpl::getTilt() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL pos=getPosition();
    return pos.tiltvalue;

  }
  
  //-------------------------------------------------------------------------
  // from panTilt.idl
    //-------------------------------------------------------------------------
  Miro::PanTiltPositionIDL 
  CanonPanTiltImpl::getPosition() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL result;
    bool done=false;

    if (!initialized) {
      initialize();
      do {
	done=true;
	try {
	  waitCompletion(); //wait until initialization is done
	}
	catch (Miro::EDevIO & e) {
	  // avoid "mode" errors
	  done=false;
	}
      } while(!done);
    }
    done=false;

    Canon::Message getPanTilt(PAN_TILT_GET_POSITION);
    
    while(!done) {
      pAnswer->init();
      connection.sendCamera(getPanTilt);
      while (!pAnswer->errorCode()) usleep(1000); //wait until the error code is finished
      //if camera is initializing, wait and try again
      if (pAnswer->errorCode()==ERROR_BUSY) {
	continue;
      }

      if (pAnswer->errorCode()==ERROR_NO_ERROR) {
	connection.getCamera(8); //get the rest of the answer
      }
      
      checkAnswer(); //wait for completion and check
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
    }
    result.panvalue=deg2Rad(double(str2int(pAnswer->parameter(),4)-0x8000)*panPulseRatio);
    result.tiltvalue=deg2Rad(double(str2int(pAnswer->parameter()+4,4)-0x8000)*tiltPulseRatio);
    return result;
  }

  void 
  CanonPanTiltImpl::setPosition(const Miro::PanTiltPositionIDL & dest) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    bool done=false;
    double panTmp,tiltTmp; //to reset value in case of error

    if (!initialized) {
      initialize();
      do {
	done=true;
	try {
	  waitCompletion(); //wait until initialization is done
	}
	catch (Miro::EDevIO & e) {
	  // avoid "mode" errors
	  done=false;
	}
      } while(!done);
    }
    done=false;
    currentPan=dest.panvalue;
    currentTilt=dest.tiltvalue;
    
    char angleASCII[9];
    Canon::Message panTiltValue(PAN_TILT_SET_POSITION,
				int2str(
					angleASCII,
					int(rad2Deg(currentPan)/panPulseRatio+0x8000),
					int(rad2Deg(currentTilt)/tiltPulseRatio+0x8000),
					4));
    while (!done) {
      pAnswer->init();
      connection.sendCamera(panTiltValue);
      while (!pAnswer->errorCode()) usleep(1000); //wait until the error code is finished
    
      //check if the format is OK and no errors are returned
      //if there is an error, an exception will be thrown
      try {
	checkAnswer();
      }
      catch (Miro::EOutOfBounds& e) {
	currentPan=panTmp; //reset old values in case of error
	currentTilt=tiltTmp;
	throw (e);
      }
      catch (Miro::EDevIO& e) {
	currentPan=panTmp; //reset old values in case of error
	currentTilt=tiltTmp;
	throw (e);
      }
      catch (Miro::Exception& e) {
	currentPan=panTmp; //reset old values in case of error
	currentTilt=tiltTmp;
	throw (e);
      }
      catch (CORBA::Exception& e) {
	currentPan=panTmp; //reset old values in case of error
	currentTilt=tiltTmp;
	throw (e);
      }
      if (pAnswer->errorCode()==ERROR_NO_ERROR) done=true;
      if (pAnswer->errorCode()==ERROR_BUSY) {
	pAnswer->init();
	Message stop(PAN_TILT_STOP,0x30);
	connection.sendCamera(stop);
      }
    }

  }
  
  //-------------------------------------------------------------------------
  // from canonPanTilt.idl
  //-------------------------------------------------------------------------

  void
  CanonPanTiltImpl::waitCompletion() throw(Miro::EDevIO)
  {
    Miro::PanTiltPositionIDL goal,dest;

    goal.panvalue=currentPan;
    goal.tiltvalue=currentTilt;

    do {
      //test if finished
      dest=getPosition();
      cout << "." << endl;
    } while ((fabs(goal.panvalue-dest.panvalue)>0.01)||(fabs(goal.tiltvalue-dest.tiltvalue)>0.01));
      //0.01 rad tolerance (aprox 0.6 deg)
  }
  
  void
  CanonPanTiltImpl::setWaitPosition(const Miro::PanTiltPositionIDL & dest) throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    setPosition(dest);
    waitCompletion();
  }
  
  void
  CanonPanTiltImpl::setSpdAcc(const CanonPanTiltSpdAccIDL & dest) throw(Miro::ETimeOut, Miro::EDevIO)
  {

    //TODO
    if (!initialized) initialize();

    int i=(int)dest.targetpanspeed;
    i++; //really useless...
  }
  
  CanonPanTiltSpdAccIDL 
  CanonPanTiltImpl::getSpdAcc() throw(Miro::EDevIO, Miro::ETimeOut)
  {
    //TODO
    CanonPanTiltSpdAccIDL result;
    if (!initialized) initialize();

    return result;
  }

  CanonPanTiltLimitsIDL 
  CanonPanTiltImpl::getLimits() throw(Miro::EDevIO, Miro::ETimeOut)
  {
    CanonPanTiltLimitsIDL result;
    if (!initialized) initialize();

    return result;
  }
  
  //-------------------------------------------------------------------
  // auxiliary functions
  //-------------------------------------------------------------------
  double
  CanonPanTiltImpl::getPanPulseRatio()
  {
    panPulseRatio=0.1125;

    Canon::Message getRatio(GET_PAN_ANGLE_RATIO,0x30);
    pAnswer->init();
    connection.sendCamera(getRatio);
    while (!pAnswer->errorCode()) usleep(1000); //wait until the error code is finished
    
    //if there is no error, read the rest of the parameters
    if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(4);

    //check if the format is OK and no errors are returned
    //if there is an error, an exception will be thrown
    checkAnswer();

    panPulseRatio=double(str2int(pAnswer->parameter(),4))/100000.0;
    //    DBG(cout << "[CanonPanTilt] New pan Pulse Ratio=" << panPulseRatio << endl;)

    return panPulseRatio;
  }

  double 
  CanonPanTiltImpl::getTiltPulseRatio()
  {
    tiltPulseRatio=0.1125;

    Canon::Message getRatio(GET_TILT_ANGLE_RATIO,0x31);
    pAnswer->init();
    connection.sendCamera(getRatio);
    while (!pAnswer->errorCode()) usleep(1000); //wait until the error code is finished
    
    //if there is no error, read the rest of the parameters
    if (pAnswer->errorCode()==ERROR_NO_ERROR) connection.getCamera(4);

    //check if the format is OK and no errors are returned
    //if there is an error, an exception will be thrown
    checkAnswer();

    tiltPulseRatio=double(str2int(pAnswer->parameter(),4))/100000.0;
    //    DBG(cout << "[CanonPanTilt] New tilt Pulse Ratio=" << tiltPulseRatio << endl;)
    
    return tiltPulseRatio;
  }

  void
  CanonPanTiltImpl::initialize()
  {
    if (!initialized) {
      Canon::Message msg(HOST_CONTROL_MODE,0x30);
      pAnswer->init();
      connection.sendCamera(msg);
      checkAnswer();
      getPanPulseRatio();
      getTiltPulseRatio();
    }
    Canon::Message initMsg(INITIALIZE1,0x030);
    pAnswer->init();
    connection.sendCamera(initMsg);
    checkAnswer();
    initialized=true;
  }
  
  void CanonPanTiltImpl::addAnswer(unsigned char val)
  {
    pAnswer->add(val);
  }

  void CanonPanTiltImpl::checkAnswer() throw (Miro::EDevIO,Miro::EOutOfBounds)
  {
    while (!pAnswer->isValid()) usleep(1000); // wait to get the whole answer
    if (pAnswer->header()!=ANSWER_HEADER) throw Miro::EDevIO("[CanonPanTiltImpl] Erroneous header in answer");
    if (pAnswer->errorCode()==ERROR_PARAMETER) throw Miro::EOutOfBounds("[CanonPanTiltImpl] Parameter Error");
    if (pAnswer->errorCode()==ERROR_MODE) throw Miro::EDevIO("[CanonPanTiltImpl] Mode Error");
  }
};
