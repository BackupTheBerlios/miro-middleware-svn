// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef videoService_hh
#define videoService_hh

#include "miro/Server.h"

#include "VideoImpl.h"

// forward declarations
namespace Video
{
  class Parameters;
  class VideoDevice;
  class Consumer;
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
  Video::VideoDevice * pVideoDevice;
  Video::Consumer * pConsumer;
  Miro::VideoImpl * pGrabber;
  Miro::Video_ptr pVideo;
};
#endif




