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
#ifndef BAP_Behaviour_h
#define BAP_Behaviour_h

#include "BehaviourParameters.h"
#include "VelocitySpace.h"
#include "ActionPattern.h"
#include "Arbiter.h"

#include <string>

#define BEHAVIOUR_PARAMETERS_FACTORY(X) \
    virtual X * getParametersInstance() const; \
    virtual X * getParametersInstance(const ::Miro::BAP::BehaviourParameters& _p) const

#define BEHAVIOUR_PARAMETERS_FACTORY_IMPL(C, X) \
  X * \
  C::getParametersInstance() const \
  { \
    return new X(); \
  } \
  X * \
  C::getParametersInstance(const ::Miro::BAP::BehaviourParameters& _p) const \
  { \
    const X& p= dynamic_cast<const X&>(_p); \
    return new X(p); \
  }

namespace Miro
{
  // forward declarations
  class Parameter;

  namespace BAP
  {
    // forward declarations
    class BehaviourParameters;
    class ArbiterMessage;
    
    //! Base class of all behaviours.
    /**
     * @author Ingmar Baetge
     * @author Hand Utz
     */
    class Behaviour 
    {
    public:
      //------------------------------------------------------------------------
      // public methods
      //------------------------------------------------------------------------

      //! Default constructor.
      Behaviour();
      //! Virtual destructor.
      virtual ~Behaviour();
      
      //! Factory method for BehaviourParameters.
      virtual BehaviourParameters * getParametersInstance() const;
      //! Factory method for BehaviourParameters.
      virtual BehaviourParameters * getParametersInstance(const BehaviourParameters& _p) const;
      
      //! Bring the behaviour in active state.
      virtual void open();
      //! Terminate the active state of the behaviour.
      virtual void close();
      //! Initialize the behaviour with a new parameter set.
      virtual void init(const BehaviourParameters * _params);
      
      //! Callback for the actual implementation of the behaviour.
      virtual void action();
      //! Callback for the default implementation of the action callback.
      virtual std::string actionTake2(BehaviourParameters const * _params,
				      ArbiterMessage * _message);
      
      //! Return the name of the behaviour.
      /**
       * Deprecated.
       */
      virtual const std::string& getBehaviourName() const = 0;
      //! Return the name of the behaviour.
      /**
       * For a policy to be constructed correctly all behaviours within 
       * the behaviour repository have to report a unigue name by this method.
       *
       * The default implementation calls the deprecated version, in order
       * to keep backward compability.
       */
      virtual const std::string& getName() const { return getBehaviourName(); }

      //! Returns true if the behaviour is in active state.
      bool isActive() const;
      
      //! Hook for the ConstraintArbiter
      /** @todo replace by a method pointer within the arbitration message. */
      virtual void addEvaluation(VelocitySpace *);
      
      //! Initiate a memory cleanup during dynamic policy reconfiguration.
      /**
       * When altering the parameter set of an active behaviour, 
       * we can not delete the parameters until deactivation of the 
       * behaviour to avoid race conditions.
       */
      void deleteParametersOnTransition();

    protected:

      //------------------------------------------------------------------------
      // protected methods
      //------------------------------------------------------------------------
      //! Send a transition message to the policy.
      void sendMessage(const std::string& message);
      //! Send an arbitration message to the Arbiter of the ActionPattern.
      void arbitrate(const ArbiterMessage& message);
    
      //! Get the parameter set of an existing behaviour within an actionpattern in the transition set.
      BehaviourParameters * const 
      getSuccessorBehaviourParameters(const std::string& _transition,
				      const std::string& _behaviour) const;
      //! Set the parameters of an existing behaviour within an actionpattern in the transition set.
      void 
      setSuccessorBehaviourParameters(const std::string& _transition,
				      const std::string& _behaviour, 
				      BehaviourParameters * _parameters);
      

      //------------------------------------------------------------------------
      // protected data
      //------------------------------------------------------------------------

      //! Flag indicating the behaviour to be active.
      /** Default is false. */
      bool active_;
      //! Parameter set of the behaviour.
      /** 
       * This is exchanged on a transition to another action pattern.
       * Default is the NULL-Pointer. 
       */
      const BehaviourParameters * params_;
      //! Hold the old parameters for cleanup on dynamic policy reconfiguartion.
      /**
       * Default is NULL. 
       *
       * TODO: Check if this rather has to be a vector...
       */
      const BehaviourParameters * oldParams_;
    };
    
    //--------------------------------------------------------------------------
    // inline methods
    //--------------------------------------------------------------------------

    inline
    bool 
    Behaviour::isActive() const { 
      return active_; 
    };
    
    inline
    void
    Behaviour::deleteParametersOnTransition() {
      oldParams_ = params_;
    }
    
    inline
    void
    Behaviour::arbitrate(const ArbiterMessage& message) {
      params_->pattern->arbiter()->arbitrate(message);
    }
  }
}
#endif // BAP_Behaviour_h
