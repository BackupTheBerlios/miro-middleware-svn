// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ArbiterWidget.h"
#include "ArbiterXML.h"

ArbiterWidget::ArbiterWidget(ArbiterXML * _arbiter,
			     QWidget * _parent, char const * _name) :
  Super(darkGreen, green,
	_arbiter, _parent, _name)
{
}
