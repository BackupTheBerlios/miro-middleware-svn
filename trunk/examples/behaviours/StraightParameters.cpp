// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#include "StraightParameters.h"

#include "miro/KeyValueList.h"
#include "miro/Angle.h"

#include <iostream>

StraightParameters::StraightParameters() :
  translation(500),
  rotation(0.)
{}

void
StraightParameters::operator <<= (const Miro::KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("Translation");
  if (str.length() != 0)
    translation = str.toInt();
  str = _params.getValue("Rotation");
  if (str.length() != 0)
    rotation = Miro::deg2Rad(str.toDouble());
}

void
StraightParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "Translation: " << translation << " mm/s" << std::endl
       << "Rotation: " << Miro::rad2Deg(rotation) << " °/s" << std::endl;
}
