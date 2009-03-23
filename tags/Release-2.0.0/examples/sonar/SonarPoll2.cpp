#include <miro/Client.h>
#include <idl/RangeSensorC.h>

#include <iostream>

using std::cout;
using std::endl;
using Miro::Client;
using Miro::RangeSensor;
using Miro::RangeSensor_var;
using Miro::RangeScanEventIDL_var;

int main(int argc, char *argv[])
{
  Client client(argc, argv);          // Initialize orb.
  RangeSensor_var sonar =             // Obtain reference to sonar object.
    client.resolveName<RangeSensor>("Sonar"); 
  RangeScanEventIDL_var sonarScan;    // A RangeScanIDL  smart pointer.
  
  sonarScan = sonar->getFullScan();   // Get values of all sonars.

  cout << "Sonar reading: ";          // Print sonar scan.
  for (unsigned int i = 0; i < sonarScan->range.length(); ++i) {
    for (unsigned int j = 0; j < sonarScan->range[i].length(); ++j)
      cout << sonarScan->range[i][j] << "\t";
    cout << endl;
  }
  
  return 0;
}
