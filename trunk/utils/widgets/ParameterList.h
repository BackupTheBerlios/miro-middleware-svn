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

class ParameterList : public ParameterXML
{
  Q_OBJECT
  
  typedef ParameterXML Super;

public:
  //----------------------------------------------------------------------------
  // public types
  //----------------------------------------------------------------------------

  enum Type { NONE, VECTOR, SET};

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  ParameterList(Miro::CFG::Parameter const& _param,
		QDomNode const& _node,
		QListViewItem * _parentItem, QListViewItem * _pre,
		QObject * _parent, const char * _name);

  ParameterList(Miro::CFG::Parameter const& _param,
		QDomNode const& _node,
		QListView * _list, QListViewItem * _pre,
		QObject * _parent, const char * _name);

  //----------------------------------------------------------------------------
  // public inherited methods
  //----------------------------------------------------------------------------

  virtual void init();
  virtual void setParameters();

  //----------------------------------------------------------------------------
  // public static methods
  //----------------------------------------------------------------------------

  static Type typeFromName(QString const& _type);

protected:
  Miro::CFG::Parameter param_;
  Type type_;
  QString nestedTypeName_;

  QDomDocument tmpDocument_;
  QDomNode tmpParentNode_;
  QDomNode tmpNode_;
};

#endif
