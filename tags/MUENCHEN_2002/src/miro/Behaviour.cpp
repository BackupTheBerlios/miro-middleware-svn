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

#include "Behaviour.h"
#include "BehaviourParameters.h"
#include "ActionPattern.h"

namespace Miro 
{
  /**
   * Initializing the data members to their defaults.
   */
  Behaviour::Behaviour() :
    active_(false),
    params_(NULL)
  {}

  /**
   * Any class with virtual functions needs a virtual destructor.
   * Provided to avoid errors in derived classes.
   */
  Behaviour::~Behaviour()
  {}

  /**
   * Returns a pointer to a newly created instance of some derived
   * BehaviourParameters class. The caller takes ownership of the 
   * object.
   */
  BehaviourParameters * 
  Behaviour::getParametersInstance()
  {
    return new BehaviourParameters();
  }

  /**
   * Called on each pattern change of a behaviour. If the behaviour
   * is inactive, init is call before open is called. If the
   * behaviour is already active and will be active also in the next
   * ActionPattern, no close()/open() combination will be called,
   * but the behaviours init() method is called concurrently to its
   * action() implementation.
   */
  void
  Behaviour::init(const BehaviourParameters * _params)
  {
    params_ = _params;
  }

  /**
   * Called on activation of the behaviour. isActive() will return
   * true until the next call to close(). Note that when overwriting
   * this method the parents implementation of open() should be called
   * first within the new implementation.
   */
  void
  Behaviour::open()
  {
    active_ = true;
  }

  /** 
   * Called on deactivation of the behaviour. isActive() will return
   * false until the next call to open().  Note that when overwriting
   * this method the parents implementation of open() should be called
   * at the end of the new implementation.
   */
  void
  Behaviour::close()
  {
    active_ = false;
  }

  void 
  Behaviour::sendMessage(const std::string& message) 
  {
    ActionPattern * pattern = ActionPattern::currentActionPattern();
    if (pattern)
      pattern->sendMessage(this, message);
  }
  
  void
  Behaviour::calcDynamicWindow(DynamicWindow *) {
  }
};
