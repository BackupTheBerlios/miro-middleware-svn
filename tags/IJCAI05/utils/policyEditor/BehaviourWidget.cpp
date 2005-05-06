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

#include "BehaviourWidget.h"
#include "BehaviourXML.h"

BehaviourWidget::BehaviourWidget(BehaviourXML * _behaviour,
				 QWidget * _parent, char const * _name) :
  Super(lightGray, cyan,
	_behaviour, _parent, _name)
{}
