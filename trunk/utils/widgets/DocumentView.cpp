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

#include "DocumentView.h"
#include "ParameterXML.h"
#include "ItemXML.h"

#include "miro/Log.h"

#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>

namespace 
{
  const char * filters[3] = { 
    "parameter files (*.xml)", 
    "all files (*)", 
    NULL 
  };
}

DocumentView::DocumentView(QWidget * _parent, char const * _name, WFlags _f) :
  Super(_parent, _name, _f),
  titleBar_(NULL),
  statusBar_(NULL),
  document_(NULL),
  rootTag_()
{
  //----------------------------------------------------------------------------
  // init list view

  addColumn("Item Name");
  addColumn("Value");
  addColumn("Type");
  setRootIsDecorated(true);
  setSorting(-1);
  setResizeMode(QListView::AllColumns);

  connect(this, 
	  SIGNAL(contextMenuRequested(QListViewItem *, const QPoint&, int)),
	  this,
	  SLOT(slotContextMenu(QListViewItem *, const QPoint&, int)));
  connect(this, SIGNAL(doubleClicked(QListViewItem *)),
	  this, SLOT(slotDoubleClick(QListViewItem *)));
}

DocumentView::~DocumentView()
{
}

void
DocumentView::setDocument(DocumentXML * _document, QString const& _rootTag)
{
  document_ = _document;
  rootTag_ = _rootTag;

  //----------------------------------------------------------------------------
  // init document //
  newDocument();
}			 

bool
DocumentView::saveIfModified()
{
  bool success = true;

  MIRO_ASSERT(document_ != NULL);

  if (document_->modified()) {
    
    int rc = QMessageBox::warning(this, 
				  "Save if Modified", 
				  QString("The document ") + document_->name() + " has been modified\n" + 
				  "Do you want to save it?",
				  "&Save...", "&Dont't Save", "&Cancel", 0, 2);
    
    switch(rc) {
    case 0: 
      success = saveDocument(); 
      break;
    case 1:
      success = true;
      break; 
    case 2:
      success = false;
    }
  }  
  return success;
}

void 
DocumentView::newDocument()
{
  MIRO_ASSERT(document_ != NULL);

  document_->init(rootTag_);
  
  setTitle("New document.");
  QString message("New document opened.");
  setMessage(message, 3000);
  update();
}

void 
DocumentView::openDocument(const QString& _name)
{
  MIRO_ASSERT(document_ != NULL);

  if (_name.isEmpty())
    return;

  try {
    // load selected XML file
    document_->loadXML(_name);
    document_->parse();
    
    // set new caption and status bar
    setTitle(document_->name());
    QString message(_name + " opened.");
    setMessage(message, 3000);
    update();
  }
  catch (QString const& e) {
    QMessageBox::warning(this, 
			 "Error loading Config file", e,
			     QMessageBox::Ok, QMessageBox::NoButton);
  }
}

bool
DocumentView::saveDocument() 
{
  MIRO_ASSERT(document_ != NULL);

  bool procede = true;
  if (QString(document_->name()).isEmpty()) {
    procede = saveDocumentAs();
  }
  if (procede) {
    document_->saveXML();
    QString message(QString(document_->name()) + " saved.");
    setMessage(message, 3000);
  }
  return procede;
}

bool
DocumentView::saveDocumentAs()
{
  bool rc = true;
  bool selected = false;
  QString filename;

  while (!selected) {
    // show file dialog
    filename = QFileDialog::getSaveFileName(0, "*.xml", this);
	
    if (filename.isEmpty()) {
      rc = false;
      break;
    }

    // append file extension if none is given
    QFileInfo file(filename);
    if (file.extension(false).isEmpty()) {
      filename.append(".xml");
      file.setFile(filename);
    }

    // warn if file exists
    if (!file.exists()) {
      selected = true;
    } 
    else {
      int button = 
	QMessageBox::warning(this, 
			     "DocumentView", 
			     "The file " + filename + " alread exists!\n" +
			     "Do you want to overwrite the existing file?",
			     "&Yes", "&No", QString::null, 1);
      
      if (button == 0)
	selected = true;
    }
  }

  // set new caption and document name
  setTitle(filename);
  document_->setName(filename);
  return rc;
}

void 
DocumentView::slotNew()
{
  if (saveIfModified()) {
    newDocument();
  }
}


void 
DocumentView::slotLoad()
{
  if (saveIfModified()) {
    QString filename =
      QFileDialog::getOpenFileName(0, 
				   "Polycies *.xml\nAlle Dateien *", 
				   this);
    if (filename.isNull())
      return;

    openDocument(filename);
  }
}

void 
DocumentView::slotSave()
{
  saveDocument();
}
  

void
DocumentView::slotSaveAs()
{
  if (saveDocumentAs()) {
    saveDocument();
  }
}

void 
DocumentView::slotContextMenu(QListViewItem * _item, const QPoint & _pos, int)
{
  if (_item == NULL)
    return;

  ItemXML::ItemMap::const_iterator item = ItemXML::itemMap().find(_item);
  MIRO_ASSERT(item != ItemXML::itemMap().end());

  QPopupMenu menu(NULL, "conext_menu");
  item->second->contextMenu(menu);

  if (menu.count() > 0)
    menu.exec(_pos);
}

void
DocumentView::slotDoubleClick(QListViewItem * _item)
{
  if (_item->firstChild() == NULL) {
    ItemXML::ItemMap::const_iterator item = ItemXML::itemMap().find(_item);
    MIRO_ASSERT(item != ItemXML::itemMap().end());
    
    ParameterXML * param =  dynamic_cast<ParameterXML *>(item->second);
    if (param != NULL) {
      try {
	param->setParameters();
      }
      catch (QString const& e) {
	QMessageBox::warning(this, 
			     "Error editing Parameters", e,
			     QMessageBox::Ok, QMessageBox::NoButton);
      }
    }
  }
}

void 
DocumentView::tryClose(QCloseEvent *e)
{
  if (saveIfModified()) {
    e->accept();
  }
  else {
    e->ignore(); 
  }
}

void 
DocumentView::setMessage(QString const& _message, int _ms)
{
  if (statusBar_)
    statusBar_->message(_message, _ms);
}

void 
DocumentView::setTitle(QString const& _title)
{
  if (titleBar_)
    titleBar_->setCaption(_title);
}
