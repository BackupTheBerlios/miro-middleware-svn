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
#ifndef Policy_h
#define Policy_h

#include "Synch.h"

#include <string>
#include <map>
#include <iostream>

// Forward declaration.
class QDomDocument;

namespace Miro
{
  // forward declarations
  class ArbiterRepository;
  class BehaviourRepository;
  class ActionPattern;
  class KeyValueList;
  class Policy;
  class StructuredPushSupplier;
  class BehaviourParameters;

  //! Ostream operator for debug purposes.
  std::ostream& operator << (std::ostream& ostr, const Policy&);

  //! Class representing a policy.
  /**
   * A policy is a full configuration of the behaviour engine
   * of Miro.
   */
  class Policy 
  {
  public:
    //! Mapping of names to action pattern instances.
    typedef std::map<std::string, ActionPattern*> ActionPatternMap;

    //! Default constructor.
    Policy(StructuredPushSupplier * _pSupplier = NULL);
    //! Initializing constructor.
    Policy(const char* _patternfile, StructuredPushSupplier * _pSupplier = NULL);
    //! Virtual destructor.
    virtual ~Policy();

    //! Load a policy from disc.
    void loadPolicyFile(const char* _fileName);
    //! Load a policy from a string.
    void loadPolicy(const char * _policy);
    //! Parse a policy XML document.
    void parsePolicy(const QDomDocument& _doc);

    //! Register an action pattern at the policy.
    void registerActionPattern(ActionPattern * _pattern);
    //! Lookup action pattern by name.
    ActionPattern* getActionPattern(const std::string& _name);
    //! Lookup action pattern by name.
    ActionPattern const * const getActionPattern(const std::string& _name) const;
    //! Set the default pattern for open.
    void setStartPattern(ActionPattern * _pattern);
    //! Get the parameter set of an existing behaviour within an actionpattern.
    BehaviourParameters * const getBehaviourParameters(const std::string& _pattern,
						       const std::string& _behaviour) const;
    //! Set the parameters of an existing behaviour within an actionpattern.
    void setBehaviourParameters(const std::string& _pattern,
				const std::string& _behaviour, 
				BehaviourParameters * _parameters);

    //! Retrieve the current action pattern.
    ActionPattern * const currentActionPattern();

    //! Start the policy.
    void open();
    //! Start a specific patern.
    void openActionPattern(const std::string& _pattern);
    //! End the policy.
    void close();
    //! Clear the policy.
    void clear();
    //! Is this a valid policy?
    bool valid() const;
    //! Set validity of the policy (for debugging only!).
    void valid(bool flag);

  protected:
    //! Set the current action pattern.
    void currentActionPattern(ActionPattern * _pattern);

    //! Dump the policy configuration to an output stream.
    virtual void printToStream(std::ostream& ostr) const;

    //! Event supplier for online output.
    /** Default is the NULL pointer. */
    StructuredPushSupplier * pSupplier_;
    //! The action pattern to start with.
    /**
     * Default is the NULL pointer. A policy is not valid
     * if no startPattern_ is specified.
     */
    ActionPattern * startPattern_;
    //! The currently active action pattern.
    /**
     * Default is the NULL pointer.
     */
    ActionPattern * currentPattern_;

    //! Map of registered action patterns, sorted by name.
    ActionPatternMap actionPatterns_;

    //! Flag to indicate that the object contains a valid configuration.
    /** Default is false. */
    bool valid_;

    //! Mutex for synchronising transitions.
    Mutex transitionMutex_;

    friend
    std::ostream& operator << (std::ostream& ostr, const Policy&);

    friend class ActionPattern;
  };

  inline
  void
  Policy::setStartPattern(ActionPattern * _pattern) {
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
  ActionPattern * const
  Policy::currentActionPattern() {
    return currentPattern_;
  }
  inline
  void
  Policy::currentActionPattern(ActionPattern * _pattern)
 {
    currentPattern_ = _pattern;
  }
};
#endif

