// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


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




