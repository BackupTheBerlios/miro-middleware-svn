// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef ImageServer_h
#define ImageServer_h

#include "miro/Server.h"
#include "miro/ReactorTask.h"

#include "ImageImpl.h"

#include "miro/OdometryC.h"

namespace Miro {
  class ImageServer : public Server
  {
    typedef ImageServer self;

    // = DESCRIPTION
    //    This class starts up the ImageImpl registers it at the
    //    naming service and runs the orb

  public:
    // Initialization and Termination methods.
    ImageServer(int argc, char *argv[]);
    // Constructor.

    ~ImageServer();
    // Destructor.

    virtual void shutdown();

  private:
    /**
     * image connection (keep this member after the reactor !!)
     */
    ImageImpl *image;

    /**
     * corba reference to base for position stamps
     */
    Odometry_var odometry;
    bool positionStamps;


  };
};
#endif




