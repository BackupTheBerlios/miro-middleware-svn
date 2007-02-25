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
#include "ParameterInstance.h"

#include "utils/widgets/DocumentView.h"
#include "utils/widgets/ConfigFile.h"
#include "utils/widgets/FileListDialog.h"

#include "idl/ConfigC.h"

#include "miro/Log.h"
#include "miro/Client.h"

#include <qfiledialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qinputdialog.h>

namespace 
{
  const char * filters[3] = { 
    "parameter files (*.xml)", 
    "all files (*)", 
    NULL 
  };
}

MainWindow::MainWindow(Miro::Client& _client) :
  Super(NULL, "MainWindow"),
  config_(ConfigFile::instance()),
  view_(new DocumentView(this, "config_list")),
  document_(QDomDocument(ConfigDocumentXML::XML_DOCTYPE), view_),
  configDialog_(new FileListDialog(this, "Config dialog", "Parameter description files", filters)),
  client_(_client),
  interfaceName_("Video")
{
  resize(400, 300);
  //-----------//
  // init menu //
  //-----------//

  // file menu
  QPopupMenu * menuFile = new QPopupMenu();
  menuBar()->insertItem("&File", menuFile);

  menuFile->insertItem("New", view_, SLOT(slotNew()));
  menuFile->insertItem("Open ...", view_, SLOT(slotLoad()));
  menuFile->insertItem("Save", view_, SLOT(slotSave())); 
  menuFile->insertItem("Save As ...", view_, SLOT(slotSaveAs()));
  menuFile->insertSeparator();
  menuFile->insertItem("Get from ...", this, SLOT(slotGetFrom()));
  menuFile->insertItem("Send to ...", this, SLOT(slotSendTo()));
  menuFile->insertSeparator();
  menuFile->insertItem("Quit", this, SLOT(close()));

  // options menue
  QPopupMenu* menuOptions = new QPopupMenu();
  menuBar()->insertItem("&Options", menuOptions);

  menuOptions->insertItem("&Parameter descriptions ...", 
			  this, SLOT(paramsDescriptions()));

  // help menu
  QPopupMenu* menuHelp = new QPopupMenu();
  menuBar()->insertSeparator();
  menuBar()->insertItem("&Help", menuHelp);
  
  menuHelp->insertItem("About ConfigEditor", this, SLOT(slotAbout()));
  menuHelp->insertItem("About Qt", this, SLOT(slotAboutQt()));
  
  //-----------//
  // init view //
  //-----------//

  view_->setTitleBar(this);
  view_->setStatusBar(statusBar());
  view_->setDocument(&document_, ConfigDocumentXML::XML_TAG);

  setCentralWidget(view_);
}

MainWindow::~MainWindow()
{
}

void 
MainWindow::slotGetFrom()
{
  bool ok = false;
  QString tmp = QInputDialog::getText(tr( "Get configuration" ),
				      tr( "Interface name" ),
				      QLineEdit::Normal, interfaceName_, &ok, this );

  if ( ok && !tmp.isEmpty() ) {
    QString error;

    interfaceName_ = tmp;
    try {
      Miro::Config_var config = client_.resolveName<Miro::Config>(interfaceName_.latin1());

      char * param;
      char * type;
      char * document;
      config->getParameters(param, type, document);
      view_->openDocumentXML(param, document);
    }
    catch(const CORBA::Exception& e) {
      std::ostringstream sstr;
      sstr << "Communication Failed. CORBA exception: " << e << std::flush;

      error = sstr.str().c_str();
      ok = false;
    }

    if (!ok) {
      QMessageBox::warning(this, "Couln't get configuration:", error);
    }

  }
}

void 
MainWindow::slotSendTo()
{
  bool ok = false;
  QString tmp = QInputDialog::getText(tr( "Send configuration" ),
				      tr( "Interface name" ),
				      QLineEdit::Normal, interfaceName_, &ok, this );

  if ( ok && !tmp.isEmpty() ) {
    QString error;

    interfaceName_ = tmp;
    try {
      Miro::Config_var config = client_.resolveName<Miro::Config>(interfaceName_.latin1());

      config->setParameters(document_.domDocument().toCString());
    }
    catch(const Miro::EOutOfBounds& e) {
      error = QString("Error parsing policy: ") + e.what;
      ok = false;
    }
    catch(const CORBA::Exception& e) {
      std::ostringstream sstr;
      sstr << "Communication Failed. CORBA exception: " << e << std::flush;

      error = sstr.str().c_str();
      ok = false;
    }

    if (!ok) {
      QMessageBox::warning(this, "Couln't send configuration:", error);
    }

  }
}

void
MainWindow::paramsDescriptions()
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
		     "About Config Editor", 
		     "Config Editor Ver. 1.0\nCopyright 2004 Hans Utz");

}

void 
MainWindow::slotAboutQt()
{
  QMessageBox::aboutQt(this, "About Qt");
}

void 
MainWindow::closeEvent(QCloseEvent *e)
{
  view_->tryClose(e);
}
