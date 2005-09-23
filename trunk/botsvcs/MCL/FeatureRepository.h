// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MCL_FeatureRepository_h
#define MCL_FeatureRepository_h

#include "Feature.h"

#include <miro/Repository.h>

namespace Miro
{
  namespace MCL
  {
    //! The repository of feature evaluators.
    typedef Miro::Repository<Feature> FeatureRepository;
  }
}
#endif // MCL_FeatureRepository_h
