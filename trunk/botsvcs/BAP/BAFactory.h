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
#ifndef BAP_BAFactory_h
#define BAP_BAFactory_h

#include <vector>

namespace Miro
{
  namespace BAP
  {
    class TimedBehaviour;
    class EventBehaviour;
    class TaskBehaviour;
    class Arbiter;
 
    //! Helper class for behaviour and arbiter instance management.
    /**
     * Behaviours and arbiters have to be added to the respective
     * repositories. Also, event behaviours might be constructed 
     * unconnected, to prevent subtle deadlocks in the colocated 
     * scenario. This can be all done by hand, but this class -
     * used as a base class for behaviour and arbiter factories.
     *
     * Create the instances of the behaviours in the constructor,
     * adding the factory base class, using the overloaded add method.
     *
     */   
    class BAFactory
    {
    public:
      // Initialization and Termination methods.
      BAFactory();
      // Constructor.
      
      virtual ~BAFactory();
      // Destructor.
      
      void init();
      void fini();
      void connect();
      void disconnect();

    protected:
      void add(TimedBehaviour * _behaviour);
      void add(EventBehaviour * _behaviour);
      void add(TaskBehaviour * _behaviour);
      void add(Arbiter * _arbiter);

      typedef std::vector<TimedBehaviour *> TimedBehaviourVector;
      typedef std::vector<EventBehaviour *> EventBehaviourVector;
      typedef std::vector<TaskBehaviour *> TaskBehaviourVector;
      typedef std::vector<Arbiter *> ArbiterVector;
      
      TimedBehaviourVector timedBehaviours_;
      EventBehaviourVector eventBehaviours_;
      TaskBehaviourVector taskBehaviours_;
      
      ArbiterVector arbiters_;
    };
  }
}
#endif




