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
#ifndef ArbiterMessage_h
#define ArbiterMessage_h

#include <iostream>

namespace Miro
{
  // forward declarations
  class Behaviour;

  //! Base class of all arbiter messages.
  /**
   * Arbiters can extend their derived message by individual
   * arbitration parameters.
   *
   * @author Hans Utz
   */
  struct ArbiterMessage
  {
    //! Id of the behaviour
    Behaviour * id;
    //! True if the behaviour requests to be considered in the arbitration process.
    bool active;

    //! Default contructor.
    ArbiterMessage();
    //! Virtual destructor.
    virtual ~ArbiterMessage();

    //! Virtual assignement operator replacement.
    virtual void assign(const ArbiterMessage& _lhs);

  protected:
    //! Dump message to the specified output stream.
    virtual void printToConsole(std::ostream& ostr) const;

    friend 
    std::ostream& operator << (std::ostream& ostr, const ArbiterMessage& _message);
  };

  //! Ostream opreator for debug purposes.
  std::ostream&
  operator << (std::ostream& ostr, const ArbiterMessage& _message);
};
#endif
