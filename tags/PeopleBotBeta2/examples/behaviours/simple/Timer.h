// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Timer_hh
#define Timer_hh

#include "Parameters.h"

#include <miro/TimedBehaviour.h>
#include <miro/Synch.h>

class Timer : public Miro::TimedBehaviour
{
  typedef Miro::TimedBehaviour Super;

public:
  Timer(ACE_Reactor &);

  BEHAVIOUR_PARAMETERS_FACTORY(TimerParameters);

  void init(const Miro::BehaviourParameters*);
  void action();
  const std::string& getBehaviourName() const;

protected:
  Miro::Mutex mutex_;
  ACE_Time_Value timeValue_;

  static const std::string name_;
};

#endif
