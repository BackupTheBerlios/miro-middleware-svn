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
#include "WanderParameters.h"

#include "miro/KeyValueList.h"
#include "miro/Angle.h"

#include <iostream>

WanderParameters::WanderParameters() :
  thresholdChange(0.20),
  thresholdStraight(0.5),
  minTranslation(200),
  maxTranslation(500),
  minRotation(Miro::deg2Rad(10)),
  maxRotation(Miro::deg2Rad(90))
{
  pace = ACE_Time_Value(1);
}

void
WanderParameters::operator <<= (const Miro::KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("ThresholdChange");
  if (str.length() != 0)
    thresholdChange = str.toDouble();
  str = _params.getValue("ThresholdStraight");
  if (str.length() != 0)
    thresholdStraight = str.toDouble();
  str = _params.getValue("MinTranslation");
  if (str.length() != 0)
    minTranslation = str.toInt();
  str = _params.getValue("MaxTranslation");
  if (str.length() != 0)
    maxTranslation = str.toInt();
  str = _params.getValue("MinRotation");
  if (str.length() != 0)
    minRotation = Miro::deg2Rad(str.toDouble());
  str = _params.getValue("MaxRotation");
  if (str.length() != 0)
    maxRotation = Miro::deg2Rad(str.toDouble());
}

void
WanderParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "thresholdChange: " << thresholdChange << endl
       << "thresholdStraight: " << thresholdStraight << endl
       << "minTranslation: " << minTranslation << "mm/s" << endl
       << "maxTranslation: " << maxTranslation << "mm/s" << endl
       << "maxRotation: " << Miro::rad2Deg(maxRotation) << "°/s" << endl;
}
