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
#ifndef ArbiterRepository_h
#define ArbiterRepository_h

#include "miro/Singleton.h"

#include <string>
#include <map>
#include <iostream>


namespace Miro
{
  // forward declaration
  class Arbiter;
  class ArbiterRepository;
  std::ostream&
  operator << (std::ostream& ostr, const ArbiterRepository& factory);

  // class declaration
  class ArbiterRepository 
  {
  public:
    ArbiterRepository();
    virtual ~ArbiterRepository();

    void registerArbiter(Arbiter * _arbiter);
    Arbiter * getArbiter(const std::string& _name);

    static Singleton<ArbiterRepository> instance;

  protected:
    virtual void printToStream(std::ostream& ostr) const;

    typedef std::map<std::string, Arbiter *> ArbiterMap;

    ArbiterMap arbiters_;

    friend std::ostream& operator << (std::ostream&, const ArbiterRepository&);
  };
};
#endif
