// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
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




