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
#ifndef ArbiterParameters_h
#define ArbiterParameters_h

#include <iostream>
#include <map>

// forward declarations
class QDomNode;

namespace Miro
{
  // forward declarations
  class Behaviour;
  class ArbiterParameters;

  //! Output stream operator for debug purposes.
  std::ostream&
  operator << (std::ostream& ostr, const ArbiterParameters& _params);

  //! Base class for parameters for Arbiter class childs.
  struct ArbiterParameters
  {
    //! Mapping of behaviour id's to the registration order.
    /**
     * The priority based arbiter chose the active behaviours 
     * actuatory request with the lowest registration number.
     */
    typedef std::map<Behaviour *, unsigned int> RegistrationMap;

    //! Map of the behaviours priorities.
    RegistrationMap priorities;

    //! Default constructor.
    ArbiterParameters();
    //! Virtual destructor.
    virtual ~ArbiterParameters();

    //! XML-parsingoperator.
    void operator<<=(const QDomNode&);

    //! Register behaviours for acces control
    virtual void registerBehaviour(Behaviour * _behaviour);
    
  protected:
    //! Dump the ArbiterParameters to the specified output stream.
    virtual void printToStream(std::ostream& ostr) const;

    //!  to assign for the next registered behaviour
    unsigned int nextNumber_;

    friend
    std::ostream&
    operator << (std::ostream& ostr, const ArbiterParameters& _params);

  };
};
#endif
