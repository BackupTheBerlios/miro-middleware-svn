// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ArbiterRepository.h"
#include "Arbiter.h"

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
    cout << "ArbiterRepository: Registering " 
	 << _arbiter->getName() << endl;
    if (arbiters_.find(_arbiter->getName()) != arbiters_.end())
      throw Exception("Arbiter " + _arbiter->getName() + " already registered at the Repository.");
      
    arbiters_[_arbiter->getName()] = _arbiter;	
  }

  Arbiter * 
  ArbiterRepository::getArbiter(const std::string& _name) throw(Exception)
  {
    ArbiterMap::const_iterator i = arbiters_.find(_name);
    if (i != arbiters_.end())
      return i->second;
    throw Exception("Arbiter " + _name + " not registered at the Repository.");
  }

  void
  ArbiterRepository::printToStream(std::ostream& ostr) const
  {
    ArbiterRepository::ArbiterMap::const_iterator i;

    ostr << "Active Arbiters:" << endl;
    for(i = arbiters_.begin(); i != arbiters_.end(); ++i) {
      // if a arbiter is active, check for disabling
      if (i->second->isActive()) {
	ostr << i->first << endl;
      }
    }
	
    ostr << "Inactive Arbiters:" << endl;
    for(i = arbiters_.begin(); i != arbiters_.end(); ++i) {
      // if a behaviour is active, check for disabling
      if (!i->second->isActive()) {
	ostr << i->first << endl;
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
