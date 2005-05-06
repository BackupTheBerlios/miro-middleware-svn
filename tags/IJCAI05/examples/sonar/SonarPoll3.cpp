#include <miro/Client.h>
#include <idl/RangeSensorC.h>
#include <miro/IO.h>

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using Miro::Client;
using Miro::RangeSensor;
using Miro::RangeSensor_var;
using Miro::RangeScanEventIDL_var;

int main(int argc, char *argv[])
{
  Client client(argc, argv);            // Initialize orb.
  try {                                 // Obtain reference to sonar object.
    RangeSensor_var sonar = client.resolveName<RangeSensor>("Sonar");
    RangeScanEventIDL_var sonarScan;         // A RangeScanIDL smart pointer.
  
    sonarScan = sonar->getFullScan();   // Get values of all sonars.

    cout << "Sonar reading: " << sonarScan.in() << endl; // Print sonar scan.
  }
  catch (const CORBA::Exception& e) {   // Catch any CORBA exception.
    cerr << "Exception on sonar query:" << endl << e << endl;
    return 1;
  }
  
  return 0;
}
