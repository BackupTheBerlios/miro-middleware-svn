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

#include "MainWindow.h"
#include "ParameterInstance.h"

#include "utils/widgets/DocumentView.h"
#include "utils/widgets/ConfigFile.h"
#include "utils/widgets/FileListDialog.h"

#include "miro/Log.h"

#include <qfiledialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpushbutton.h>

namespace 
{
  const char * filters[3] = { 
    "parameter files (*.xml)", 
    "all files (*)", 
    NULL 
  };
}

MainWindow::MainWindow() :
  Super(NULL, "MainWindow"),
  config_(ConfigFile::instance()),
  view_(new DocumentView(this, "config_list")),
  document_(QDomDocument(ConfigDocumentXML::XML_DOCTYPE), view_),
  configDialog_(new FileListDialog(this, "Config dialog", "Parameter description files", filters))
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
  //  std::cout << "deleting main window" << std::endl;
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
