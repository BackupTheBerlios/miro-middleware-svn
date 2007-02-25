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
#include "B21BaseDummy.h"
#include "base/BaseDummyImpl.h"

#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/Log.h"

B21BaseDummy::B21BaseDummy(int argc, char *argv[]) :
  super(argc, argv),
  base()
{
  CORBA::Object_var obj;
  CosNaming::Name n;

  Miro::B21Motion_var pBase;

  pBase = base._this();

  // Force binding of references to make
  // sure they are always up-to-date.
  n.length(1);
  n[0].id = CORBA::string_dup("Motion");
  namingContext->bind(n, pBase.in());

  MIRO_LOG(LL_NOTICE , "DummyServer initialized.." );
}

B21BaseDummy::~B21BaseDummy()
{
  MIRO_DBG(B21 , LL_CTOR_DTOR , "Destructing DummyServer." );
  try {
    CosNaming::Name n;
     
    n.length(1);
    n[0].id = CORBA::string_dup("Motion");
    namingContext->unbind(n);
  }
  catch (const CORBA::Exception& e) {
    MIRO_LOG_OSTR( LL_ERROR , "Caught CORBA exception on unbind: " << e );
  }
}

int
main(int argc, char *argv[])
{
  try {
    MIRO_LOG(LL_NOTICE , "Initialize server daemon." );
    B21BaseDummy b21BaseDummy(argc, argv);
    MIRO_LOG( LL_NOTICE , "Loop forever handling events." );
    b21BaseDummy.run();
    MIRO_LOG( LL_NOTICE , "baseServer ended, exiting." );
  }
  catch (const CORBA::Exception & e) {
    MIRO_LOG_OSTR( LL_ERROR , "Uncaught CORBA exception: " << e );
    return 1;
  }
  catch (...) {
    MIRO_LOG( LL_ERROR , "Uncaught exception: " );
    return 1;
  }
  return 0;
}




