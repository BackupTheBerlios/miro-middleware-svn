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
  Video::Consumer * pConsumer;
  Video::Connection connection;

  // Steffen:

  // This is your sample interface
  // It doesn't exist by now, therefor commented out
  Miro::VideoImpl grabber;
};
#endif




