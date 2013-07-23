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
#include "ArbiterRepository.h"
#include "Arbiter.h"

namespace Miro
{
  namespace BAP
  {
    /**
     * To get a pointer to the global ArbiterRepository instance,
     * simply call ArbiterRepository::instance();
     */
    Singleton<ArbiterRepository> ArbiterRepository::instance;

    ArbiterRepository::ArbiterRepository() :
      arbiters_()
    {}

    void 
    ArbiterRepository::registerArbiter(Arbiter * _arbiter) throw(Exception)
    {
      std::cout << "ArbiterRepository: Registering " 
		<< _arbiter->getName() << std::endl;
      if (arbiters_.find(_arbiter->getName()) != arbiters_.end())
	throw Exception("Arbiter " + _arbiter->getName() + " already registered at the Repository.");
      
      arbiters_[_arbiter->getName()] = _arbiter;	
    }

    Arbiter * 
    ArbiterRepository::getArbiter(const std::string& _name) throw()
    {
      ArbiterMap::const_iterator i = arbiters_.find(_name);
      if (i != arbiters_.end())
	return i->second;
      return NULL;
    }

    void
    ArbiterRepository::printToStream(std::ostream& ostr) const
    {
      ArbiterRepository::ArbiterMap::const_iterator i;

      ostr << "Active Arbiters:" << std::endl;
      for(i = arbiters_.begin(); i != arbiters_.end(); ++i) {
	// if a arbiter is active, check for disabling
	if (i->second->isActive()) {
	  ostr << i->first << std::endl;
	}
      }
	
      ostr << "Inactive Arbiters:" << std::endl;
      for(i = arbiters_.begin(); i != arbiters_.end(); ++i) {
	// if a behaviour is active, check for disabling
	if (!i->second->isActive()) {
	  ostr << i->first << std::endl;
	}
      }
    }

    std::ostream&
    operator << (std::ostream& ostr, const ArbiterRepository& _factory)
    {
      _factory.printToStream(ostr);
      return ostr;
    }
  }
}
