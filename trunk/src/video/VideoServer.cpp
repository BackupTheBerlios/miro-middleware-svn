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
#include "VideoParameters.h"

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

VideoService::VideoService(int argc, char *argv[],
			   const Miro::RobotParameters& _robotParameters,
			   const Video::Parameters& _videoParms) :
  Super(argc, argv, _robotParameters),

  // Video board initialization
  pConsumer(new Video::Consumer(connection /*, &grabber */)),
  connection(pConsumer, _videoParms),
  grabber(pConsumer, _videoParms)

  // Service initialization
{
  init();
  DBG(cout << "VideoService initialized.." << endl);
}

VideoService::VideoService(Server& _server,
			   const Video::Parameters& _videoParms) :
  Super(_server),

  // Video board initialization
  pConsumer(new Video::Consumer(connection /*, &grabber */)),
  connection(pConsumer, _videoParms),
  grabber(pConsumer, _videoParms)

  // Service initialization
{
  init();
  DBG(cout << "VideoService initialized.." << endl);
}

void
VideoService::init()
{
  // register the grabber interface at the POA
  Miro::Video_var pGrabber = grabber._this();

  addToNameService(pGrabber.in(), "Video");
}

VideoService::~VideoService()
{
  DBG(cout << "Destructing VideoService." << endl);
}
