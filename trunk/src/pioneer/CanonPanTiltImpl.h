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
#ifndef CanonPanTiltImpl_hh
#define CanonPanTiltImpl_hh

#include "miro/CanonPanTiltS.h"

#include "miro/Exception.h"
#include "pioneer/PioneerConnection.h"

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class Exception;
};

namespace Pioneer
{
  // forward declerations
  //  class Connection;
  class Consumer;
};

namespace Canon
{
  class CanonPanTiltImpl :  public virtual POA_Miro::CanonPanTilt,
		      public virtual ::Miro::PanTilt
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    CanonPanTiltImpl(Pioneer::Connection& _connection,
	       Pioneer::Consumer& _consumer, bool _upsideDown=false) throw(Miro::Exception);
    virtual ~CanonPanTiltImpl();

    //-------------------------------------------------------------------------
    // from pan.idl
    //-------------------------------------------------------------------------
    virtual void setPan(double angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual double getPan()
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from tilt.idl
    //-------------------------------------------------------------------------
    virtual void setTilt(double angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual double getTilt()
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from panTilt.idl
    //-------------------------------------------------------------------------
    virtual Miro::PanTiltPositionIDL getPosition()
      throw(Miro::EDevIO);
    virtual void setPosition(const Miro::PanTiltPositionIDL & dest) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from canonPanTilt.idl
    //-------------------------------------------------------------------------
    virtual void waitCompletion()
      throw(Miro::EDevIO);
    virtual void setWaitPosition(const Miro::PanTiltPositionIDL & dest) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual void setSpdAcc(const Miro::CanonPanTiltSpdAccIDL & dest)
      throw(Miro::ETimeOut, Miro::EDevIO);
    virtual Miro::CanonPanTiltSpdAccIDL getSpdAcc()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual Miro::CanonPanTiltLimitsIDL getLimits()
      throw(Miro::EDevIO, Miro::ETimeOut);

    Answer * getAnswer();
    //    unsigned char * getAnswer();
    void addAnswer(unsigned char val);
    void newAnswer();
    void checkAnswer() throw(Miro::EDevIO,Miro::EOutOfBounds);

  protected:
    Answer * pAnswer;

    Pioneer::Connection&         connection; // encapsulating communication to hardware
    Pioneer::Consumer&           consumer;   // asynchronous processing of pioneer output
    
    static ACE_Time_Value maxWait;
    bool initialized;
    double panPulseRatio;
    double tiltPulseRatio;
    double currentPan;
    double currentTilt;
    bool upsideDown;

    //    unsigned char answer[20];
    //    int answerIndex;
    //    bool answerValid;

    double getPanPulseRatio();
    double getTiltPulseRatio();
    void initialize();

  };

  inline Answer* CanonPanTiltImpl::getAnswer() {return pAnswer;}
  //  inline unsigned char * CanonPanTiltImpl::getAnswer() {return answer;}

  inline void CanonPanTiltImpl::newAnswer() {
    cout << "new answer" << endl;
    connection.getCamera(0); //flush buffers
    //  answerValid=false; 
    //    answerIndex=0; 
    pAnswer->init();
  }
};

#endif




