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


#include "VideoConsumer.h"
#include "VideoDevice.h"
#include "VideoImpl.h"

#include "miro/Exception.h"
#include "miro/TimeHelper.h"

#include <algorithm>

// #undef DEBUG
#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Video
{
  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer(Device& _device,
		     ACE_Sched_Params * pschedp) :
    videoDevice(_device),
    schedp_(ACE_SCHED_OTHER, 0)

  {
    DBG(cout << "Constructing VideoConsumer." << endl);
    if (pschedp) 
      schedp_ = (*pschedp);
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//
  Consumer::~Consumer()
  { 
    DBG(cout << "Destructing VideoConsumer." << endl);
  }

  int 
  Consumer::svc()
  {
    if (ACE_OS::sched_params(schedp_) == -1) {
      std::cerr << "[Video::Consumer] Could not set sched parameters." << endl 
		<< "[Video::Consumer] Maybe suid root is missing." << endl
		<< "[Video::Consumer] Will work on default sched policy" << endl;
    }

    while (!canceled())
    {
      try {
	videoDevice.calcConnectivity();
	videoDevice.processFilterTree();
      }
      catch(Miro::CException& e) {
	cout << "VideoConsumer::svc() caught Miro::CException: " << endl
	     << e << endl;
      }
      catch(Miro::Exception& e) {
	cout << "VideoConsumer::svc() caught Miro::Exception: " << e << endl;
      }
    }

    cout << "VideoConsumer::svc() exiting." << endl;
    return 0;
  }
};
