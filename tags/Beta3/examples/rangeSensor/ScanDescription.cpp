// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/Client.h"
#include "idl/RangeSensorC.h"
#include "miro/ScanDescriptionHelper.h"

using std::cout;
using std::cerr;
using std::endl;

using Miro::Client;
using Miro::RangeSensor;
using Miro::RangeSensor_var;
using Miro::ScanDescriptionIDL_var;

int main(int argc, char * argv[])
{
  Client client(argc, argv);

  if (argc < 2) {
    cout << "usage: " << argv[0] << " <sensor name>" << endl;
    return 1;
  }

  cout << "scan desecription of " << argv[1] << ":" << endl;

  try {
    RangeSensor_var sensor = client.resolveName<RangeSensor>(argv[1]);

    ScanDescriptionIDL_var description = sensor->getScanDescription();

    cout << *description << endl;
  }
  catch (const CORBA::Exception& e) {
    cerr << "CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
