// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "FeatureRepository.h"

namespace Miro
{
  template<> 
  Miro::Singleton<Miro::Repository<Miro::MCL::Feature> > Repository<Miro::MCL::Feature>::instance;
}

