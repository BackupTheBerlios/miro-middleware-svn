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
#ifndef OnButton_h
#define OnButton_h

#include "miro/EventBehaviour.h"

#include "examples/behaviours/b21/Parameters.h"

class OnButton : public Miro::EventBehaviour
{
  typedef EventBehaviour Super;

public:
  OnButton(CosNotifyChannelAdmin::EventChannel_ptr _ec,
	   const std::string& _domainName);

  BEHAVIOUR_PARAMETERS_FACTORY(OnButtonParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
  static const std::string name_;
};
#endif
