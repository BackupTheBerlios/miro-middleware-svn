// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/BehaviourEngineC.h"
#include "Behaviour.h"
#include "BehaviourParameters.h"
#include "ActionPattern.h"


namespace Miro 
{
  /**
   * Initializing the data members to their defaults.
   */
  Behaviour::Behaviour() :
    active_(false),
    params_(NULL),
    oldParams_(NULL)
  {}

  /**
   * Any class with virtual functions needs a virtual destructor.
   * Provided to avoid errors in derived classes.
   */
  Behaviour::~Behaviour()
  {
    // clean up dangling behaviour parameters
    // left over from ActionPattern::setBehaviourParameters()    
    delete oldParams_;
    oldParams_ = NULL;
  }

  /**
   * Returns a pointer to a default constructed instance of some derived
   * BehaviourParameters class. The caller takes ownership of the 
   * object.
   */
  BehaviourParameters * 
  Behaviour::getParametersInstance() const
  {
    return new BehaviourParameters();
  }

  /**
   * Returns a pointer to a copy constructed instance of some derived
   * BehaviourParameters class. The caller takes ownership of the 
   * object.
   */
  BehaviourParameters * 
  Behaviour::getParametersInstance(const BehaviourParameters& _p) const
  {
    return new BehaviourParameters(_p);
  }

  /**
   * Called on each pattern change of a behaviour. If the behaviour
   * is inactive, init is call before open is called. If the
   * behaviour is already active and will be active also in the next
   * ActionPattern, no close()/open() combination will be called,
   * but the behaviours init() method is called concurrently to its
   * action() implementation.
   */
  void
  Behaviour::init(const BehaviourParameters * _params)
  {
    params_ = _params;
  }

  /**
   * Called on activation of the behaviour. isActive() will return
   * true until the next call to close(). Note that when overwriting
   * this method the parents implementation of open() should be called
   * first within the new implementation.
   */
  void
  Behaviour::open()
  {
    active_ = true;
  }

  /** 
   * Called on deactivation of the behaviour. isActive() will return
   * false until the next call to open().  Note that when overwriting
   * this method the parents implementation of open() should be called
   * at the end of the new implementation.
   */
  void
  Behaviour::close()
  {
    active_ = false;
  }

  void 
  Behaviour::sendMessage(const std::string& message)
  {
    params_->pattern->sendMessage(this, message);
  }

  void
  Behaviour::addEvaluation(VelocitySpace *) {
  }

  /**
   * This can be done, while the policy is open, and an action pattern
   * is active. And is a first step into dynamic policy configuration.
   *
   * @param _pattern The name of the action pattern.
   * @param _behaviour The name of the behaviour within the action pattern.
   * @return A parameter struct for the behaviour. Note that the type
   * of the parameter struct is the type of the behaviours parameter
   * struct. The calller takes ownership of the BehaviourParameters
   * object.
   */
  BehaviourParameters * const
  Behaviour::getSuccessorBehaviourParameters(const std::string& _transition,
					     const std::string& _behaviour) const
  {
    ActionPattern const * const pattern = 
      params_->pattern->getTransitionPattern(_transition);

    if (pattern) 
      return pattern->getBehaviourParameters(_behaviour);
    
    throw BehaviourEngine::EUnknownActionPattern();
  }

  /**
   * This can be done, while the policy is open, and an action pattern
   * is active. And is a first step into dynamic policy configuration.
   *
   * @param _pattern The name of the action pattern.
   * @param _behaviour The name of the behaviour within the action pattern.
   * @param _parameters A parameter struct for the behaviour. Note
   * that the type of the parameter struct has to match the type
   * expected by the behaviour. The policy takes ownership of the
   * BehaviourParameters object.
   */
  void
  Behaviour::setSuccessorBehaviourParameters(const std::string& _transition,
					     const std::string& _behaviour, 
					     BehaviourParameters * _parameters) 
  {
    ActionPattern * const pattern = 
      params_->pattern->getTransitionPattern(_transition);

    if (pattern) {
      pattern->setBehaviourParameters(_behaviour, _parameters);
    }
    else 
      throw BehaviourEngine::EUnknownActionPattern();
  }
};
