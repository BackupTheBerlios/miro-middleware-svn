// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BehaviourParameters_h
#define BehaviourParameters_h

#include <iostream>
#include <qdom.h>

namespace Miro
{
  // forward declarations
  class KeyValueList;
  class BehaviourParameters;
  class ActionPattern;

  //! Ostream operator for debug purposes.
  std::ostream& operator << (std::ostream& ostr, const BehaviourParameters&);

  //! Base class for all parameter classes for derived Behaviours.
  /**
   * @author Ingmar Baetge
   * @author Hans Utz
   */
  struct BehaviourParameters
  {
    //! Default constructor.
    BehaviourParameters();
    //! Virtual destructor.
    virtual ~BehaviourParameters();

    //! XML-Parsing operator.
    virtual void operator <<= (const QDomNode&);
    virtual QDomElement operator >>= (QDomNode& _node) const;

  protected:
    //! Dump the BehaviourParameters to the specified output stream.
    virtual void printToStream(std::ostream&) const;

    friend
    std::ostream& operator << (std::ostream& ostr, const BehaviourParameters&);

  private:
    ActionPattern * pattern;

    friend class Behaviour;
    friend class ActionPattern;
  };
};
#endif
