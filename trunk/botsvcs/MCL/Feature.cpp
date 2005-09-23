// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
// 
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "Feature.h"

#include <iostream>

namespace Miro
{
  namespace MCL
  {
    Feature::Feature(std::string const& _name) :
      name_(_name)
    {}
    
    Feature::~Feature()
    {}
    
    void Feature::initStatistics()
    {
    }
    void Feature::setGlobalMax()
    {
    }
    void Feature::statistics(std::ostream&) const
    {
    }
    void Feature::printLast(std::ostream&) const
    {
    }
    bool Feature::validData() const
    {
      return true;
    }
  }
}
