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

//! Dialog for editing a single parameter value.
class SingleParameterDialog : public DialogXML
{
  Q_OBJECT

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef DialogXML Super;

public:
  //----------------------------------------------------------------------------
  // pbulic methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  SingleParameterDialog(Miro::CFG::Parameter const & _parameter,
			QDomNode const& _node,
			ItemXML * _parentItem,
			ItemXML * _item,
			QWidget * _parent, const char * _name);

  //! Inherited method.
  virtual void setXML();

protected:
  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! The parameter description.
  Miro::CFG::Parameter parameter_;
  //! The edit field.
  ParameterEdit * edit_;
};

#endif
