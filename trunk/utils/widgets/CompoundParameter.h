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
#ifndef CompoundParameter_h
#define CompoundParameter_h

#include "ParameterXML.h"
#include "params/Type.h"

//! Class representing compound parameter items.
class CompoundParameter : public ParameterXML
{
  Q_OBJECT
  
  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef ParameterXML Super;
public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  CompoundParameter(Miro::CFG::Type const& _type,
		    QDomNode const& _node,
		    QListViewItem * _listViewItem, QListViewItem * _pre,
		    QObject * _parent, const char * _name);

  //! Initializing constructor. For toplevel QListViewItem.
  CompoundParameter(Miro::CFG::Type const& _type,
		    QDomNode const& _node,
		    QListView * _list, QListViewItem * _pre,
		    QObject * _parent, const char * _name);

  //----------------------------------------------------------------------------
  // public inherited methods
  //----------------------------------------------------------------------------

  //! Inherited method.
  virtual void init();
  //! Inherited method.
  virtual void setParameters();

  //----------------------------------------------------------------------------
  // public static methods
  //----------------------------------------------------------------------------

  //! Translates a XML node and name into a CFG::Type.
  static Miro::CFG::Type const& type(QDomNode const& _node,
				     QString const& _name);

protected:
  //! Type description of the compound parameter.
  Miro::CFG::Type type_;
};

#endif
