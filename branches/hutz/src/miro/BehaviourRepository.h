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
#ifndef BehaviourRepository_h
#define BehaviourRepository_h

#include "Singleton.h"

#include <map>
#include <string>
#include <iostream>

namespace Miro
{
  // forward declarations
  class Behaviour;
  class BehaviourRepository;

  std::ostream&
  operator << (std::ostream& ostr, const BehaviourRepository& factory);

  class BehaviourRepository  
  {
  public:
    void registerBehaviour(Behaviour * _behaviour);
    Behaviour* getBehaviour(const std::string& _name);

    static Singleton<BehaviourRepository> instance;

  protected:
    typedef std::map<std::string, Behaviour*> BehaviourMap;
    BehaviourMap behaviours_;

    friend std::ostream& operator << (std::ostream&, const BehaviourRepository&);
  };
};
#endif

