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
#include "VideoDevice.h"
#include "VideoConsumer.h"
#include "VideoDeviceDummy.h"
#include "VideoDeviceBTTV.h"
#include "VideoDeviceMeteor.h"
#include "VideoDevice1394.h"
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
  pVideoDevice(NULL),
  pConsumer(NULL),
  pGrabber(NULL)
{
  init();
  DBG(cout << "VideoService initialized.." << endl);
}

VideoService::VideoService(Server& _server) :
  Super(_server),
  schedparams_(ACE_SCHED_FIFO, 10),
  pVideoDevice(NULL),
  pConsumer(NULL),
  pGrabber(NULL)
{
  init();
  DBG(cout << "VideoService initialized.." << endl);
}

void
VideoService::init()
{
  if (Video::Parameters::instance()->grabber == "bttv")
    pVideoDevice = new Video::VideoDeviceBTTV();
  //else if (Video::Parameters::instance()->grabber == "v4l")
  //	pVideoDevice = new  VideoDeviceV4L;
  else if (Video::Parameters::instance()->grabber== "meteor")
      pVideoDevice = new Video::VideoDeviceMeteor();
#ifdef HAVE_VIDEODEVICE1394
  else if (Video::Parameters::instance()->grabber == "1394")
    pVideoDevice = new  Video::VideoDevice1394();
#endif // HAVE_VIDEODEVICE1394
  else if (Video::Parameters::instance()->grabber == "dummy")
    pVideoDevice = new  Video::VideoDeviceDummy();
  else
    throw Miro::Exception(std::string("VideoDevice::constructor: unknown device: ") + 
			  Video::Parameters::instance()->grabber);
  pConsumer = new Video::Consumer(*pVideoDevice, &schedparams_);
  pGrabber = new Miro::VideoImpl(pConsumer);

  pVideoDevice->connect();
  pConsumer->open(NULL);

  // register the grabber interface at the POA
  pVideo = pGrabber->_this();

  addToNameService(pVideo, "Video");
}

VideoService::~VideoService()
{
  DBG(cout << "Destructing VideoService." << endl);

  pConsumer->cancel();
  pVideoDevice->disconnect();

  // Deactivate the interfaces.
  // we have to do this manually for none owned orbs,
  // as the class goes out of scope before
  // the orb is shut down
  PortableServer::ObjectId_var oid;
  oid =  poa->reference_to_id (pVideo);
  poa->deactivate_object (oid.in());

  delete pGrabber;
  delete pConsumer;
  delete pVideoDevice;
}
