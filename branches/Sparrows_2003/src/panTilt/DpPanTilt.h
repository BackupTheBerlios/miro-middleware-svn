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
#ifndef panTiltServer_hh
#define panTiltServer_hh

#include "miro/Server.h"
#include "miro/ReactorTask.h"

#include "PanTiltImpl.h"

namespace DpPanTilt
{
  /** 
   * This class starts up the PanTiltImpl registers it at the
   * naming service and runs the orb.
   */
  class PanTiltServer : public Miro::Server, 
			public Miro::Log
  {
    typedef PanTiltServer Self;

  public:
    // Initialization and Termination methods.
    /*
     * Constructor.
     */
    PanTiltServer(int argc, char *argv[]);

    /**
     * Destructor.
     */
    ~PanTiltServer();

  protected:
    /**
     * we have put the reactor that runs in seperate task, 
     * for providing concurrency.
     */
    Miro::ReactorTask reactorTask;

    Data data_;
    EventHandler * pEventHandler_;
    Connection connection_;
    
    /**
     * panTilt connection (keep this member after the reactor !!)
     */
    PanTiltImpl *panTilt;
  };
};
#endif




