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
  ACE_Time_Value Consumer::maxWait(0, 500000);

  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer(Device& _device,
		     ACE_Sched_Params * pschedp) :
    videoDevice(_device),
    mutex(),
    cond(mutex),
    pCurrentImageData(NULL),
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
    pCurrentImageData = NULL;

    if (ACE_OS::sched_params(schedp_) == -1) {
      std::cerr << "[Video::Consumer] Could not set sched parameters." << endl 
		<< "[Video::Consumer] Maybe suid root is missing." << endl
		<< "[Video::Consumer] Will work on default sched policy" << endl;
    }

    ACE_Time_Value prevTimeStamp = ACE_OS::gettimeofday();

    int counter = 49;
    double deltaT[50];

    while (!canceled())
    {
      ACE_Time_Value timeStamp;
      try {
	videoDevice.process();
	videoDevice.processFilterTree();
	timeStamp = videoDevice.timeStamp();
      }
      catch(Miro::CException& e) {
	cout << "VideoConsumer::svc() caught Miro::CException: " << endl
	     << e << endl;
      }
      catch(Miro::Exception& e) {
	cout << "VideoConsumer::svc() caught Miro::Exception: " << e << endl;
      }
      cond.broadcast();

      // jitter statistics
      ACE_Time_Value dT = timeStamp - prevTimeStamp;
      deltaT[counter] = (double)dT.sec() + (double)dT.usec() / 1000000.;
      prevTimeStamp = timeStamp;
      --counter;

      if (counter < 0) {
	double mean = deltaT[0];
	for (int i = 49; i > 0; --i)
	  mean += deltaT[i];
	mean /= 50.;

	double var = 0.;
	for (int i = 49; i >= 0; --i)
	  var += (deltaT[i] - mean) * (deltaT[i] - mean);
	var /= 49.;
	cout << "VideoConsumer: mean=" << mean << "sec \t var=" << sqrt(var) << endl;
	counter = 49;
      }
    }

    cout << "VideoConsumer::svc() exiting." << endl;
    return 0;
  }
};
