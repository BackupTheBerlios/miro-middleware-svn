// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "Pattern.h"
#include "ActionPattern.h"

#include "idl/BehaviourEngineC.h"

#include "Behaviour.h"
#include "BehaviourParameters.h"
#include "BehaviourRepository.h"
#include "ArbiterRepository.h"

#include "miro/StructuredPushSupplier.h"

#include <qdom.h>


namespace Miro
{
  namespace BAP
  {
    unsigned int Pattern::Indent::indent_ = 0;
    unsigned int const Pattern::Indent::WS_LEN;

    char const Pattern::Indent::WHITE_SPACE[WS_LEN + 1] = 
    "                                                                                ";
    char const * const 
    Pattern::Indent::ws() 
    { 
      unsigned int i = std::min(indent_, WS_LEN); 
      return WHITE_SPACE + (WS_LEN - i); 
    }

    /**
     * @param parent Pointer to the parent pattern. NULL for a root pattern.
     * @param _name The name of the action pattern.
     */
    Pattern::Pattern(Policy * const _parent,
		     const std::string& _name) :
      parent_(_parent),
      patternName_(_name)
    {
    }

    /**
     * Copy constructing is used to clone an instance of a policy from
     * a default instance.
     *
     * Actually we don't realy copy too muth in this constructor,
     * but ensure, that the relevant members stay uninitialized.
     */
    Pattern::Pattern(Pattern const& _rhs) :
      parent_(NULL),
      patternName_(_rhs.patternName_),
      transitionTable_()
    {
    }

    /**
     * Noop implementation.
     */
    Pattern::~Pattern()
    {
    }

    void 
    Pattern::xmlAddTransition(QDomElement _element)
    {
      // a transition must have a message tag
      QDomAttr attrMessage = _element.attributeNode("message");
      if (attrMessage.isNull() || 
	  attrMessage.value().isEmpty()) {
	throw BehaviourEngine::
	  EMalformedPolicy(CORBA::string_dup("Transition without message."));
      }
      std::string message = attrMessage.value().latin1();
    
      // a transition must have a target tag
      QDomAttr attrPattern = _element.attributeNode("target");
      if (attrPattern.isNull() ||
	  attrPattern.value().isEmpty()) {
	throw BehaviourEngine::
	  EMalformedPolicy(CORBA::string_dup("Transition without target."));
      }
      std::string target = attrPattern.value().latin1();

      // internal transitions are only possible within some policy
      if (parent_ == NULL)
	throw BehaviourEngine::
	  EMalformedPolicy(CORBA::string_dup("Internal transition at top level."));
	
      parent_->transitions_.insert(std::make_pair(std::make_pair(getName(),
								 message), 
						  target));
    }

    void 
    Pattern::addTransition(const std::string& _name, 
			   Pattern * const _pattern) 
    {
      transitionTable_[_name] = _pattern;	
    }

    /**
     * Search the transition list for a pattern by name.
     *
     * @param patterName The name of the pattern to be found.
     * @return A pointer to the pattern,
     * or NULL if no pattern with this name is found.
     *
     */
     Pattern * const
     Pattern::getTransitionPattern(const std::string& _patternName) 
     {
       PatternMap::const_iterator i = transitionTable_.find(_patternName);
       return (i != transitionTable_.end())? i->second : NULL;
     }

    /**
     * Self transitions are noops.  Real transitions trigger the
     * transition protocol.  Unbound transitions are passed to the
     * parent or ignored if no parent is available.
     */
    void 
    Pattern::sendTransitionMessage(const std::string& _message) 
    {
      Pattern * const ap = getTransitionPattern(_message);
      if (ap != currentPattern()) { // self transitions are noops
	if (ap != NULL) {           // real transitions trigger the transition protocol
	  ap->open(currentPattern(), _message);
	}
	else {                      // unbound transitions are 
	  if (parent_ != NULL) {    // passed to the parent
	    parent_->sendTransitionMessage(_message);
	  }
	  else {                    // or ignored if no parent is available
		  std::cerr << "Pattern: no transition registered for message! \a" 
		 << std::endl;
	  }
	}
      }
    }

    Pattern *
    Pattern::rootPattern() 
    {
      if (parent_)
	return parent_->rootPattern();
      return this;
    }

    Pattern const *
    Pattern::rootPattern() const
    {
      if (parent_)
	return parent_->rootPattern();
      return this;
    }

    void 
    Pattern::printToStream(std::ostream& ostr) const
    {
      ostr << Indent::ws() << "Transitions:" << std::endl;
      Indent indent;
      PatternMap::const_iterator i;
      for(i = transitionTable_.begin(); i != transitionTable_.end(); ++i) {
	ostr << Indent::ws() << i->first << " --> "
	     << ((i->second)? i->second->getName() : "NULL")
	     << std::endl;
      }
    }

    std::string
    Pattern::getFullName() const {
      if (!parent_)
	return patternName_;
      return parent_->getName() + "/" + patternName_;
    }
    /*

    void 
    Pattern::sendTransitionMessage(const std::string& message) 
    {
    Pattern * pattern = currentPattern();
    if (pattern)
    pattern->sendMessage(NULL, message);
    }

    */
  
    /**
     * This operator serves as an implementation for all 
     * pattern class descendands as it calls their virtual
     * method printToStream().
     */
    std::ostream&
    operator << (std::ostream& ostr, const Pattern& _pattern)
    {
      _pattern.printToStream(ostr);
      return ostr;
    }
  }
}
