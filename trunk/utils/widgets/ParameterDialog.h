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
#ifndef ParameterDialog_h
#define ParameterDialog_h

#include "DialogXML.h"
#include "params/Type.h"

#include <vector>

// forward declarations
class ParameterEdit;

//! Parameter edit dialogs of the configuration framework.
/** 
 * As every parameter type in the framework is a compound parameter,
 * this dialog is actually a compound parameter dialog.
 */
class ParameterDialog : public DialogXML
{
  Q_OBJECT

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef DialogXML Super;
  //! A vector of edit widgets.
  typedef std::vector<ParameterEdit *> EditVector;

public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  ParameterDialog(Miro::CFG::Type const& _parameterType,
		  QDomNode const& _parentNode, QDomNode const& _node,
		  ItemXML * _parentItem, ItemXML * _item,
		  QWidget * _parent, const char * _name);

  //! Update the xml tree according to the dialog entries.
  virtual void setXML();

protected:
  //----------------------------------------------------------------------------
  // protected methods
  //----------------------------------------------------------------------------

  //! Initialize the dialog.
  void initDialog();

  //! Type description of the underling parameter.
  Miro::CFG::Type parameterType_;
  //! Parameter type elements.
  Miro::CFG::Type::ParameterSet params_;
  
private:
  //! Vector holding all the edit fields of the nested types.
  EditVector editFelds_;
};

#endif
