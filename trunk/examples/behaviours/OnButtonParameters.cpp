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
#include "OnButtonParameters.h"

#include "miro/KeyValueList.h"

#include <iostream>

OnButtonParameters::OnButtonParameters() :
  button(0),
  onPress(true)
{}

void
OnButtonParameters::operator <<= (const Miro::KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("Button");
  if (str.length() != 0)
    button = str.toInt();
  str = _params.getValue("OnRelease");
  if (str.length() != 0)
    onPress = false;
}

void
OnButtonParameters::printToStream(std::ostream& ostr) const
{
  const char * event = (onPress)? "OnPress" : "OnRelease";

  Super::printToStream(ostr);
  ostr << "Button: " << button << " " << event << endl;
}
