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


#include "ArbiterRepository.h"
#include "Arbiter.h"

namespace Miro
{
  Singleton<ArbiterRepository> ArbiterRepository::instance;

  ArbiterRepository::ArbiterRepository() :
    arbiters_()
  {}

  ArbiterRepository::~ArbiterRepository()
  {
  }

  void 
  ArbiterRepository::registerArbiter(Arbiter * _arbiter) 
  {
    cout << "ArbiterRepository: Registering " 
	 << _arbiter->getName() << endl;
    arbiters_[_arbiter->getName()] = _arbiter;	
  }

  Arbiter * 
  ArbiterRepository::getArbiter(const std::string& _name) 
  {
    ArbiterMap::const_iterator i = arbiters_.find(_name);
    if (i != arbiters_.end())
      return i->second;
    else
      return NULL;
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
