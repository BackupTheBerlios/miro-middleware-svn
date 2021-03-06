// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BAP_ArbiterParameters_h
#define BAP_ArbiterParameters_h

#include <iostream>
#include <map>

// forward declarations
class QDomNode;
class QDomElement;

namespace Miro
{
  namespace BAP
  {
    // forward declarations
    class Behaviour;
    class ActionPattern;
    class Arbiter;
    class ArbiterParameters;

    //! Output stream operator for debug purposes.
    std::ostream&
    operator << (std::ostream& ostr, const ArbiterParameters& _params);

    //! Base class for all parameter classes of derived Arbiters.
    /**
     * This class should actually be generated by a parameters XML file.
     * But we need some dirty tricks, that the clean design of
     * the parameters definitions does not allow...
     *
     * @author Hans Utz
     */
    struct ArbiterParameters
    {
      //------------------------------------------------------------------------
      // public types
      //------------------------------------------------------------------------

      //! Mapping of behaviour id's to the registration order.
      /**
       * The priority based arbiter chose the active behaviours 
       * actuatory request with the lowest registration number.
       */
      typedef std::map<Behaviour *, unsigned int> RegistrationMap;
      
      //------------------------------------------------------------------------
      // public methods
      //------------------------------------------------------------------------

      //! Map of the behaviours priorities.
      RegistrationMap priorities;
      
      //! Default constructor.
      ArbiterParameters();
      //! Copy constructor.
      ArbiterParameters(ArbiterParameters const&);
      //! Virtual destructor.
      virtual ~ArbiterParameters();
      
      //! XML-parsingoperator.
      virtual void operator <<= (const QDomNode&);
      //! XML-parsingoperator.
      virtual QDomElement operator >>= (QDomNode&) const;
      
      //! Register behaviours for acces control
      virtual void registerBehaviour(Behaviour * _behaviour);
      
    protected:
      //------------------------------------------------------------------------
      // protected methods
      //------------------------------------------------------------------------

      //! Dump the BehaviourParameters to the specified output stream.
      virtual void printToStream(std::ostream& ostr) const;
      
      //------------------------------------------------------------------------
      // protected data
      //------------------------------------------------------------------------

      //!  To assign for the next registered behaviour.
      unsigned int nextNumber_;
      
    private:
      //------------------------------------------------------------------------
      // hidden methods
      //------------------------------------------------------------------------
      //! Assignement operator.
      ArbiterParameters& operator=(ArbiterParameters const&);

      //------------------------------------------------------------------------
      // private data
      //------------------------------------------------------------------------

      //! Backlink to the action pattern the parameterized arbiter belongs to.
      ActionPattern * pattern;
      
      //------------------------------------------------------------------------
      // friend declarations
      //------------------------------------------------------------------------

      friend class Arbiter;
      friend class ActionPattern;
      friend std::ostream& operator << (std::ostream& ostr,
					const ArbiterParameters& _params);
    };
  }
}
#endif
