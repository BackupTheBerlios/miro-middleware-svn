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
#ifndef ArbiterParameters_h
#define ArbiterParameters_h

#include <map>
#include <iostream>


// forward declarations
class QDomNode;

namespace Miro
{
  // forward declarations
  class Behaviour;
  class ArbiterParameters;

  std::ostream& operator << (std::ostream& ostr, const ArbiterParameters&);

  struct ArbiterParameters
  {
    ArbiterParameters();
    virtual ~ArbiterParameters();
    
    typedef std::map<Behaviour *, unsigned int> PriorityMap;

    PriorityMap priorities;

    virtual void operator <<= (const QDomNode&);

  protected:
    virtual void printToStream(std::ostream& /*ostr*/) const;

    friend 
    std::ostream& operator << (std::ostream& ostr, const ArbiterParameters& _params);
  };
};
#endif
