// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Policy_h
#define Policy_h

#include "Pattern.h"

#include "miro/Synch.h"

#include <string>
#include <iostream>

// Forward declaration.
class QDomDocument;

namespace Miro
{
  // forward declarations
  class StructuredPushSupplier;
  template<class T>
  class Repository;

  namespace BAP
  {
    // forward declarations
    class Arbiter;
    class Behaviour;
    typedef Repository<Arbiter>  ArbiterRepository;
    typedef Repository<Behaviour> BehaviourRepository;
    class ActionPattern;
    class Policy;
    class BehaviourParameters;

    //! Class representing a policy.
    /**
     * A policy is a full configuration of the behaviour engine
     * of Miro.
     */
    class Policy : public Pattern
    {
      //--------------------------------------------------------------------------
      // private types
      //--------------------------------------------------------------------------

      //! Type of the super class.
      typedef Pattern Super;
      //! Type forming a mapping from a (pattern, event) pair to a pattern.
      typedef std::map<std::pair<std::string, std::string>, std::string> TransitionMap;
      //! Type forming a mapping from a event to a pattern.
      typedef std::map<std::string, std::string> TransitionPatternMap;

    public:
      //--------------------------------------------------------------------------
      // static public methods
      //--------------------------------------------------------------------------

      //! Load a policy from disc.
      static void loadPolicyFile(Policy& _policy, char const * _fileName);
      //! Load a policy from a string.
      static void loadPolicy(Policy& _policy, char const * _xml);
      //! Parse a policy XML document.
      static void parsePolicy(Policy& _policy, QDomDocument const& _doc);

      //--------------------------------------------------------------------------
      // public methods
      //--------------------------------------------------------------------------

      //! Default constructor.
      Policy(StructuredPushSupplier * _pSupplier = NULL);
      //! Initializing constructor.
      Policy(QDomElement const& _doc, StructuredPushSupplier * _pSupplier = NULL);
      //! Copy constructor.
      Policy(const Policy& _rhs);
      //! Virtual destructor.
      virtual ~Policy();
      //! Virtual copy constructor.
      virtual Policy * clone() const;

      //! Parse DOM node
      virtual void xmlInit(const QDomElement& _element);
      virtual void xmlAddTransitionPattern(QDomElement _element);

      //! Register a pattern at the policy.
      void registerPattern(Pattern * const _pattern);
      //! Lookup child pattern by name.
      Pattern * const getPattern(const std::string& _name);
      //! Lookup child pattern by name.
      Pattern const * const getPattern(const std::string& _name) const;

      //! Send a transition message, switching to another action pattern.
      virtual void sendTransitionMessage(const std::string& _message);

      //! Set the default pattern for open.
      void setStartPattern(Pattern * _pattern);
      //! Get the parameter set of an existing behaviour within an actionpattern.
      BehaviourParameters * const getBehaviourParameters(const std::string& _pattern,
							 const std::string& _behaviour) const
	throw (BehaviourEngine::EUnknownActionPattern,
	       BehaviourEngine::EUnknownBehaviour);
      //! Set the parameters of an existing behaviour within an actionpattern.
      /** Nested behaviours are addressed by <Subpolicy>/<Pattern> */
      void setBehaviourParameters(const std::string& _pattern,
				  const std::string& _behaviour, 
				  BehaviourParameters * _parameters)
	throw (BehaviourEngine::EUnknownActionPattern,
	       BehaviourEngine::EUnknownBehaviour);

      //! Retrieve the active pattern of the (sub-)policy.
      Pattern * const currentPattern();

      //! Start the policy.
      void open(Pattern * const _predecessor = NULL,
		std::string const& _transition = std::string());
      //! End the policy.
      void close(ActionPattern * const _successor = NULL);

      //! Start a specific patern.
      void openPattern(const std::string& _pattern);
      //! Clear the policy.
      void clear();
      //! Is this a valid policy?
      bool valid() const;
      //! Set validity of the policy (for debugging only!).
      void valid(bool flag);

    protected:
      //--------------------------------------------------------------------------
      // protected methods
      //--------------------------------------------------------------------------

      //! Retrieve the associtated successor for the transition pattern message.
      Pattern * const getTransitionPatternPattern(const std::string& _patternName);

      //! Init the childs transition tables;
      void lowerTransitions();

      //! Set the current action pattern.
      void currentPattern(Pattern * const _pattern);

      virtual ActionPattern * currentLeafPattern();

      //! Dump the policy configuration to an output stream.
      virtual void printToStream(std::ostream& ostr) const;

      //--------------------------------------------------------------------------
      // protected static methods
      //--------------------------------------------------------------------------

      //! Get the next path element of the pattern.
      static void splitPath(std::string const& _path,
			    std::string& _first, std::string& _rest);

      //--------------------------------------------------------------------------
      // protected data
      //--------------------------------------------------------------------------

      //! Event supplier for online output.
      /** Default is the NULL pointer. */
      StructuredPushSupplier * pSupplier_;

      PatternMap patterns_;
      PatternMap transitionPatternTable_;

      TransitionMap transitions_;
      TransitionPatternMap transitionPatterns_;

      //! The action pattern to start with.
      /**
       * Default is the NULL pointer. A policy is not valid
       * if no startPattern_ is specified.
       */
      Pattern * startPattern_;
      //! The currently active action pattern.
      /**
       * Default is the NULL pointer.
       */
      Pattern * currentPattern_;

      //! Flag to indicate that the object contains a valid configuration.
      /** Default is false. */
      bool valid_;

    private:
      //--------------------------------------------------------------------------
      // hidden methods
      //--------------------------------------------------------------------------
      
      Policy& operator= (Policy const&);

      //--------------------------------------------------------------------------
      // friend declarations
      //--------------------------------------------------------------------------

      friend class Pattern;
      friend class ActionPattern;
    };

    //----------------------------------------------------------------------------
    //  inline methods
    //----------------------------------------------------------------------------

    inline
    void
    Policy::setStartPattern(Pattern * _pattern) {
      startPattern_ = _pattern;
    }

    inline
    bool
    Policy::valid() const {
      return valid_;
    }

    inline
    void
    Policy::valid(bool flag) {
      valid_ = flag;
    }

    inline
    Pattern * const
    Policy::currentPattern() {
      return currentPattern_;
    }

    inline
    void
    Policy::currentPattern(Pattern * const _pattern) {
      // std::cout << "policy: " << getName() << " current pattern " << _pattern->getName() << std::endl;
      currentPattern_ = _pattern;
    }
  }
}
#endif

