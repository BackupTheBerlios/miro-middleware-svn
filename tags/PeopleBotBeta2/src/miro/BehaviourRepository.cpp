// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BehaviourRepository.h"
#include "Behaviour.h"

namespace Miro
{
  /**
   * To get a pointer to the global ArbiterRepository instance,
   * simply call ArbiterRepository::instance();
   */
  Singleton<BehaviourRepository> BehaviourRepository::instance;

  BehaviourRepository::BehaviourRepository()
  {}

  void
  BehaviourRepository::registerBehaviour(Behaviour * _behaviour)  throw(Exception)
  {
    cout << "BehaviourRepository: Registering " << _behaviour->getBehaviourName() << endl;
    if (behaviours_.find(_behaviour->getBehaviourName()) != behaviours_.end())
      throw Exception("Behaviour " + _behaviour->getBehaviourName() + " already registered at the Repository.");
      
    behaviours_[_behaviour->getBehaviourName()] = _behaviour;	
  }

  Behaviour * 
  BehaviourRepository::getBehaviour(const std::string& _name) throw()
  {
    BehaviourMap::const_iterator i = behaviours_.find(_name);
    if (i != behaviours_.end())
      return i->second;
    return NULL;
  }

  void
  BehaviourRepository::printToStream(std::ostream& ostr) const
  {
    BehaviourRepository::BehaviourMap::const_iterator i;	

    ostr << "Active Behaviours:" << endl;
    for(i = behaviours_.begin(); i != behaviours_.end(); ++i) {		
      // if a behaviour is active, check for disabling
      if (i->second->isActive()) {
	ostr << i->first << endl;
      }
    }
	
    ostr << "Inactive Behaviours:" << endl;
    for(i = behaviours_.begin(); i != behaviours_.end(); ++i) {		
      // if a behaviour is active, check for disabling
      if (!i->second->isActive()) {
	ostr << i->first << endl;
      }
    }
  }

  std::ostream&
  operator << (ostream& ostr, const BehaviourRepository& _repository)
  {
    _repository.printToStream(ostr);
    return ostr;
  }

};
