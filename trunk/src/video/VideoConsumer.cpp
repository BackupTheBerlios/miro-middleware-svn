// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "VideoConsumer.h"
#include "VideoDevice.h"

#include "miro/Exception.h"
#include "miro/Log.h"

namespace Video
{
  using ::operator<<;

  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer(Device& _device,
		     ACE_Sched_Params * pschedp) :
    videoDevice(_device),
    schedp_(ACE_SCHED_OTHER, 0)

  {
    MIRO_LOG_CTOR("VideoConsumer");
    if (pschedp) 
      schedp_ = (*pschedp);
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//
  Consumer::~Consumer()
  { 
    MIRO_LOG_DTOR("VideoConsumer");
  }

  int 
  Consumer::svc()
  {
    if (ACE_OS::sched_params(schedp_) == -1) {
      MIRO_LOG_OSTR(LL_WARNING,
		    "[Video::Consumer] Could not set sched parameters." << 
		    std::endl <<
		    "[Video::Consumer] Maybe suid root is missing." << 
		    std::endl <<
		    "[Video::Consumer] Will work on default sched policy" << 
		    std::endl);
    }

    while (!canceled())
    {
      try {
	videoDevice.setBrokerRequestQueue();
	videoDevice.calcConnectivity();
	videoDevice.processFilterTree();
      }
      catch(Miro::CException& e) {
	MIRO_LOG_OSTR(LL_ERROR,
		      "VideoConsumer::svc() caught Miro::CException: " << 
		      std::endl <<
		      e << std::endl);
      }
      catch(Miro::Exception& e) {
	MIRO_LOG_OSTR(LL_ERROR,
		      "VideoConsumer::svc() caught Miro::Exception: " << 
		      e << std::endl);
      }
    }

    MIRO_LOG(LL_NOTICE, "VideoConsumer::svc() exiting.");
    return 0;
  }
}
