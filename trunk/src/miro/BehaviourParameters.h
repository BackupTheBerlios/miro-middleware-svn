// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
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

  //! Ostream operator for debug purposes.
  std::ostream& operator << (std::ostream& ostr, const BehaviourParameters&);

  //! Base class for all parameter classes for derived Behaviours.
  struct BehaviourParameters
  {
    //! Virtual destructor.
    virtual ~BehaviourParameters();

    //! XML-Parsing operator.
    virtual void operator <<= (const KeyValueList&);

  protected:
    //! Dump the BehaviourParameters to the specified output stream.
    virtual void printToStream(std::ostream&) const;

    friend
    std::ostream& operator << (std::ostream& ostr, const BehaviourParameters&);
  };
};
#endif
