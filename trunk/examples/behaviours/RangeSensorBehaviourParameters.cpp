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
  historyMSec(5000),
  sensorName("Sonar")
{}

void
RangeSensorBehaviourParameters::operator <<= (const KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("HistoryMSec");
  if (str.length() != 0)
    historyMSec = str.toInt();
  str = _params.getValue("SensorName");
  if (str.length() != 0)
    sensorName = str;
}

void
RangeSensorBehaviourParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "history: " << historyMSec << "msec" << endl
       << "sensor name: " << sensorName << endl;
}
