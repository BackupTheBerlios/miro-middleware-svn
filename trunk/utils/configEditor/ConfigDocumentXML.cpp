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

#include "ConfigDocumentXML.h"
#include "Section.h"

#include "utils/widgets/ConfigFile.h"

#include "params/Generator.h"

#include "miro/Log.h"

#include <qlistview.h>
#include <qpopupmenu.h>
#include <qstring.h>

#include <algorithm>
#include <iostream>

const QString ConfigDocumentXML::XML_DOCTYPE("MiroConfigDocument");
const QString ConfigDocumentXML::XML_TAG("config");


using std::cout;
using std::endl;

//------------------------------------------------------------------------------
// public methods

ConfigDocumentXML::ConfigDocumentXML(QDomDocument const& _document, 
				     QListView * _listView,
				     QObject * _parent, const char * _name) :
  Super(_document, _listView, _parent, _name),
  menuAddSection_(NULL)
{
}

ConfigDocumentXML::~ConfigDocumentXML()
{}

//----------------------------------------------------------------------------  
// inherited public methods

void
ConfigDocumentXML::contextMenu(QPopupMenu& _menu)
{
  menuAddSection_ = new QPopupMenu(&_menu);

  _menu.insertItem("Add Section", menuAddSection_);

  Miro::CFG::Generator::QStringVector childSections;
  QListViewItem * item = listViewItem()->firstChild();
  while (item != NULL) {
    childSections.push_back(item->text(0));
    item = item->nextSibling();
  }

  // submenu: add all section names
  // not yet available in the document
  Miro::CFG::Generator::QStringVector sections =
    ConfigFile::instance()->description().groups();
  Miro::CFG::Generator::QStringVector::const_iterator first, last = sections.end();
  for (first = sections.begin(); first != last; ++first) {
    Miro::CFG::Generator::QStringVector::const_iterator i = 
      std::find(childSections.begin(), childSections.end(), *first);
    if (i == childSections.end()) {
      menuAddSection_->insertItem(*first);
    }
  }
  connect(menuAddSection_, SIGNAL(activated(int)),
	  this, SLOT(onAddSection(int)));
}

//----------------------------------------------------------------------------  
// public slots

void 
ConfigDocumentXML::onAddSection(int _n)
{
  QDomElement config = document_.documentElement();
  cout << config.tagName() << endl;
  QDomElement e = document_.createElement(Section::XML_TAG);
  e.setAttribute(Section::XML_ATTRIBUTE_KEY, menuAddSection_->text(_n));

  QDomNode n = config.firstChild();
  if (!n.isNull())
    cout << n.toElement().tagName() << endl;
  QDomNode newChild; 
  if (true || config.firstChild().isNull())
    newChild = config.appendChild(e);
  else
    newChild = config.insertBefore(e, n);

  MIRO_ASSERT(!newChild.isNull());
  new Section(newChild, 
	      listViewItem(), NULL, 
	      this, menuAddSection_->text(_n));
  setModified();
}

//------------------------------------------------------------------------------
// protected methods

void
ConfigDocumentXML::parse()
{
  QDomNode n = document_.firstChild();
  if (!n.isNull()) {
    QDomNode n1 = n.firstChild();
    QListViewItem * pre = NULL;
    while (!n1.isNull()) {
      QDomElement e = n1.toElement();
      if (!e.isNull() &&
	  e.tagName() == Section::XML_TAG) {
	Section * section =
	  new Section(e, 
		      listViewItem(), pre, 
		      this, e.attribute("name"));
	pre = section->listViewItem();
      }
      n1 = n1.nextSibling();
    }
  }
}
