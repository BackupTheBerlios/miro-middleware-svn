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


#include "BehaviourRepository.h"
#include "Behaviour.h"

namespace Miro
{
  Singleton<BehaviourRepository> BehaviourRepository::instance;

  void
  BehaviourRepository::registerBehaviour(Behaviour * _behaviour) 
  {
    cout << "BehaviourRepository: Registering " << _behaviour->getBehaviourName() << endl;
    behaviours_[_behaviour->getBehaviourName()] = _behaviour;	
  }

  Behaviour * 
  BehaviourRepository::getBehaviour(const std::string& _name) 
  {
    BehaviourMap::const_iterator i = behaviours_.find(_name);
    if (i != behaviours_.end())
      return i->second;
    else
      return NULL;
  }

  std::ostream&
  operator << (ostream& ostr, const BehaviourRepository& factory)
  {
    BehaviourRepository::BehaviourMap::const_iterator i;	

    ostr << "Active Behaviours:" << endl;
    for(i = factory.behaviours_.begin(); i != factory.behaviours_.end(); ++i) {		
      // if a behaviour is active, check for disabling
      if (i->second->isActive()) {
	ostr << i->first << endl;
      }
    }
	
    ostr << "Inactive Behaviours:" << endl;
    for(i = factory.behaviours_.begin(); i != factory.behaviours_.end(); ++i) {		
      // if a behaviour is active, check for disabling
      if (!i->second->isActive()) {
	ostr << i->first << endl;
      }
    }

    return ostr;
  }
};
