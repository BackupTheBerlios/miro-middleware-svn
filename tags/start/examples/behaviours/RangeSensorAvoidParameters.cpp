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
#include "RangeSensorAvoidParameters.h"

#include "miro/KeyValueList.h"
#include "miro/Angle.h"

#include <iostream>

using Miro::KeyValueList;

RangeSensorAvoidParameters::RangeSensorAvoidParameters() :
  historySize(50),
  apexAngle(Miro::deg2Rad(30)),
  minDistance(500),
  haltDistance(300),
  lateralDistance(500),
  translation(200),
  rotation(Miro::deg2Rad(45)),
  sensorName("Sonar")
{}

void
RangeSensorAvoidParameters::operator <<= (const KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("HistorySize");
  if (str.length() != 0)
    historySize = str.toInt();
  str = _params.getValue("MinDistance");
  if (str.length() != 0)
    minDistance = str.toInt();
  str = _params.getValue("HaltDistance");
  if (str.length() != 0)
    haltDistance = str.toInt();
  str = _params.getValue("LateralDistance");
  if (str.length() != 0)
    lateralDistance = str.toInt();
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
RangeSensorAvoidParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "history size: " << historySize << endl
       << "minimal distance: " << minDistance << "mm" << endl
       << "halt distane: " << haltDistance << "mm" << endl
       << "lateral distane: " << lateralDistance << "mm" << endl
       << "translation: " << translation << "mm/s" << endl
       << "rotation:" << rotation << "°/s" << endl 
       << "sensor name: " << sensorName << endl;
}
