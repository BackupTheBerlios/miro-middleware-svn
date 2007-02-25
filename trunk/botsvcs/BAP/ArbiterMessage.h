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
#ifndef ArbiterMessage_h
#define ArbiterMessage_h

#include <iostream>

namespace Miro
{
  namespace BAP
  {
    // forward declarations
    class Behaviour;
    class ArbiterMessage;
    
    //! Ostream opreator for debug purposes.
    std::ostream&
    operator << (std::ostream& ostr, const ArbiterMessage& _message);

    //! Base class of all arbiter messages.
    /**
     * Arbiters can extend their derived message by individual
     * arbitration parameters.
     *
     * @author Hans Utz
     */
    struct ArbiterMessage
    {
      //! Id of the behaviour.
      /**
       * The this pointer of the behaviour is used in this case. What's 
       * acutally needed is a unique identifier.
       */
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
      virtual void printToStream(std::ostream& ostr) const;
      
      friend 
      std::ostream& operator << (std::ostream& ostr, const ArbiterMessage& _message);
    };
  }
}
#endif
