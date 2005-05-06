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
#ifndef ArbiterWidget_h
#define ArbiterWidget_h

#include "ParameterWidget.h"

// forward declarations
class ArbiterXML;

/**
 * This class shows a arbier name for use in PatternWidget
 */
class ArbiterWidget : public ParameterWidget
{
  Q_OBJECT

  typedef ParameterWidget Super;

public:
  ArbiterWidget(ArbiterXML * _arbiter,
		QWidget * _parent, char const * _name);
};

#endif
