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
#ifndef OnButton_h
#define OnButton_h

#include "miro/EventBehaviour.h"

#include "OnButtonParameters.h"

class OnButton : public Miro::EventBehaviour
{
  typedef EventBehaviour Super;

public:
  OnButton(CosNotifyChannelAdmin::EventChannel_ptr _ec,
	   const std::string& _domainName);

  // factory method for BehaviourParameters
  OnButtonParameters * getParametersInstance();

  void action();
  const std::string& getBehaviourName() const;

protected:
  std::string domainName_;

  static std::string name_;
};
#endif
