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
