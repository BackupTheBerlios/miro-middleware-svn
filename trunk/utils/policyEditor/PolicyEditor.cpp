// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1998, 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PolicyEditor.h"
#include "PolicyView.h"
#include "ParameterDialog.h"

#include "../widgets/FileListDialog.h"

#include "miro/BehaviourEngineC.h"

#include <qapplication.h>
#include <qfiledialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qstatusbar.h>
#include <qinputdialog.h>

#include <iostream>
#include <sstream>

PolicyEditorClass::PolicyEditorClass(Miro::Client& _client) :
  QMainWindow(NULL, "PolicyEditor"),
  client_(_client),
  config_(*PolicyConfigClass::instance()),
  view_(new PolicyViewClass(this, document_)),
  configDialog_(new FileListDialog(this)),
  robot_(getenv("HOST"))
{
  resize(600, 500);

  //-----------//
  // init menu //
  //-----------//

  // file menu
  QPopupMenu* menuFile = new QPopupMenu();
  menuBar()->insertItem("&File", menuFile);

  menuFile->insertItem("New",         this, SLOT(slotNew()));
  menuFile->insertItem("Open ...",    this, SLOT(slotLoad()));
  menuFile->insertItem("Save",        this, SLOT(slotSave())); 
  menuFile->insertItem("Save As ...", this, SLOT(slotSaveAs()));
  menuFile->insertSeparator();
  menuFile->insertItem("Send to ...", this, SLOT(slotSendTo()));
  menuFile->insertSeparator();
  menuFile->insertItem("Quit",        this, SLOT(close()));

  // options menue
  QPopupMenu* menuOptions = new QPopupMenu();
  menuBar()->insertItem("&Options", menuOptions);

  //menuOptions->insertItem("&Configuration ..",  this, SLOT(slotConfiguration()));
  //  MenuEdit->insertItem("Add Basic Behaviour ..", this, NULL);
  menuOptions->insertItem("&Behaviour descriptions ...", this, SLOT(behaviourDescriptions()));

  // help menu
  QPopupMenu* menuHelp = new QPopupMenu();
  menuBar()->insertSeparator();
  menuBar()->insertItem("&Help", menuHelp);
  
  menuHelp->insertItem("About PolicyEditor", this, SLOT(slotAbout()));
  menuHelp->insertItem("About Qt",           this, SLOT(slotAboutQt()));
  
  //-----------//
  // init view //
  //-----------//
  setCentralWidget(view_);

  //---------------//
  // init document //
  //---------------//

  newPolicy();
}

// update the view
void
PolicyEditorClass::defaultView()
{
  view_->update();     
}

bool
PolicyEditorClass::saveIfModified()
{
  bool success = true;

  if (document_.getModified()) {
    
    int rc = QMessageBox::warning(this, 
				  "Save if Modified", 
				  "The document " + document_.getDocumentName() + " has been modified\n" + 
				  "Do you want to save it?",
				  "&Save...", "&Dont't Save", "&Cancel", 0, 2);
    
    switch(rc) {
    case 0: 
      success = savePolicy(); 
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
PolicyEditorClass::newPolicy()
{
  document_.init();
  view_->init();
  
  setCaption("New document.");
  QString message("New document opend.");
  statusBar()->message(message, 3000);
  defaultView();
}

void 
PolicyEditorClass::openPolicy(const QString& _name)
{
  if (_name.isEmpty())
    return;

  // load selected XML file
  document_.loadXML(_name);
  view_->init();

  // set new caption and status bar
  setCaption(document_.getDocumentName());
  QString message(_name + " opened.");
  statusBar()->message(message, 3000);
  defaultView();
}

bool
PolicyEditorClass::savePolicy() 
{
  bool procede = true;
  if (document_.getDocumentName().isEmpty()) {
    procede = savePolicyAs();
  }
  if (procede) {
    document_.saveXML();
    QString message(document_.getDocumentName() + " saved.");
    statusBar()->message(message, 3000);
  }
  return procede;
}

bool
PolicyEditorClass::savePolicyAs()
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
      int button = QMessageBox::warning(this, 
					"PolicyEditor", 
					"The file " + filename + " alread exists!\n" +
					"Do you want to overwrite the existing file?",
					"&Yes", "&No", QString::null, 1);
      
      if (button == 0)
	selected = true;
    }
  }

  // set new caption and document name
  setCaption(filename);
  document_.setDocumentName(filename);

  return rc;
}

