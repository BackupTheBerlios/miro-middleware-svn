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
#ifndef CanonPanTiltImpl_hh
#define CanonPanTiltImpl_hh

#include "idl/CanonPanTiltS.h"

#include "miro/PanTiltImpl.h"

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
  // forward declarations
  class Consumer;
};

namespace Canon
{
  class CanonPanTiltImpl :  public virtual POA_Miro::CanonPanTilt,
			    //			    public virtual ::Miro::PanTilt,
			    public Miro::PanTiltImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    CanonPanTiltImpl(Pioneer::Connection& _connection,
		     Miro::PanParameters _panParameters,
		     Miro::TiltParameters _tiltParameters,
		     bool _upsideDown = false) throw(Miro::Exception);
    virtual ~CanonPanTiltImpl();

    //-------------------------------------------------------------------------
    // from pan.idl
    //-------------------------------------------------------------------------
    virtual void setPan(double angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual double getPan()
      throw(Miro::EDevIO);
    /**
     * returns the pan angle limits
     */
    virtual Miro::PanLimitsIDL getPanLimits() throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from tilt.idl
    //-------------------------------------------------------------------------
    virtual void setTilt(double angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual double getTilt()
      throw(Miro::EDevIO);
    /**
     * returns the tilt angle limits
     */
    virtual Miro::TiltLimitsIDL getTiltLimits() throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from panTilt.idl
    //-------------------------------------------------------------------------
    virtual Miro::PanTiltPositionIDL getPosition()
      throw(Miro::EDevIO);
    virtual void setPosition(const Miro::PanTiltPositionIDL & dest) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual Miro::PanTiltLimitsIDL getPanTiltLimits()
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from canonPanTilt.idl
    //-------------------------------------------------------------------------
    virtual void waitCompletion()
      throw(Miro::EDevIO, Miro::ETimeOut);
    virtual void setWaitPosition(const Miro::PanTiltPositionIDL & dest) 
      throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut);
    virtual void setSpdAcc(const Miro::CanonPanTiltSpdAccIDL & dest)
      throw(Miro::ETimeOut, Miro::EDevIO, Miro::EOutOfBounds);
    virtual Miro::CanonPanTiltSpdAccIDL getSpdAcc()
      throw(Miro::EDevIO, Miro::ETimeOut);
    /**
     * DEPRECATED.
     * Use getPanTiltLimits() instead
     */
    virtual Miro::PanTiltLimitsIDL getLimits()
      throw(Miro::EDevIO, Miro::ETimeOut);
   
    Answer * getAnswer();
    void checkAnswer() throw(Miro::EDevIO,Miro::EOutOfBounds, Miro::ETimeOut);

    //closes the connection and returns control to Remote Controller
    void done();


  protected:
    Answer answer;
    Answer * pAnswer;

    Pioneer::Connection&         connection; // encapsulating communication to hardware
    
    static ACE_Time_Value maxWait;
    bool initialized;
    double panPulseRatio;
    double tiltPulseRatio;
    double currentPan;
    double currentTilt;
    double panMinSpeed;
    double panMaxSpeed;
    double tiltMinSpeed;
    double tiltMaxSpeed;
    double minPan;
    double maxPan;
    double minTilt;
    double maxTilt;
    bool upsideDown;

    double getPanPulseRatio();
    double getTiltPulseRatio();
    void initialize();
    void waitInitialize(bool force=false,bool forceWait=false);

  };

  inline Answer* CanonPanTiltImpl::getAnswer() {
    return &answer;
  }
};

#endif




