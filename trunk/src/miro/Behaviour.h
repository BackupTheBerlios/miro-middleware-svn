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
#ifndef Behaviour_h
#define Behaviour_h

#include "DynamicWindow.h"
#include "ActionPattern.h"
#include "Arbiter.h"

#include <string>

namespace Miro
{
  // forward declarations
  class Parameter;
  class BehaviourParameters;
  class ArbiterMessage;

  class Behaviour {
  public:
    /** 
     * Default constructor.
     *
     * Initializing the data members to their defaults.
     */
    Behaviour();
    /** Virtual destructor.
     *
     * Any class with virtual functions needs a virtual destructor.
     * Provided to avoid errors in derived classes.
     */
    virtual ~Behaviour();

    /*
     * Factory method for BehaviourParameters.
     *
     * Returns a pointer to a newly created instance of some derived
     * BehaviourParameters class. The caller takes ownership of the 
     * object.
     */
    virtual BehaviourParameters * getParametersInstance();

    /**
     * Bring the behaviour in active state.
     *
     * Called on activation of the behaviour. isActive() will return
     * true until the next call to close(). Note that when overwriting
     * this method the parents implementation of open() should be called
     * first within the new implementation.
     */
    virtual void open();
    /**
     * Terminate the active state of the behaviour.
     * 
     * Called on deactivation of the behaviour. isActive() will return
     * false until the next call to open().  Note that when overwriting
     * this method the parents implementation of open() should be called
     * at the end of the new implementation.
     */
    virtual void close();
    /**
     * Initialize the behaviour with a new parameter set.
     *
     * Called on each pattern change of a behaviour. If the behaviour
     * is inactive, init is call before open is called. If the
     * behaviour is already active and will be active also in the next
     * ActionPattern, no close()/open() combination will be called,
     * but the behaviours init() method is called concurrently to its
     * action() implementation.
     */
    virtual void init(const BehaviourParameters * _params);
    virtual void action() = 0;

    /**
     * Return the name of the behaviour.
     *
     * For a policy to be constructed correctly all behaviours within 
     * the behaviour repository have to report a unigue name by this method.
     */
    virtual const std::string& getBehaviourName() const = 0;

    /**
     * Returns true if the behaviour is in active state.
     */
    bool isActive();
    void sendMessage(const std::string& message);
    void arbitrate(const ArbiterMessage& message);
    
    virtual void calcDynamicWindow(DynamicWindow *);

  protected:
    /** Default is false. */
    bool active_;
    /** Default is the NULL-Pointer. */
    const BehaviourParameters * params_;
  };

  inline
  bool 
  Behaviour::isActive() { 
    return active_; 
  };

  inline
  void
  Behaviour::arbitrate(const ArbiterMessage& message) {
    ActionPattern::currentActionPattern()->arbiter()->arbitrate(message);
  }
};
#endif
