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
#include "TurnRightParameters.h"

#include "miro/KeyValueList.h"
#include "miro/Angle.h"

#include <iostream>

using Miro::KeyValueList;

TurnRightParameters::TurnRightParameters() :
  minDistance(500),
  haltDistance(300),
  leftLaser(270),
  rightLaser(90),
  translation(200),
  rotation(Miro::deg2Rad(45))
{}

void
TurnRightParameters::operator <<= (const KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("MinDistance");
  if (str.length() != 0)
    minDistance = str.toInt();
  str = _params.getValue("HaltDistance");
  if (str.length() != 0)
    haltDistance = str.toInt();
  str = _params.getValue("LeftLaser");
  if (str.length() != 0)
    leftLaser = str.toInt();
  str = _params.getValue("RightLaser");
  if (str.length() != 0)
    rightLaser = str.toInt();
  str = _params.getValue("Translation");
  if (str.length() != 0)
    translation = str.toInt();
  str = _params.getValue("Rotation");
  if (str.length() != 0)
    rotation = Miro::deg2Rad(str.toDouble());
}

void
TurnRightParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "minimal distance: " << minDistance << endl
       << "halt distane: " << haltDistance << endl
       << "left laser: " << leftLaser << endl
       << "right laser: " << rightLaser << endl
       << "translation: " << translation << "mm/s" << endl
       << "rotation:" << rotation << "°/s" << endl;
}
