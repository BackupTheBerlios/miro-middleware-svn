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
#include "WallFollowParameters.h"

#include "miro/KeyValueList.h"
#include "miro/Angle.h"

#include <iostream>

using Miro::KeyValueList;

WallFollowParameters::WallFollowParameters() :
  minDistance(600),
  maxDistance(800)
{}

void
WallFollowParameters::operator <<= (const KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("MinDistance");
  if (str.length() != 0)
    minDistance = str.toInt();
  str = _params.getValue("MaxDistane");
  if (str.length() != 0)
    maxDistance = str.toInt();
}

void
WallFollowParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "minimal distance: " << minDistance << "mm" << endl
       << "maximum distane: " << maxDistance << "mm" << endl;
}
