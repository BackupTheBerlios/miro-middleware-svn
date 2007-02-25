// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "BehaviourRepository.h"
#include "Behaviour.h"

namespace Miro
{
  namespace BAP
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
      std::cout << "BehaviourRepository: Registering " << _behaviour->getBehaviourName() << std::endl;
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

      ostr << "Active Behaviours:" << std::endl;
      for(i = behaviours_.begin(); i != behaviours_.end(); ++i) {		
	// if a behaviour is active, check for disabling
	if (i->second->isActive()) {
	  ostr << i->first << std::endl;
	}
      }
	
      ostr << "Inactive Behaviours:" << std::endl;
      for(i = behaviours_.begin(); i != behaviours_.end(); ++i) {		
	// if a behaviour is active, check for disabling
	if (!i->second->isActive()) {
	  ostr << i->first << std::endl;
	}
      }
    }

    std::ostream&
    operator << (ostream& ostr, const BehaviourRepository& _repository)
    {
      _repository.printToStream(ostr);
      return ostr;
    }
  }
}
