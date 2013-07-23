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
#ifndef panTiltServer_h
#define panTiltServer_h

#include "miro/Server.h"
#include "miro/ReactorTask.h"

#include "PanTiltImpl.h"

namespace DpPanTilt
{
  /** 
   * This class starts up the PanTiltImpl registers it at the
   * naming service and runs the orb.
   */
  class PanTiltServer : public Miro::Server
  {
    typedef Miro::Server Super;

  public:
    // Initialization and Termination methods.
    /*
     * Constructor.
     */
    PanTiltServer(int& argc, char *argv[], Miro::PanTiltParameters panTiltParameters);

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
}
#endif // panTiltServer_h




