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
#include "ArbiterMessage.h"

namespace Miro
{
  namespace BAP
  {
    std::ostream&
    operator << (std::ostream& _ostr, const ArbiterMessage& _message)
    {
      _message.printToStream(_ostr);
      return _ostr;
    }

    /**
     * Initializes @ref id to NULL and @ref active to false.
     */
    ArbiterMessage::ArbiterMessage() :
      id(NULL),
      active(false)
    {}
    
    /** Noop implementation. */
    ArbiterMessage::~ArbiterMessage()
    {
    }
    
    void
    ArbiterMessage::assign(const ArbiterMessage& _lhs)
    {
      *this = _lhs;
    }

    void
    ArbiterMessage::printToStream(std::ostream& _ostr) const
    {
      _ostr << "id: " << id << std::endl
	    << "active: " << active << std::endl;
    }
  }
}
