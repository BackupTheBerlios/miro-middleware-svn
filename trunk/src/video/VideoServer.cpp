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
  consumer(connection, NULL, &schedparams_),
  connection(&consumer),
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
  consumer(connection, NULL, &schedparams_),
  connection(&consumer),
  grabber(&consumer)

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
