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
#include "SonarAvoidParameters.h"

#include "miro/KeyValueList.h"
#include "miro/Angle.h"

#include <iostream>

using Miro::KeyValueList;

SonarAvoidParameters::SonarAvoidParameters() :
  minDistance(500),
  haltDistance(300),
  leftSonar(8),
  rightSonar(15),
  translation(200),
  rotation(Miro::deg2Rad(45))
{}

void
SonarAvoidParameters::operator <<= (const KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("MinDistance");
  if (str.length() != 0)
    minDistance = str.toInt();
  str = _params.getValue("HaltDistance");
  if (str.length() != 0)
    haltDistance = str.toInt();
  str = _params.getValue("LeftSonar");
  if (str.length() != 0)
    leftSonar = str.toInt();
  str = _params.getValue("RightSonar");
  if (str.length() != 0)
    rightSonar = str.toInt();
  str = _params.getValue("Translation");
  if (str.length() != 0)
    translation = str.toInt();
  str = _params.getValue("Rotation");
  if (str.length() != 0)
    rotation = Miro::deg2Rad(str.toDouble());
}

void
SonarAvoidParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "minimal distance: " << minDistance << endl
       << "halt distane: " << haltDistance << endl
       << "left sonar: " << leftSonar << endl
       << "right sonar: " << rightSonar << endl
       << "translation: " << translation << "mm/s" << endl
       << "rotation:" << rotation << "°/s" << endl;
}
