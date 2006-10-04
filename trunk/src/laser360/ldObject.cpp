
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldObject.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------


#include "ldObject.h"

namespace ldoem_
{
  //---------------------------------------------------------------------------
  // initialization static variable
  bool Object::initialized_ = false;

  //---------------------------------------------------------------------------
  // constructor
  Object::Object()
  {
    if ( !initialized_ )
    {
      initialize();
    }
  }

  //---------------------------------------------------------------------------
  // destructor
  Object::~Object()
  {
  }

  //---------------------------------------------------------------------------
  // member method
  const char * Object::getTypeName() const
  {
    return "object";
  }

  //---------------------------------------------------------------------------
  // member method
  void Object::initialize()
  {
    initialized_ = true;
  }
}
