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
#ifndef BehaviourParameters_h
#define BehaviourParameters_h

#include <iostream>

namespace Miro
{
  // forward declarations
  class KeyValueList;
  class BehaviourParameters;

  std::ostream& operator << (std::ostream& ostr, const BehaviourParameters&);

  struct BehaviourParameters
  {
    BehaviourParameters();
    virtual ~BehaviourParameters();

    virtual void operator <<= (const KeyValueList&);

  protected:
    virtual void printToStream(std::ostream&) const;

    friend
    std::ostream& operator << (std::ostream& ostr, const BehaviourParameters&);
  };
};
#endif
