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

#include "ArbiterRepository.h"
#include "Arbiter.h"
#include "Log.h"

namespace Miro
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
    MIRO_LOG_OSTR(LL_NOTICE, "ArbiterRepository: Registering " 
	 << _arbiter->getName() << std::endl);
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
};
