// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef DeferredParameterEdit_h
#define DeferredParameterEdit_h

#include "ParameterEdit.h"

// forward declarations
class ConfigFile;
class QPushButton;
class QString;

//! Deferred parameter edit field.
/*
 * The edit field is actually a button that, when pressen, launches
 * another input dialog. This is necessary for nested compound types,
 * vectors and sets.
 */
class DeferredParameterEdit : public ParameterEdit
{
  Q_OBJECT;

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef ParameterEdit Super;

public:
  //----------------------------------------------------------------------------
  // public types
  //----------------------------------------------------------------------------

  //! Contained type of the deferred edit field.
  enum EditType { NESTED_PARAMETER, VECTOR, SET };

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  DeferredParameterEdit(EditType _type,
			Miro::CFG::Parameter const& _parameter,
			QDomNode& _parentNode, QDomNode& _node,
			ItemXML * _parentItem, ItemXML * _item,
			QWidget * _parent, const char * _name);

  //! Inherited method.
  virtual void setXML();
  //! Inherited method.
  virtual bool modified() const;

  //! Translate the type name into an edit type.
  static EditType editType(QString const& _name);

protected slots:
  //----------------------------------------------------------------------------
  // protected slots
  //----------------------------------------------------------------------------

  //! Launch deferred input dialog.
  void deferredEdit();

protected:
  //----------------------------------------------------------------------------
  // protected data
  //----------------------------------------------------------------------------

  //! Type information on the parameter.
  EditType type_;
  //! Flag indicating if the dialog is modified.
  bool modified_;

  //! A temporary document, holding a copy of the XML node.
  QDomDocument tmpDocument_;
  //! The parent node in the temporary document.
  QDomNode tmpParentNode_;
  //! The temporary dublicate of the node.
  QDomNode tmpNode_;
};

#endif // DeferredParameterEdit_h
