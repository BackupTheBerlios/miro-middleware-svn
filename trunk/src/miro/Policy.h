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
#ifndef Policy_h
#define Policy_h

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

    Policy(StructuredPushSupplier * _pSupplier = NULL);
    //! Initializing constructor.
    Policy(const char* patternfile, StructuredPushSupplier * _pSupplier = NULL);
    //! Virtual destructor.
    virtual ~Policy();

    void loadPolicyFile(const char* _fileName);
    void loadPolicy(const char * _policy);
    void parsePolicy(const QDomDocument& _doc);

    //! Register an action pattern at the policy.
    void registerActionPattern(ActionPattern * _pattern);
    //! Lookup action pattern by name.
    ActionPattern* getActionPattern(const std::string& _name);
    void setStartPattern(ActionPattern * _pattern);

    //! Start the policy.
    void open();
    //! End the policy.
    void close();
    //! Clear the policy.
    void clear();
    //! Is this a valid policy?
    bool valid() const;

  protected:
    //! Dump the policy configuration to an output stream.
    virtual void printToStream(std::ostream& ostr) const;

    //! Event supplier for online output.
    StructuredPushSupplier * pSupplier_;
    //! The action pattern to start with.
    ActionPattern * startPattern_;

    //! Map of registered action patterns, sorted by name.
    ActionPatternMap actionPatterns_;

    bool valid_;

    friend
    std::ostream& operator << (std::ostream& ostr, const Policy&);
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
};
#endif

