// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
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
    //! Default constructor.
    Behaviour();
    //! Virtual destructor.
    virtual ~Behaviour();

    //! Factory method for BehaviourParameters.
    virtual BehaviourParameters * getParametersInstance();

    //! Bring the behaviour in active state.
    virtual void open();
    //! Terminate the active state of the behaviour.
    virtual void close();
    //! Initialize the behaviour with a new parameter set.
    virtual void init(const BehaviourParameters * _params);

    //! The actual implementation of the behaviour.
    virtual void action() = 0;

    //! Return the name of the behaviour.
    /**
     * For a policy to be constructed correctly all behaviours within 
     * the behaviour repository have to report a unigue name by this method.
     */
    virtual const std::string& getBehaviourName() const = 0;

    //! Returns true if the behaviour is in active state.
    bool isActive();

    //! Send a transition message to the policy.
    void sendMessage(const std::string& message);
    //! Send an arbitration message to the Arbiter of the ActionPattern.
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
