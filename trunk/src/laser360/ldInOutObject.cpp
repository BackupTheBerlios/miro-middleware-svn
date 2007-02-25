// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005
// Department of Software Technology, Technical University of Graz, Austria
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
#include "ldInOutObject.h"
#include <iostream>

namespace ldoem_
{

  //---------------------------------------------------------------------------
  // default constructor
  InOutObject::InOutObject() : Object()
  {
  }

  //---------------------------------------------------------------------------
  // copy constructor
  InOutObject::InOutObject( const InOutObject & other ) : Object()
  {
    copy( other );
  }

  //---------------------------------------------------------------------------
  // destructor
  InOutObject::~InOutObject()
  {
  }

  //---------------------------------------------------------------------------
  // copy data from other InOutObject
  InOutObject & InOutObject::copy( const InOutObject & other )
  {
    return ( * this );
  }

  //---------------------------------------------------------------------------
  // assigment operator (alias for copy(other)).
  InOutObject & InOutObject::operator = ( const InOutObject & other )
  {
    return copy( other );
  }

  //---------------------------------------------------------------------------
  // get type name
  const char * InOutObject::getTypeName() const
  {
    return "InOutObject";
  }

  //---------------------------------------------------------------------------
  // read InOutObject
  bool read( InOutHandler & handler, InOutObject & p, const bool complete )
  {
    return p.read( handler, complete );
  }

  //---------------------------------------------------------------------------
  // write InOutObject
  bool write( InOutHandler & handler, const InOutObject & p, const bool complete )
  {
    return p.write( handler, complete );
  }
}
