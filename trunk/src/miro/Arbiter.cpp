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

#include "Arbiter.h"
#include "ArbiterParameters.h"
#include "ArbiterMessage.h"
#include "Behaviour.h"
#include "miro/Log.h"

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

  /**
   * Returns a pointer to a newly created instance of some derived
   * ArbiterParameters class. The caller takes ownership of the 
   * object.
   */
  ArbiterParameters * 
  Arbiter::getParametersInstance() const
  {
    return new ArbiterParameters();
  }

  /**
   * Returns a pointer to a newly created instance of some derived
   * ArbiterMessage class. The caller takes ownership of the 
   * object.
   */
  ArbiterMessage * 
  Arbiter::getMessageInstance() const
  {
    return new ArbiterMessage();
  }
  
  /**
   * Called on transition to a new action pattern. If the arbiter is currently
   * inactive open is called afterwards. Note that if the arbiter is already
   * part of the current behaviour, arbitrate can be called concurrently.
   */
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

  /**
   * Called to start an arbiter. Note that arbiters are not closed and reopend
   * on a transition between action patterns, if they are also part of the 
   * subsequent action pattern.
   */
  void
  Arbiter::open()
  {
    active_ = true;
  }

  /**
   * Called to stop an arbiter. Note that arbiters are not closed and reopend
   * on a transition between action patterns, if they are also part of the 
   * subsequent action pattern.
   */
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
	else { MIRO_LOG_OSTR(LL_WARNING, "PriorityArbiter: got message from unregistered behaviour: "<< _message.id->getBehaviourName() << std::endl);
	}
      }
      else { MIRO_LOG_OSTR(LL_WARNING, "PriorityArbiter: got arbitrate call while not active from behaviour:" << _message.id->getBehaviourName() << std::endl);
      }
    }
    else MIRO_LOG_OSTR(LL_WARNING, "PriorityArbiter: received message without behaviour id." << std::endl);
  }

  /**
   * @return A pointer containing an arbiter message.
   * Note that the caller takes ownership of the message and is
   * responsible for freeing its memory if the message is no longer
   * needed, by using delete.
   *
   * Creates a default message instance and initializes the message id.
   */
  ArbiterMessage *
  Arbiter::getMessageForBehaviour(Behaviour * _id)
  {
    ArbiterMessage * message = getMessageInstance();

    message->id = _id;
    return message;
  }
}
