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
#ifndef ParameterList_h
#define ParameterList_h

#include "ParameterXML.h"
#include "params/Parameter.h"

#include <qstring.h>

// forward declarations
class ConfigFile;
class QListView;
class QListViewItem;

//! Class representing vector and set items. 
class ParameterList : public ParameterXML
{
  Q_OBJECT
  
  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef ParameterXML Super;

public:
  //----------------------------------------------------------------------------
  // public types
  //----------------------------------------------------------------------------

  //! Type of the parameter list.
  enum Type { NONE, VECTOR, SET};

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  ParameterList(Miro::CFG::Parameter const& _param,
		QDomNode const& _node,
		QListViewItem * _parentItem, QListViewItem * _pre,
		QObject * _parent, const char * _name);

  //! Initializing constructor.
  ParameterList(Miro::CFG::Parameter const& _param,
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

  //! Convert type string into a Type.
  static Type typeFromName(QString const& _type);

protected:
  //! Description of the parameter.
  Miro::CFG::Parameter param_;
  //! Type description of the parameter.
  Type type_;
  //! std::vector<int> -> int
  QString nestedTypeName_;

  //! A temporary document, holding a copy of the XML node.
  QDomDocument tmpDocument_;
  //! The parent node in the temporary document.
  QDomNode tmpParentNode_;
  //! The temporary dublicate of the node.
  QDomNode tmpNode_;
};

#endif
