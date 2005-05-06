// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (Middleware for Robots)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MoveToPoint_hh
#define MoveToPoint_hh

#include "botsvcs/BAP/EventBehaviour.h"

#include "Parameters.h"

class  MoveToPoint : public Miro::BAP::EventBehaviour
{
  typedef EventBehaviour Super;

public:
  MoveToPoint(CosNotifyChannelAdmin::EventChannel_ptr _ec,
	      const std::string& _domainName);
  virtual ~MoveToPoint();

  BEHAVIOUR_PARAMETERS_FACTORY(MoveToPointParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
 static std::string name_;
};
#endif
