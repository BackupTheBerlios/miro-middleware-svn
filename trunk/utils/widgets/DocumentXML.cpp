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

#define QT_ALTERNATE_QTSMANIP

#include "DocumentXML.h"

#include "miro/Log.h"

#include <qfile.h>
#include <qtextstream.h>
#include <qobjectlist.h>
#include <qlistview.h>

//------------------------------------------------------------------------------
// public methods
//------------------------------------------------------------------------------

DocumentXML::DocumentXML(QDomDocument const& _document,
			 QListView * _listView, 
			 QObject * _parent, const char * _name) :
  Super(_document, _listView, NULL, _parent, ""),
  document_(_document)
{}

DocumentXML::~DocumentXML()
{}

void
DocumentXML::init(QString const& _rootTag)
{
  clear();

  // init default document
  QDomElement root = document_.createElement(_rootTag);
  QDomNode n = document_.appendChild( root );
  MIRO_ASSERT(!n.isNull());

  listViewItem()->setText(0, _rootTag);
  listViewItem()->setText(2, document_.doctype().name());
  listViewItem()->setOpen(true);

  setModified(false, true);
}

void
DocumentXML::clear()
{
  document_.removeChild(document_.documentElement());
  setModified(true);

  if (children()) {
    QObjectList childList = *children();
    QObject * c = childList.first();
    while (c) {
      delete c;
      c = childList.next();
    }
  }
}

//------------------------------------------------------------------------------
// document methods

void
DocumentXML::setName(const QString& _name)
{
  if (name() != _name) {
    Super::setName(_name);
    setModified(true);
  }
}

void 
DocumentXML::loadXML(const QString& _name) 
{
  clear();
  setModified(false, true);

  QFile f(_name);
  if (!f.open(IO_ReadOnly)) {
    throw QString("DocumentXML::loadXML: Error: file not found!");
  }
  if (!document_.setContent(&f)) {
    throw QString("DocumentXML::loadXML: Error: XML parsing error!");
  }
  Super::setName(_name);
}


void
DocumentXML::saveXML()
{
  QFile f(name() );
  if (!f.open(IO_WriteOnly)) {
    throw QString("DocumentXML::loadXML: Error: file error!");
  }
  QTextStream ts(&f);
  document_.save(ts, 0);
  setModified(false, true);
}
