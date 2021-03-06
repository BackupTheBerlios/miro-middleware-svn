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
#include "StraightParameters.h"

#include "miro/KeyValueList.h"

#include <iostream>

StraightParameters::StraightParameters() :
  translation(500)
{}

void
StraightParameters::operator <<= (const Miro::KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("Translation");
  if (str.length() != 0)
    translation = str.toInt();
}

void
StraightParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "Translation: " << translation << " mm/s" << endl;
}
