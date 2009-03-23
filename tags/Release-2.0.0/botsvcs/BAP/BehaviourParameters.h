// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef BAP_BehaviourParameters_h
#define BAP_BehaviourParameters_h

#include <iostream>
#include <qdom.h>

namespace Miro
{
  namespace BAP
  {
    // forward declarations
    class BehaviourParameters;
    class ActionPattern;
    
    //! Ostream operator for debug purposes.
    std::ostream& operator << (std::ostream& ostr, const BehaviourParameters&);

    //! Base class for all parameter classes for derived Behaviours.
    /**
     * This class should actually be generated by a parameters XML file.
     * But we need some dirty tricks, that the clean design of
     * the parameters definitions does not allow...
     *
     * @author Ingmar Baetge
     * @author Hans Utz
     */
    struct BehaviourParameters
    {
      //------------------------------------------------------------------------
      // public methods
      //------------------------------------------------------------------------

      //! Default constructor.
      BehaviourParameters();
      //! Virtual destructor.
      virtual ~BehaviourParameters();

      //! XML-Parsing operator.
      virtual void operator <<= (const QDomNode&);
      virtual QDomElement operator >>= (QDomNode& _node) const;

    protected:
      //------------------------------------------------------------------------
      // protected methods
      //------------------------------------------------------------------------

      //! Dump the BehaviourParameters to the specified output stream.
      virtual void printToStream(std::ostream&) const;

      
    private:
      //------------------------------------------------------------------------
      // private data
      //------------------------------------------------------------------------

      //! Backlink to the action pattern the parameterized behaviour belongs to.
      ActionPattern * pattern;

      //------------------------------------------------------------------------
      // friend declarations
      //------------------------------------------------------------------------

      friend class Behaviour;
      friend class ActionPattern;
      friend std::ostream& operator << (std::ostream& ostr, 
					const BehaviourParameters&);
    };
  }
}
#endif
