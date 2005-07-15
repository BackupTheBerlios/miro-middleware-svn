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
#include "miro/IO.h"

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
		    "[Video::Consumer] Could not set sched parameters: " << 
		    std::endl << schedp_ <<
		    "[Video::Consumer] Maybe suid root is missing. Will work on default sched policy");
    }

    while (!canceled())
    {
      try {
	// clock_t start = clock();
	if (videoDevice.calcConnectivity()) {
	  ACE_Time_Value maxJitter = 
	    (maxSynchJitter_ == ACE_Time_Value::zero)?
	    ACE_Time_Value::zero :
	    ACE_OS::gettimeofday() - maxSynchJitter_;
	  if (videoDevice.deviceAsynchLinkManager()->tryAcquireBufferSets(maxJitter)) {
	    videoDevice.setBrokerRequestQueue();
	    videoDevice.processFilterTree();
	    videoDevice.deviceAsynchLinkManager()->releaseBufferSets();
	  }
	  else {
	    MIRO_LOG_OSTR(LL_WARNING, 
			  "Device " << videoDevice.name() << 
			  "VideoConsumer::svc() skipping image as asynch device is not ready yet.");
	    videoDevice.skipImage();
	  }
	}
	else {
	  MIRO_LOG_OSTR(LL_WARNING, 
			"Device " << videoDevice.name() << 
			"VideoConsumer::svc() skipping image due to synch latencies");
	  videoDevice.skipImage();
	}
	// std::cout << "TIME " << (double)(clock() - start)/CLOCKS_PER_SEC  << std::endl;

      }
      catch(Miro::CException& e) {
	MIRO_LOG_OSTR(LL_ERROR,
		      videoDevice.name() <<
		      " VideoConsumer::svc() caught Miro::CException: " << 
		      std::endl <<
		      e << std::endl);
	ACE_OS::sleep(ACE_Time_Value(0, 500000));
      }
      catch(Miro::Exception& e) {
	MIRO_LOG_OSTR(LL_ERROR,
		      videoDevice.name() <<
		      " VideoConsumer::svc() caught Miro::Exception: " << 
		      e << std::endl);
	ACE_OS::sleep(ACE_Time_Value(0, 500000));
      }
    }

    MIRO_LOG(LL_NOTICE, "VideoConsumer::svc() exiting.");
    return 0;
  }
}
