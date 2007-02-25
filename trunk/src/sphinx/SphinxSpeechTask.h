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
#ifndef SphinxSpeechTask_hh
#define SphinxSpeechTask_hh

#include "miro/Task.h"
#include "miro/Log.h"

#include <string>
#include <list>

namespace Miro 
{

  class SphinxSpeechImpl;

  /**
   * handles complete packages as assembled by laserEvent
   * gets the packages from its message queue, where 
   * laserEvent puts them.
   * keeps statistics about corrupted packages
   * uses the reference to LaserImpl to signal the data
   * received via condition variables
   */
  class SphinxSpeechTask : public Task, public Log
  {
    /**
     * main super class
     */
    typedef Task Super;

  public:
    /**
     * Constructor
     */
    SphinxSpeechTask(SphinxSpeechImpl * _speechImpl);

    /**
     * Destructor
     */
    virtual ~SphinxSpeechTask();

    /**
     * this handles one packet that is already decoded from a message
     * block, this used to call this from outside, if we do not run in
     * thread, but for convenience we use it in the threaded approach
     * also
     */
    //    void doPkt(LaserMessage *data);

    /**
     * this is the task main loop, which defines the work to be done
     * this is worked upon on the number of threads that are created
     * inside this task
     */
    virtual int svc();

    //sphinx

    void addLm(std::string lmFileName, std::string lmName);
    void setLm(std::string lmName);
    std::string getLm();


    static bool done;

  protected:
    SphinxSpeechImpl * speechImpl;

    //sphinx
    std::list<std::string> newLmFileNames;
    std::list<std::string> newLmNames;
    std::list<std::string> lmNames;
    
    std::string currentLm;
    std::string newLm;

    /**
     * Laser message interface: the asynchronously called handler
     * does the (hardware specific decoding of the message
     */ 
    //    virtual void handleMessage(const LaserMessage& message);

  protected:
  };
};
#endif




