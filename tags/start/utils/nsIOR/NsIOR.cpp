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


#include <miro/Client.h>

#include <iostream>

using Miro::Client;
using CORBA::String_var;

int main(int argc, char *argv[])
{
  // initialize orb //
  Client client(argc, argv);
    
  // Write the IOR to cout
  String_var str = client.orb()->object_to_string(client.initialNamingContext.in());

  cout << "Naming Service IOR:" << endl
       << str << endl;

  return 0;
}
