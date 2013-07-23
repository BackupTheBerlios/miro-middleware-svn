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
#include "MainWindow.h"
#include "PolicyView.h"
#include "PolicyXML.h"

#include "utils/widgets/DocumentView.h"
#include "utils/widgets/ConfigFile.h"
#include "utils/widgets/FileListDialog.h"

#include "idl/BehaviourEngineC.h"

#include "miro/Client.h"
#include "miro/Log.h"

#include <qfiledialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qinputdialog.h>
#include <qsplitter.h>

#include <iostream>
#include <sstream>

namespace 
{
  const char * filters[3] = { 
    "config files (*.xml)", 
    "all files (*)", 
    NULL 
  };
}


MainWindow::MainWindow(Miro::Client& _client) :
  QMainWindow(NULL, "MainWindow"),
  client_(_client),
  config_(ConfigFile::instance()),
  splitter_(new QSplitter(Qt::Horizontal, this, "main window")),
  documentView_(new DocumentView(splitter_, "config list")),
  document_(QDomDocument(PolicyDocumentXML::XML_DOCTYPE), documentView_),
  policyView_(new PolicyView(splitter_)),
  configDialog_(new FileListDialog(this, "Config dialog", "Behaviour description files", filters)),
  robot_(getenv("HOST"))
{
  resize(600, 500);

  //-----------//
  // init menu //
  //-----------//

  // file menu
  QPopupMenu* menuFile = new QPopupMenu();
  menuBar()->insertItem("&File", menuFile);

  menuFile->insertItem("New",         documentView_, SLOT(slotNew()));
  menuFile->insertItem("Open ...",    documentView_, SLOT(slotLoad()));
  menuFile->insertItem("Save",        documentView_, SLOT(slotSave())); 
  menuFile->insertItem("Save As ...", documentView_, SLOT(slotSaveAs()));
  menuFile->insertSeparator();
  menuFile->insertItem("Send to ...", this, SLOT(slotSendTo()));
  menuFile->insertSeparator();
  menuFile->insertItem("Quit",        this, SLOT(close()));

  // options menue
  QPopupMenu* menuOptions = new QPopupMenu();
  menuBar()->insertItem("&Options", menuOptions);

  menuOptions->insertItem("&Behaviour descriptions ...", 
			  this, SLOT(behaviourDescriptions()));

  // help menu
  QPopupMenu* menuHelp = new QPopupMenu();
  menuBar()->insertSeparator();
  menuBar()->insertItem("&Help", menuHelp);
  
  menuHelp->insertItem("About MainWindow", this, SLOT(slotAbout()));
  menuHelp->insertItem("About Qt", this, SLOT(slotAboutQt()));
  

  //-----------//
  // init view //
  //-----------//

  documentView_->setTitleBar(this);
  documentView_->setStatusBar(statusBar());
  documentView_->setDocument(&document_, PolicyDocumentXML::XML_TAG);

  connect(documentView_, SIGNAL(selectionChanged(QListViewItem *)),
	  this, SLOT(slotNewSelection(QListViewItem *)));

  setCentralWidget(splitter_);
}

MainWindow::~MainWindow()
{}

void 
MainWindow::slotSendTo()
{
  bool ok = false;
  QString tmp = QInputDialog::getText(tr( "Send policy" ),
				      tr( "Robot name" ),
				      QLineEdit::Normal, robot_, &ok, this );

  if ( ok && !tmp.isEmpty() ) {
    QString error;

    robot_ = tmp;

    // build the lookup string for the naming service
    CosNaming::Name name;
    name.length(2);
    name[0].id = CORBA::string_dup(robot_.latin1());
    name[1].id = CORBA::string_dup("BehaviourEngine");

    try {
      Miro::BehaviourEngine_var engine = client_.resolveName<Miro::BehaviourEngine>(name);

      engine->loadPolicy(document_.domDocument().toCString());
    }
    catch(const Miro::BehaviourEngine::EMalformedPolicy& e) {
      error = QString("Error parsing policy: ") + e.what;
      ok = false;
    }
    catch(const Miro::BehaviourEngine::EMalformedXML& ) {
      error = "Malformed XML.";
      ok = false;
    }
    catch(const CORBA::Exception& e) {
      std::ostringstream sstr;
      sstr << "Communication Failed. CORBA exception: " << e << std::flush;

      error = sstr.str().c_str();
      ok = false;
    }

    if (!ok) {
      QMessageBox::warning(this, "Couln't send policy:", error);
    }

  }
}

void
MainWindow::behaviourDescriptions()
{
  configDialog_->init(config_->getDescriptionFiles());
  if (configDialog_->exec() &&
      configDialog_->modified()) {
    QStringList list = configDialog_->result();
    config_->setDescriptionFiles(list);
  }
}

void 
MainWindow::slotAbout()
{
  QMessageBox::about(this, 
		     "About Policy Editor", 
		     "Policy Editor Ver. 2.0\nCopyright 1999 - 2004 Stefan Enderle, Markus Lauer, Hans Utz");

}

void 
MainWindow::slotAboutQt()
{
  QMessageBox::aboutQt(this, "About Qt");
}

void
MainWindow::slotNewSelection(QListViewItem * _item)
{
  PolicyXML * policy = NULL;
  while (_item != NULL &&
	 policy == NULL) {
    policy = dynamic_cast<PolicyXML *>(Item::itemFromListViewItem(_item));
    _item = _item->parent();
  }
  policyView_->init(policy);
}

void 
MainWindow::closeEvent(QCloseEvent *e)
{
  documentView_->tryClose(e);
}
