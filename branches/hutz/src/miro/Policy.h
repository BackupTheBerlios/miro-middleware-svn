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
#ifndef Policy_h
#define Policy_h

#include <string>
#include <map>
#include <iostream>

// forward declarations
class QDomNode;

namespace Miro
{
  // forward declarations
  class ArbiterRepository;
  class BehaviourRepository;
  class ActionPattern;
  class KeyValueList;
  class Policy;
  class StructuredPushSupplier;

  std::ostream& operator << (std::ostream& ostr, const Policy&);

  class Policy 
  {
  public:
    typedef std::map<std::string, ActionPattern*> ActionPatternMap;

    Policy(const char* patternfile, StructuredPushSupplier * _pSupplier = NULL);
    virtual ~Policy();

    void registerActionPattern(ActionPattern *);
    ActionPattern* getActionPattern(const std::string& name);

    void open();
    void close();

  protected:
    void loadActionPatterns(const char*);
    void retrieveBehaviours(ActionPattern& _actionPattern, const QDomNode&);
    void retrieveArbiters(ActionPattern& _actionPattern, const QDomNode&) ;
    void retrieveBehaviourParameters(KeyValueList& _params, const QDomNode&);
    void retrieveTransitions(ActionPattern& _actionPattern, const QDomNode&);

    virtual void printToStream(std::ostream& ostr) const;

    StructuredPushSupplier * pSupplier_;
    BehaviourRepository * behaviourRepository_;
    ArbiterRepository * arbiterRepository_;
    ActionPattern * startPattern_;

    ActionPatternMap actionPatterns_;

    friend
    std::ostream& operator << (std::ostream& ostr, const Policy&);
  };
};
#endif

