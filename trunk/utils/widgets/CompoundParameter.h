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

class CompoundParameter : public ParameterXML
{
  Q_OBJECT
  
  typedef ParameterXML Super;
public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  CompoundParameter(Miro::CFG::Type const& _type,
		    QDomNode const& _node,
		    QListViewItem * _listViewItem, QListViewItem * _pre,
		    QObject * _parent, const char * _name);

  CompoundParameter(Miro::CFG::Type const& _type,
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

  static Miro::CFG::Type const& type(QDomNode const& _node,
				     QString const& _name);

protected:
  Miro::CFG::Type type_;
};

#endif
