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
#ifndef SingleParameterDialog_h
#define SingleParameterDialog_h

#include "DialogXML.h"
#include "params/Parameter.h"

#include <vector>

// forward declarations
class ParameterEdit;
class ConfigFile;

/**
 * This class shows a dialog for the behaviour parameters.
 */
class SingleParameterDialog : public DialogXML
{
  Q_OBJECT

  typedef DialogXML Super;

public:
  SingleParameterDialog(Miro::CFG::Parameter const & _parameter,
			QDomNode const& _node,
			ItemXML * _parentItem,
			ItemXML * _item,
			QWidget * _parent, const char * _name);

  virtual void setXML();

protected:
  Miro::CFG::Parameter parameter_;
  ParameterEdit * edit_;
};

#endif
