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
#ifndef Pattern_h
#define Pattern_h

#include "miro/Synch.h"
#include "idl/BehaviourEngineC.h"

#include <map>
#include <string>
#include <iostream>

// forward declarations
class QDomElement;

namespace Miro
{
  //! Namespace of the Behaviour ActionPattern Policy framework.
  namespace BAP
  {
    // forward declarations
    class Pattern;
    class Policy;
    class ActionPattern;
    class BehaviourParameters;

    //! Ostream operator for debug purposes.
    std::ostream& operator << (std::ostream& ostr, const Pattern&);
    
    //! Class representing an ation pattern.
    /** 
     * This class represents a pattern, which can be subclassed
     * into a Policy or an ActionPattern
     *
     * @author Hans Utz
     */
    class Pattern 
    {
    public:
      //------------------------------------------------------------------------
      // public types
      //------------------------------------------------------------------------

      //! A map matching transitions to the following Pattern.
      typedef std::map<std::string, Pattern *> PatternMap;

      //------------------------------------------------------------------------
      // public methods
      //------------------------------------------------------------------------

      //! Initializing constructor.
      Pattern(Policy * const _parent = NULL,
	      const std::string& _name = std::string());
      //! Copy constructor.
      Pattern(Pattern const& _rhs);
      //! Virtual destructor.
      virtual ~Pattern();
      //! Virtual copy constructor.
      virtual Pattern * clone() const = 0;
      //! Clear the policy.
      virtual void clear();

      //! Parse DOM node
      //! Parse DOM transition node
      virtual void xmlInit(const QDomElement& _element) = 0;
      virtual void xmlAddTransition(QDomElement _element);

      //! Activate the pattern.
      virtual void open(Pattern * const _predecessor,
			std::string const& _transition) = 0;
      //! Deactivate the action pattern.
      virtual void close(ActionPattern * const _successor = NULL) = 0;

      //! Set the name of the pattern.
      void setName(std::string const& _name);
      //! Query the name of the pattern.
      std::string const& getName() const;
      //! Query the name of the pattern, including the path of parent patterns.
      std::string getFullName() const;

      //! Set the parent of the pattern.
      void parent(Policy * const _parent);
      //! Query the parent of the pattern.
      Policy * const parent() const;

      //! Send a transition message, switching to another action pattern.
      virtual void sendTransitionMessage(const std::string& _message);

      //! Add a transition to another pattern.
      void addTransition(const std::string&, Pattern *);
      //! Retrieve the associtated successor pattern for the transition message.
      Pattern * const getTransitionPattern(const std::string & _message);

      //! Retrieve the current action pattern.
      virtual Pattern * const currentPattern() = 0;
      ActionPattern * currentActionPattern();
      Pattern * rootPattern();
      Pattern const * rootPattern() const;

      //! Get the parameter set of an existing behaviour within an actionpattern.
      virtual 
      BehaviourParameters * const 
      getBehaviourParameters(const std::string& _pattern,
			     const std::string& _behaviour) const 
	throw (BehaviourEngine::EUnknownActionPattern,
	       BehaviourEngine::EUnknownBehaviour) = 0;
      //! Set the parameters of an existing behaviour within an actionpattern.
      /** Nested behaviours are addressed by <Subpolicy>/<Pattern> */
      virtual
      void 
      setBehaviourParameters(const std::string& _pattern,
			     const std::string& _behaviour, 
			     BehaviourParameters * _parameters) 
	throw (BehaviourEngine::EUnknownActionPattern,
	       BehaviourEngine::EUnknownBehaviour) = 0;

      Miro::Mutex& transitionMutex() const;

    protected:
      //------------------------------------------------------------------------
      // protected types
      //------------------------------------------------------------------------

      class Indent 
      {
      public:
	Indent() { indent_ += STEP; }
	~Indent() { indent_ -= STEP; }

	static char const * const ws();

      protected:
	static unsigned int indent_;

	static unsigned int const STEP = 2;
	static unsigned int const WS_LEN = 80;
	static char const WHITE_SPACE[WS_LEN + 1];
      };

      //------------------------------------------------------------------------
      // protected methods
      //------------------------------------------------------------------------

      //! Search leave pattern.
      virtual ActionPattern * currentLeafPattern() = 0;

      //! Dump action pattern configuration to ostream for debugging.
      virtual void printToStream(std::ostream& ostr) const;

      //------------------------------------------------------------------------
      // protected data
      //------------------------------------------------------------------------

      //! The parent pattern.
      /** 
       * Parents can only be policies and no action patterns.
       * Therefore we needn't use the abstract base type. 
       */
      Policy * parent_;
      //! The name of the action pattern.
      std::string patternName_;
      //! Table of transitions.
      PatternMap transitionTable_;


    private:
      //------------------------------------------------------------------------
      // private data
      //------------------------------------------------------------------------

      //! Mutex for synchronising transitions.
      mutable Mutex transitionMutex_;

      //------------------------------------------------------------------------
      // hidden methods
      //------------------------------------------------------------------------
      Pattern& operator=(Pattern const&);

      //------------------------------------------------------------------------
      // friend declarations
      //------------------------------------------------------------------------
      friend class Policy;
      friend std::ostream& operator << (std::ostream& ostr, const Pattern&);
    };


    //--------------------------------------------------------------------------
    // inline methods
    //--------------------------------------------------------------------------
    inline
    const std::string&
    Pattern::getName() const {
      return patternName_;
    }

    inline
    void
    Pattern::setName(std::string const& _name) {
      patternName_ = _name;
    }

    inline
    Policy * const
    Pattern::parent() const {
      return parent_;
    }

    inline
    void
    Pattern::parent(Policy * const _parent) {
      parent_ = _parent;
    }
    inline
    ActionPattern *
    Pattern::currentActionPattern() {
      return rootPattern()->currentLeafPattern();
    }
    inline
    Miro::Mutex&
    Pattern::transitionMutex() const {
      Pattern const * root = rootPattern();
      return root->transitionMutex_;
    }
  }
}
#endif
