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
#include "PowerArbiterMessage.h"

namespace Miro
{
  namespace BAP
  {
    /**
     * Initializing the wheel powers to zero.
     */
    PowerArbiterMessage::PowerArbiterMessage() :
      Super(),
      left(0),
      right(0)
    {
    }
    
    void
    PowerArbiterMessage::assign(const ArbiterMessage& _lhs)
    {
      Super::assign(_lhs);
      const Self& pam = dynamic_cast<const Self&>(_lhs);
      left = pam.left;
      right = pam.right;
    }
    
    void
    PowerArbiterMessage::printToStream(std::ostream& _ostr) const
    {
      Super::printToStream(_ostr);
      _ostr << "left power (+/- 30000): " << left << std::endl
	    << "right power (+/- 30000): " << right << std::endl;
    }
  }
}
