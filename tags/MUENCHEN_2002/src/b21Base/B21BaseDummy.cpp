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

#include <iostream>

using std::cout;
using std::cerr;

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

#ifdef DEBUG
  cout << "DummyServer initialized.." << endl;
#endif
}

B21BaseDummy::~B21BaseDummy()
{
#ifdef DEBUG
  cout << "Destructing DummyServer." << endl;
#endif
  try {
    CosNaming::Name n;
     
    n.length(1);
    n[0].id = CORBA::string_dup("Motion");
    namingContext->unbind(n);
  }
  catch (const CORBA::Exception& e) {
    cerr << "Caught CORBA exception on unbind: " << e << endl;
  }
}

int
main(int argc, char *argv[])
{
  try {
#ifdef DEBUG
    cout << "Initialize server daemon." << endl;
#endif
    B21BaseDummy b21BaseDummy(argc, argv);
#ifdef DEBUG
    cout << "Loop forever handling events." << endl;
#endif
    b21BaseDummy.run();
#ifdef DEBUG
    cout << "baseServer ended, exiting." << endl;
#endif
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }
  return 0;
}




