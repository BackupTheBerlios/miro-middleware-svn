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
#ifndef videoService_hh
#define videoService_hh

#include "miro/Server.h"

#include "video/VideoConnection.h"
#include "video/VideoConsumer.h"
#include "video/VideoImpl.h"

// forward declarations
namespace Video
{
  class Parameters;
};

class VideoService : public Miro::Server
{
  typedef Miro::Server Super;

  // = DESCRIPTION
  //    This class starts up the ServiceImpl registers it at the
  //    naming service and runs the orb

public:
  // Initialization and Termination methods.

  // Constructor
  VideoService(int argc, char *argv[]);
  VideoService(Server& _server);

  // Destructor.
  ~VideoService();

  void init();

private:
  /** Sceduling parameters for a realtime thread */
  ACE_Sched_Params schedparams_;
  Video::Consumer consumer;
  Video::Connection connection;
  Miro::VideoImpl grabber;
};
#endif




