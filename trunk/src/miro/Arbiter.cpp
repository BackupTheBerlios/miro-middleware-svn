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

#include "Arbiter.h"
#include "ArbiterParameters.h"
#include "ArbiterMessage.h"
#include "Behaviour.h"

namespace Miro
{
  Arbiter::Arbiter() :
    mutex_(),
    active_(false),
    message_(),
    params_(NULL)
  {
  }

  Arbiter::~Arbiter()
  {}

  ArbiterParameters * 
  Arbiter::getParametersInstance()
  {
    return new ArbiterParameters();
  }

  ArbiterMessage * 
  Arbiter::getMessageInstance()
  {
    return new ArbiterMessage();
  }

  void
  Arbiter::init(const ArbiterParameters * _params)
  {
    Guard guard(mutex_);

    // convert current arbitration to new configuration

    // build new message vector
    MessageVector tmp(_params->priorities.size(), NULL);
    MessageVector::iterator first, last = tmp.end();
    for (first = tmp.begin(); first != last; ++first)
      delete *first;
    
    // copy old message values if available

    // allways except first time
    if (params_) { 
      ArbiterParameters::RegistrationMap::const_iterator i, j;
      // for each behaviour
      for (i = _params->priorities.begin(); i != _params->priorities.end(); ++i) {
	// if it existed before
	j = params_->priorities.find(i->first);
	if (j != params_->priorities.end()) {
	  // copy its current arbitration message to 
	  // the new index in the new message vector
	  tmp[i->second] = message_[j->second];
	  // remove copy of pointer to avoid confusion
	  message_[j->second] = NULL;
	} 
	else {
	  // else build new entry
	  tmp[i->second] = getMessageInstance();
	}
      }
    }
    else
      for (first = tmp.begin(); first != last; ++first)
	*first = getMessageInstance();
    
    // swap old and new message vectors
    message_.swap(tmp);
    // delete old messages
    last = tmp.end();
    for (first = tmp.begin(); first != last; ++first)
      delete *first;
    // create local copy of current priority mapping
    // for next arbitration change
    params_ = _params;

    // new activations
    calcActivation();
  }

  void
  Arbiter::open()
  {
    active_ = true;
  }

  void
  Arbiter::close()
  {
    Guard guard(mutex_);

    // clear active flag
    active_ = false;

    // delete arbitration messages by hand,
    // to avoid memory leaks
    MessageVector::const_iterator first, last = message_.end();
    for (first = message_.begin(); first != last; ++first)
      delete *first;
    message_.clear();
    params_ = NULL;
  }

  void
  Arbiter::arbitrate(const ArbiterMessage& _message)
  {
    // no anonymous messages allowed
    if (_message.id) {
      // arbiter could be disabled due to a transition
      if (active_) {                   
	Guard guard(mutex_);
	// get behaviours priority
	ArbiterParameters::RegistrationMap::const_iterator i
	  = params_->priorities.find(_message.id);
	// if the behaviour belongs to the current action pattern
	if (i != params_->priorities.end()) {
	  // copy the message
	  message_[i->second]->assign(_message);
	  
	  // set actuators according to arbitration message      
	  calcActivation();
	}
	else {
	  cerr << "PriorityArbiter: got message from unregistered behaviour: "
	       << _message.id->getBehaviourName() << endl;
	}
      }
      else {
	cerr << "PriorityArbiter: got arbitrate call while not active from behaviour: "
	     << _message.id->getBehaviourName() << endl;
      }
    }
    else
      cerr << "PriorityArbiter: received message without behaviour id." << endl;
  }
};
