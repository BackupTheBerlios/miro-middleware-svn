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
#ifndef TactileStop_h
#define TactileStop_h

#include "idl/MotionC.h"
#include "botsvcs/BAP/EventBehaviour.h"

#include "Parameters.h"

class TactileStop : public Miro::BAP::EventBehaviour
{
  typedef Miro::BAP::EventBehaviour Super;

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

  static const std::string name_;
};
#endif
