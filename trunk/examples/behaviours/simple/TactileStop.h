// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef TactileStop_h
#define TactileStop_h

#include "miro/MotionC.h"
#include "miro/EventBehaviour.h"

#include "Parameters.h"

class TactileStop : public Miro::EventBehaviour
{
  typedef Miro::EventBehaviour Super;

public:
  TactileStop(Miro::Motion_ptr _motion,
	      CosNotifyChannelAdmin::EventChannel_ptr _ec,
	      const std::string& _domainName);
  ~TactileStop();

  BEHAVIOUR_PARAMETERS_FACTORY(TactileStopParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
  Miro::Motion_var motion_;
  std::string domainName_;

  static const std::string name_;
};
#endif
