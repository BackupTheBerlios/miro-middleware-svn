#include <string>
#include <iostream>
#include <stdio.h>
#include "Xbus.h"
#include "XsensInertialSensor.h"



int main(int argc, char *argv[])
{
  std::string dev = "/dev/ttyUSB0";

  Inertial::XsensInertialSensorReader xsens( dev, OUTPUTMODE_ORIENT, OUTPUTSETTINGS_EULER );

  xsens.setSensorMode();

  for ( int i = 0; i < 1000; i++ )
  {
    xsens.readData();
  }
  //std::cout << orient << std::endl;

  std::cout << "huhu" << std::endl;
}
