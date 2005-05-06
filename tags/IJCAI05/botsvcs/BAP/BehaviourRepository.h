// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BAP_BehaviourRepository_h
#define BAP_BehaviourRepository_h

#include "Behaviour.h"
#include "miro/Repository.h"

namespace Miro
{
  namespace BAP
  {
    typedef Miro::Repository<Behaviour> BehaviourRepository;
  }
}
#endif

