#include <miro/Client.h>
#include <idl/RangeSensorC.h>

#include <iostream>

int main(int argc, char *argv[])
{
  Miro::Client client(argc, argv);     // Initialize orb.
  Miro::RangeSensor_var sonar =        // Obtain reference to sonar object.
    client.resolveName<Miro::RangeSensor>("Sonar"); 
  Miro::RangeScanEventIDL_var sonarScan;    // A RangeScanIDL smart pointer.
  
  sonarScan = sonar->getFullScan();    // Get values of all sonars.

  std::cout << "Sonar reading: ";      // Print sonar scan.
  for (unsigned int i = 0; i < sonarScan->range.length(); ++i) {
    for (unsigned int j = 0; j < sonarScan->range[i].length(); ++j)
      std::cout << sonarScan->range[i][j] << "\t";
    std::cout << std::endl;
  }
  
  return 0;
}
