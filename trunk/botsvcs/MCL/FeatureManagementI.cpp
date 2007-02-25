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
