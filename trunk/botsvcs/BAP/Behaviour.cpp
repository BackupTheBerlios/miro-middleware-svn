// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "Behaviour.h"
#include "BehaviourParameters.h"
#include "ActionPattern.h"
#include "ArbiterMessage.h"

#include "idl/BehaviourEngineC.h"
#include "miro/Log.h"

namespace Miro 
{
  namespace BAP
  {
    /**
     * Initializing the data members to their defaults.
     */
    Behaviour::Behaviour() :
      active_(false),
      params_(NULL),
      oldParams_(NULL)
    {
      MIRO_LOG_CTOR("Behaviour");
    }

    /**
     * Any class with virtual functions needs a virtual destructor.
     * Provided to avoid errors in derived classes.
     */
    Behaviour::~Behaviour()
    {
      MIRO_LOG_DTOR("Behaviour");
      if (active_) {
	MIRO_LOG_OSTR(LL_ERROR, "Behaviour " << getName() << " still active on destruction.");
      }

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

    /**
     * The default implementation calls the method actionTake2().
     * This method has an ArbiterMessage in/out parameter.
     * On input it contains the arbitration message of the next lower
     * priority behaviour that is active at the arbiter, or a default
     * message if no such behaviuor exists.
     * The two return values are the arbiter message and a transition
     * message string. The arbitration message is passed on to the arbiter,
     * if the transition message string is none null, the transition
     * message is sent.
     */
    void
    Behaviour::action()
    {
      // we need a local copy to keep up thread safety
      BehaviourParameters const * params = params_;

      ArbiterMessage * message =
	params->pattern->arbiter()->getMessageForBehaviour(this);
      std::string transition = actionTake2(params, message);
      params_->pattern->arbiter()->arbitrate(*message);
      if (transition.length() != 0)
	params->pattern->sendTransitionMessage(transition);

      delete message;
    }

    std::string
    Behaviour::actionTake2(BehaviourParameters const *,
			   ArbiterMessage * )
    {
      return std::string();
    }

    void 
    Behaviour::sendMessage(const std::string& message) 
    {
      params_->pattern->sendTransitionMessage(message);
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
     * struct. The caller takes ownership of the BehaviourParameters
     * object.
     */
    BehaviourParameters * const
    Behaviour::getSuccessorBehaviourParameters(std::string const& _transition,
					       std::string const& _behaviour) const
    {
      Pattern const * const pattern = 
	params_->pattern->getTransitionPattern(_transition);

      if (pattern) 
	return pattern->getBehaviourParameters("", _behaviour);
    
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
      Pattern * const pattern = 
	params_->pattern->getTransitionPattern(_transition);

      if (pattern) {
	pattern->setBehaviourParameters("", _behaviour, _parameters);
      }
      else 
	throw BehaviourEngine::EUnknownActionPattern();
    }
  }
}
