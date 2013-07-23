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
#ifndef ParameterXML_h
#define ParameterXML_h

#include "ItemXML.h"

#include "params/Type.h"

#include <qstring.h>

// forward declarations
class ConfigFile;
class QListView;
class QListViewItem;

//! Base class of all parameter objects of the configuration framework.
class ParameterXML : public ItemXML
{
  Q_OBJECT
  
  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef ItemXML Super;
public:

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  ParameterXML(QDomNode const& _node,
	       QListViewItem * _parentItem, QListViewItem * _pre,
	       QObject * _parent, const char * _name);

  //! Initializing constructor.
  ParameterXML(QDomNode const& _node,
	       QListView * _view, QListViewItem * _pre,
	       QObject * _parent, const char * _name);

  //----------------------------------------------------------------------------
  // public inherited methods
  //----------------------------------------------------------------------------

  //! Inherited method.
  virtual void init();
  //! Inherited method.
  virtual void contextMenu(QPopupMenu& _menu);
  //! Inherited method.
  virtual void setParameters();

  //----------------------------------------------------------------------------
  // public constants
  //----------------------------------------------------------------------------

  //! The parameter tag in the xml tree.
  static QString const XML_TAG;

public slots:
  //----------------------------------------------------------------------------
  // public slots
  //----------------------------------------------------------------------------

  void slotSetParameters();

protected:

  //----------------------------------------------------------------------------
  // puprotected members
  //----------------------------------------------------------------------------

  ConfigFile * config_;

private:
  //----------------------------------------------------------------------------
  // hidden methods
  //----------------------------------------------------------------------------
  ParameterXML(ParameterXML const&);
};

#endif // ParameterXML_h
