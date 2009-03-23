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
#include "ParameterXML.h"
#include "ConfigFile.h"

#include "miro/Log.h"

#include <qpopupmenu.h>
#include <qlistview.h>

QString const ParameterXML::XML_TAG = "parameter";

ParameterXML::ParameterXML(QDomNode const& _node,
			   QListViewItem * _parentItem, QListViewItem * _pre,
			   QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name),
  config_(ConfigFile::instance())
{
  MIRO_ASSERT(!node().toElement().isNull());
}

ParameterXML::ParameterXML(QDomNode const& _node,
			   QListView * _view, QListViewItem * _pre,
			   QObject * _parent, const char * _name) :
  Super(_node, _view, _pre, _parent, _name),
  config_(ConfigFile::instance())
{
  MIRO_ASSERT(!node().toElement().isNull());
}

void 
ParameterXML::init()
{
}

void
ParameterXML::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Set Parameters", this, SLOT(slotSetParameters()));
  _menu.insertSeparator();
  _menu.insertItem("Up", this, SLOT(up()));
  _menu.insertItem("Down", this, SLOT(down()));
  _menu.insertSeparator();
  _menu.insertItem("Delete", this, SLOT(slotDelete()));
}

void
ParameterXML::slotSetParameters()
{
  setParameters();
}

void
ParameterXML::setParameters()
{
}
