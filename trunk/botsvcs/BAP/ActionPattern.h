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
#ifndef BAP_ActionPattern_h
#define BAP_ActionPattern_h

#include "Pattern.h"
#include "Policy.h"
#include "ArbiterParameters.h"

#include "miro/Synch.h"

#include <orbsvcs/CosNotifyCommC.h>

#include <map>
#include <string>
#include <iostream>

namespace Miro
{
  // forward declarations
  class StructuredPushSupplier;

  namespace BAP
  {
    // forward declarations
    class Policy;
    class Behaviour;
    class BehaviourParameters;
    class Arbiter;
    class ActionPattern;

    //! Class representing an ation pattern.
    /** 
     * This class represents an action pattern, which consists of a set of 
     * behaviours, an arbiters and their parameters.
     *
     * @author Ingmar Baetge
     * @author Hans Utz
     */
    class ActionPattern : public Pattern
    {
      //------------------------------------------------------------------------
      // private types
      //------------------------------------------------------------------------
    
      //! Type of the super class.
      typedef Pattern Super;

    public:
      //------------------------------------------------------------------------
      // public types
      //------------------------------------------------------------------------

      //! A pair of a behaviour and its parameter set.
      typedef std::pair<Behaviour *, BehaviourParameters *> BehaviourPair;
      //! A map matching behaviours by name.
      typedef std::map<std::string, BehaviourPair> BehaviourMap;

      //------------------------------------------------------------------------
      // public methods
      //------------------------------------------------------------------------

      //! Initializing constructor.
      ActionPattern(const std::string& _name, 
		    StructuredPushSupplier * _pSupplier = NULL);
      //! Initializing constructor.
      ActionPattern(const ActionPattern& _rhs);
      //! Virtual destructor.
      virtual ~ActionPattern();
      //! Virtual copy constructor.
      virtual ActionPattern * clone() const;
      
      //! Parse DOM node
      //! Initialize the action pattern from an XML description.
      void xmlInit(const QDomElement& _element);
      
      //! Add a behaviour and its parameter set to the action pattern.
      void addBehaviour(Behaviour * _behaviour, 
			BehaviourParameters * _parameters);
      //! Get the parameters of an existing behaviour.
      virtual 
      BehaviourParameters * const 
      getBehaviourParameters(std::string const& _pattern,
			     std::string const& _behaviour) const
	throw (BehaviourEngine::EUnknownActionPattern,
	       BehaviourEngine::EUnknownBehaviour);

      //! Set the parameters of an existing behaviour.
      virtual 
      void
      setBehaviourParameters(std::string const& _pattern,
			     std::string const& _behaviour,
			     BehaviourParameters * _parameters)
	      throw (BehaviourEngine::EUnknownActionPattern,
		     BehaviourEngine::EUnknownBehaviour);

      //! Set the arbiter of the action pattern.
      void arbiter(Arbiter * _a, ArbiterParameters * _p);
      //! Lookup a behaviour of the action pattern by name.
      Behaviour * getBehaviour(const std::string &);
      
      //! Send a transition message, switching to another action pattern.
      void sendTransitionMessage(const std::string& message);

      //! Send a transition message, switching to another action pattern.
      /** none thread safe version. */
      void protectedSendTransitionMessage(const std::string& message);

      //! Get the arbiter.
      Arbiter * arbiter();
      
      //! Activate the action pattern.
      void open(Pattern * const _predecessor,
		std::string const& _transition);
      //! Deactivate the action pattern.
      void close(ActionPattern * const _successor);

   protected:
      //------------------------------------------------------------------------
      // protected methods
      //------------------------------------------------------------------------

      //! Retrieve the current action pattern.
      virtual Pattern * const currentPattern();
      virtual ActionPattern * currentLeafPattern();
      
      //! Dump action pattern configuration to ostream for debugging.
      virtual void printToStream(std::ostream& ostr) const;
      
      const BehaviourMap& behaviourMap() const;

      //------------------------------------------------------------------------
      // protected data
      //------------------------------------------------------------------------

      //! Event supplier for online output.
      /** Default is the NULL pointer. */
      StructuredPushSupplier * pSupplier_;
      //! The arbiter of the action pattern.
      /** Initialized to NULL */
      Arbiter * arbiter_;
      //! The parameters of the arbiter of the action pattern.
      /** Initialized to NULL */
      ArbiterParameters * arbiterParameters_;
      //! Map of the behaviours and their parameters.
      BehaviourMap behaviourMap_;

      //! Preconfigured event to send to the notification service.
      CosNotification::StructuredEvent notifyEvent;

    private:      
      //------------------------------------------------------------------------
      // hidden methods
      //------------------------------------------------------------------------
      ActionPattern& operator=(ActionPattern const&);

      //------------------------------------------------------------------------
      // friend declarations
      //------------------------------------------------------------------------

      friend std::ostream& operator << (std::ostream& ostr, const ActionPattern&);
      friend class Policy;
    };

    //--------------------------------------------------------------------------
    // inline methods
    //--------------------------------------------------------------------------

    inline 
    void
    ActionPattern::arbiter(Arbiter * _a, ArbiterParameters * _p) {
      _p->pattern = this;
      
      arbiter_ = _a;
      arbiterParameters_ = _p;
    }

    inline 
    Arbiter *
    ActionPattern::arbiter() {
      return arbiter_;
    }

    inline
    const ActionPattern::BehaviourMap&
    ActionPattern::behaviourMap() const {
      return behaviourMap_;	
    }

    inline
    Pattern * const
    ActionPattern::currentPattern() {
      return parent_->currentPattern();
    }
  }
}
#endif