void 
PolicyEditorClass::slotNew()
{
  if (saveIfModified())
    newPolicy();
}


void 
PolicyEditorClass::slotLoad()
{
  if (saveIfModified()) {
    QString filename = QFileDialog::getOpenFileName(0, 
						    "Polycies *.xml\nAlle Dateien *", 
						    this);
    if (filename.isNull())
      return;

    openPolicy(filename);
  }
}

void 
PolicyEditorClass::slotSave()
{
  savePolicy();
}
  

void
PolicyEditorClass::slotSaveAs()
{
  if (savePolicyAs()) {
    savePolicy();
  }
}

void 
PolicyEditorClass::slotSendTo()
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

      engine->loadPolicy(document_.getDomDocument().toCString());
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
      sstr << "Communication Failed. CORBA exception: " << e << flush;

      error = sstr.str().c_str();
      ok = false;
    }

    if (!ok) {
      QMessageBox::warning(this, "Couln't send policy:", error);
    }

  }
}

void
PolicyEditorClass::behaviourDescriptions()
{
  PolicyConfigClass * config = PolicyConfigClass::instance();
  configDialog_->init(config->getDescriptionFiles());
  if (configDialog_->exec() &&
      configDialog_->modified()) {
    QStringList list = configDialog_->result();
    config->setDescriptionFiles(list);
  }
}

void 
PolicyEditorClass::slotAbout()
{
  QMessageBox::about(this, 
		     "About Policy Editor", 
		     "Policy Editor Ver. 1.0\nCopyright 1999 - 2002 Stefan Enderle, Markus Lauer");

}

void 
PolicyEditorClass::slotAboutQt()
{
  QMessageBox::aboutQt(this, "About Qt");
}

void 
PolicyEditorClass::closeEvent(QCloseEvent *e)
{
  if (saveIfModified()) {
    e->accept();
  }
  else {
    e->ignore(); 
  }
}


//--------------------------------------------------------

int
main(int argc, char** argv) 
{
  int rc = 0;
  Miro::Client client(argc, argv);
  QApplication app(argc, argv);

//   Class::ParameterSet s;

//   s.insert(Parameter("std::string", "name", "Hans Utz", "Guy", "" ));
//   s.insert(Parameter("bool", "truth", "false", "", "" ));
//   s.insert(Parameter("char", "first initial", "H", "", "" ));
//   s.insert(Parameter("short", "short", "-3", "mm", "" ));
//   s.insert(Parameter("unsigned short", "unsigned short", "3", "mm", "" ));
//   s.insert(Parameter("long", "long", "", "", "" ));
//   s.insert(Parameter("unsigned long", "unsigned long", "", "", "" ));
//   s.insert(Parameter("int", "int", "", "", "" ));
//   s.insert(Parameter("unsigned int", "unsigned int", "", "", "" ));
//   s.insert(Parameter("double", "double", "", "", "" ));
//   s.insert(Parameter("angle", "angle", "2", "°", "" ));
//   s.insert(Parameter("Angle", "Angle", "3", "°", "" ));
//   s.insert(Parameter("ACE_Time_Value", "time", ".0003", "sec", "" ));

//   ParameterDialog d(s, "MyBehaviour", NULL);
//   d.exec();
//   return 0;
  

  try {
    PolicyEditorClass policyEditor(client);
    
    if (argc > 2) { 
      std::cout << argv[0] << " [policyfile]" << std::endl; 
      return 1;
    }

    if (argc == 2)
      policyEditor.openPolicy(argv[1]);

    app.setMainWidget(&policyEditor);
    policyEditor.show();
    app.exec();
    
  }

  catch (const std::string& s) {
    std::cerr << "Uncought exception: " << s << std::endl;
    rc = 1;
  }
  catch (const Miro::Exception& e) {
    std::cerr << "Uncought Miro exception: " << e << endl;
    rc = 1;
  }
  catch (const CORBA::Exception& e) {
    std::cerr << "Uncought CORBA exception: " << e << endl;
    rc = 1;
  }

  return rc;
}

