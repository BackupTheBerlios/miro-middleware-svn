// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef WindowArbiter_h
#define WindowArbiter_h

#include "MotionC.h"
#include "Synch.h"
#include "Arbiter.h"
#include "WindowArbiterMessage.h"
#include "WindowArbiterParameters.h"
#include "MotionArbiterMessage.h"
#include "MotionArbiterParameters.h"
#include "WindowArbiterViewerTask.h"

#include <ace/Reactor.h>

#include <orbsvcs/CosNotifyCommC.h>

#include "DynamicWindow.h"

#include <vector>
#include <string>

namespace Miro
{
  class StructuredPushSupplier;

  class WindowArbiter :
    public Arbiter,
    public ACE_Event_Handler
  {
  public:
    WindowArbiter(ACE_Reactor &, Motion_ptr _pMotion,
	StructuredPushSupplier * _pSupplier = NULL);
	
    ~WindowArbiter();

    // factory method for ArbiterParameters
    WindowArbiterParameters * getParametersInstance();

    virtual void init(const ArbiterParameters * _parms);
    virtual void arbitrate(const ArbiterMessage& _message);
    virtual const std::string& getName() const;
    
    void open();
    void close();
    
    virtual int handle_timeout(const ACE_Time_Value&, const void*);

  protected:
  
    void setMotion();
    
    typedef std::vector<WindowArbiterMessage> MessageVector;

    Motion_ptr pMotion_;
    StructuredPushSupplier * pSupplier_;

    Mutex mutex_;
    const WindowArbiterParameters * params_;
    MessageVector message_;
    
    ACE_Reactor &reactor;
    int timerId;
    
    // just as long as getVelocity doesn't work
    int leftVelocity_, rightVelocity_;

    DynamicWindow dynWindow_;
    DynamicWindow dynWindowFinished_;

    WindowArbiterViewerTask * winArbViewTask_;

    bool winArbViewTaskCreated;

    VelocityIDL currentVelocity;
    CosNotification::StructuredEvent notifyEvent;

    static const std::string name_;
    
  };
};
#endif
