// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoConsumer_h
#define VideoConsumer_h

#include "miro/Synch.h"
#include "miro/Thread.h"
#include "idl/TimeC.h"

namespace Video
{
  // forward declerations
  class Device;

  /**
   * Class for using the Video robot
   */
  class Consumer : public Miro::Thread
  {
    typedef Miro::Thread Super;
    
  public:
    Consumer(Device& _device, 
	     ACE_Sched_Params * pschedp = NULL);
    ~Consumer();

    virtual int svc();

  protected:
    Device& videoDevice;
    ACE_Sched_Params schedp_;
  };
};
#endif

