// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
// 
// (c) 2001, 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "FeatureManagementI.h"
#include "FeatureManager.h"
#include "FeatureRepository.h"

#include <sstream>

namespace Miro
{
  namespace MCL
  {
    // Implementation skeleton constructor
    FeatureManagementI::FeatureManagementI(FeatureManager& _manager) throw() :
      manager_(_manager)
    {
    }
    
    // Implementation skeleton destructor
    FeatureManagementI::~FeatureManagementI()
    {
    }

    void
    FeatureManagementI::addFeature(char const *  _feature) throw(EUnknown)
    {
      Feature * feature;
      try {
	feature = FeatureRepository::instance()->get(_feature);
      }
      catch (FeatureRepository::ENotRegistered const& e) {
	throw EUnknown();
      }
      // avoid multiple registrations
      manager_.delFeature(feature);
      manager_.addFeature(feature);
    }
    
    void
    FeatureManagementI::delFeature(char const * _feature) throw(EUnknown)
    {
      Feature * feature;
      try {
	feature = FeatureRepository::instance()->get(_feature);
      }
      catch (FeatureRepository::ENotRegistered const& e) {
	throw EUnknown();
      }
      manager_.delFeature(feature);
    }
    
    FeatureVector *
    FeatureManagementI::currentFeatures() throw()
    {
      FeatureVector_var features = new FeatureVector();
      FeatureManager::FeatureVector f = 
	manager_.features();
      
      features->length(f.size());
      for (unsigned int i = 0; i < f.size(); ++i) {
	features[i] = CORBA::string_dup(f[i]->name().c_str());
      }
      
      return features._retn();
    }
    
    FeatureVector *
    FeatureManagementI::availableFeatures() throw()
    {
      // return type
      FeatureVector_var features = new FeatureVector();
      
      FeatureRepository * repository =
	FeatureRepository::instance();
      std::stringstream sstr;
      sstr << *repository;
      
      std::string name;
      unsigned int maxLen = 8;
      unsigned int len = 0;
      while (sstr >> name) {
	++len;
	if (len > features->length()) {
	  features->length(maxLen);
	  maxLen *= 2;
	}
	features[len - 1] = CORBA::string_dup(name.c_str());
      }
      features->length(len);
      
      return features._retn();
    }
  }
}
