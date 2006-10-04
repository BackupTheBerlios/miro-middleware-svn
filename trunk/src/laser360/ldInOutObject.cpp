
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldInOutObject.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

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
