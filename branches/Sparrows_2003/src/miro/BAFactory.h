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
#ifndef BAFactory_hh
#define BAFactory_hh

#include <vector>

namespace Miro
{
  class TimedBehaviour;
  class EventBehaviour;
  class TaskBehaviour;
  class Arbiter;

  class BAFactory
  {
  public:
    // Initialization and Termination methods.
    BAFactory();
    // Constructor.
    
    virtual ~BAFactory();
    // Destructor.
    
    virtual void init();
    virtual void connect();
    virtual void disconnect();

  protected:
    typedef std::vector<Miro::TimedBehaviour *> TimedBehaviourVector;
    typedef std::vector<Miro::EventBehaviour *> EventBehaviourVector;
    typedef std::vector<Miro::TaskBehaviour *> TaskBehaviourVector;
    typedef std::vector<Miro::Arbiter *> ArbiterVector;

    TimedBehaviourVector timedBehaviours_;
    EventBehaviourVector eventBehaviours_;
    TaskBehaviourVector taskBehaviours_;

    ArbiterVector arbiters_;
  };
};
#endif




