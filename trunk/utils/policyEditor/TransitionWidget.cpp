// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "TransitionWidget.h"
#include "TransitionContainer.h"
#include "TransitionXML.h"

TransitionWidget::TransitionWidget(TransitionXML * _transition,
				   TransitionContainer * _parent, char * _name) :
  Super(_parent, _name),
  transition_(_transition)
{
  setText(transition_->name());
}

TransitionWidget::~TransitionWidget()
{
}
