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
#include "RangeSensorBehaviourParameters.h"

#include "miro/KeyValueList.h"
#include "miro/Angle.h"

#include <iostream>

using Miro::KeyValueList;

RangeSensorBehaviourParameters::RangeSensorBehaviourParameters() :
  historySize(50),
  apexAngle(Miro::deg2Rad(30)),
  translation(400),
  rotation(Miro::deg2Rad(45)),
  sensorName("Sonar")
{}

void
RangeSensorBehaviourParameters::operator <<= (const KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("HistorySize");
  if (str.length() != 0)
    historySize = str.toInt();
  str = _params.getValue("Translation");
  if (str.length() != 0)
    translation = str.toInt();
  str = _params.getValue("Rotation");
  if (str.length() != 0)
    rotation = Miro::deg2Rad(str.toDouble());
  str = _params.getValue("SensorName");
  if (str.length() != 0)
    sensorName = str;
}

void
RangeSensorBehaviourParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "history size: " << historySize << endl
       << "translation: " << translation << "mm/s" << endl
       << "rotation:" << Miro::rad2Deg(rotation) << "°/s" << endl 
       << "sensor name: " << sensorName << endl;
}
