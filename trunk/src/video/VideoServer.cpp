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

#include "VideoServer.h"
#include "Parameters.h"

#include "miro/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"

#include <iostream>

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cerr;

VideoService::VideoService(int argc, char *argv[]) :
  Super(argc, argv),
  schedparams_(ACE_SCHED_FIFO, 10),
  // Video board initialization
  videoDevice(Video::Parameters::instance()->grabber),
  consumer(videoDevice, NULL, &schedparams_),
  grabber(&consumer)

  // Service initialization
{
  init();
  DBG(cout << "VideoService initialized.." << endl);
}

VideoService::VideoService(Server& _server) :
  Super(_server),
  schedparams_(ACE_SCHED_FIFO, 10),
  // Video board initialization
  videoDevice(Video::Parameters::instance()->grabber),
  consumer(videoDevice, NULL, &schedparams_),
  grabber(&consumer)

  // Service initialization
{
  init();
  DBG(cout << "VideoService initialized.." << endl);
}

void
VideoService::init()
{
  videoDevice.connect();
  consumer.open(NULL);

  // register the grabber interface at the POA
  pVideo = grabber._this();

  addToNameService(pVideo, "Video");
}

VideoService::~VideoService()
{
  DBG(cout << "Destructing VideoService." << endl);

  consumer.cancel();
  videoDevice.disconnect();


  // Deactivate the interfaces.
  // we have to do this manually for none owned orbs,
  // as the class goes out of scope before
  // the orb is shut down
  PortableServer::ObjectId_var oid;
  oid =  poa->reference_to_id (pVideo);
  poa->deactivate_object (oid.in());
}
