// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
	exit(-1);
      }
    }

    MIRO_LOG(LL_NOTICE, "VideoConsumer::svc() exiting.");
    return 0;
  }
}
