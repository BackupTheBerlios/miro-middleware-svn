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
#ifndef CanonCameraImpl_hh
#define CanonCameraImpl_hh

#include "miro/CanonCameraS.h"

#include "miro/Exception.h"

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class Exception;
};

namespace Pioneer
{
  // forward declerations
  class Connection;
  class Consumer;
};

namespace Canon
{
  class Answer;
  class CanonCameraImpl :  public virtual POA_Miro::CanonCamera
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    CanonCameraImpl(Pioneer::Connection& _connection,
		    Pioneer::Consumer& _consumer,
		    Answer * _pAnswer) throw(Miro::Exception);
    virtual ~CanonCameraImpl();

    virtual void setZoom(short factor)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual short getZoom()
      throw(Miro::EDevIO);

    virtual void setFocus(short factor)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual short getFocus()
      throw(Miro::EDevIO);
    virtual void autoFocus()
      throw(Miro::EDevIO);
    virtual Miro::FocusRangeIDL getFocusRange()
      throw(Miro::EDevIO);

    //    virtual void waitCompletion()
    //      throw(Miro::EDevIO);
    //    virtual Miro::CanonPanTiltLimitsIDL getLimits()
    //      throw(Miro::EDevIO, Miro::ETimeOut);

    Answer * getAnswer();
    void addAnswer(unsigned char val);
    void checkAnswer() throw(Miro::EDevIO,Miro::EOutOfBounds);

    //closes the connection and returns control to Remote Controller
    void done();


  protected:
    Answer * pAnswer;

    Pioneer::Connection&         connection; // encapsulating communication to hardware
    Pioneer::Consumer&           consumer;   // asynchronous processing of pioneer output
    
    static ACE_Time_Value maxWait;
    bool initialized;
    /*    double panPulseRatio;
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
    */

    void initialize();
    void waitInitialize(bool force=false,bool forceWait=false);

  };

  inline Answer* CanonCameraImpl::getAnswer() {return pAnswer;}
};

#endif




