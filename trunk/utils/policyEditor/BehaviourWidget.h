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
#ifndef BehaviourWidget_h
#define BehaviourWidget_h

#include "ParameterWidget.h"

// forward declarations
class BehaviourXML;

/**
 * This class shows a behaviour name for use in PatternWidget
 */
class BehaviourWidget : public ParameterWidget
{
  Q_OBJECT

  typedef ParameterWidget Super;

public:
  BehaviourWidget(BehaviourXML * _behaviour,
		  QWidget * _parent, char const * _name);

};

#endif // BehaviourWidget_h
