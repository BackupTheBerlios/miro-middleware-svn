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
  Behaviour::Behaviour() :
    active_(false),
    params_(NULL)
  {}

  Behaviour::~Behaviour()
  {}

  BehaviourParameters * 
  Behaviour::getParametersInstance()
  {
    return new BehaviourParameters();
  }

  void
  Behaviour::init(const BehaviourParameters * _params)
  {
    params_ = _params;
  }

  void
  Behaviour::open()
  {
    active_ = true;
  }

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
