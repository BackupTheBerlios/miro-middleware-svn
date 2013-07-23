// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
};

#endif
