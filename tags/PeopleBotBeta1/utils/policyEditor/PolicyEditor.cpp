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

PolicyEditorClass::PolicyEditorClass(int argc, char** argv, Miro::Client& _client) :
  QMainWindow(NULL, "PolicyEditor"),
  client_(_client),
  miroRoot_(getenv("MIRO_ROOT")),
  policyFileName_("_"),
  robot_(getenv("HOST"))
{
  resize(600, 500);

  //-----------//
  // init menu //
  //-----------//

  // file menu
  QPopupMenu* MenuFile = new QPopupMenu();
  menuBar()->insertItem("&File", MenuFile);

  MenuFile->insertItem("New",         this, SLOT(slotNew()));
  MenuFile->insertItem("Open ...",    this, SLOT(slotLoad()));
  MenuFile->insertItem("Save",        this, SLOT(slotSave())); 
  MenuFile->insertItem("Save As ...", this, SLOT(slotSaveAs()));
  MenuFile->insertSeparator();
  MenuFile->insertItem("Send to ...", this, SLOT(slotSendTo()));
  MenuFile->insertSeparator();
  MenuFile->insertItem("Quit",        this, SLOT(quit()));

  // options menue
  QPopupMenu* MenuOptions = new QPopupMenu();
  menuBar()->insertItem("&Options", MenuOptions);

  //MenuOptions->insertItem("&Configuration ..",  this, SLOT(slotConfiguration()));
  //  MenuEdit->insertItem("Add Basic Behaviour ..", this, NULL);
  MenuOptions->insertItem("&Behaviour-Description-Filename..", this, SLOT(getBehaviourDescriptionFileName()));
  MenuOptions->insertItem("&Load Behaviour-Description-Filename..", this, SLOT(setBehaviourDescriptionFileName()));


  // help menu
  QPopupMenu* MenuHelp = new QPopupMenu();
  menuBar()->insertSeparator();
  menuBar()->insertItem("&Help", MenuHelp);
  
  MenuHelp->insertItem("About PolicyEditor", this, SLOT(slotAbout()));
  MenuHelp->insertItem("About Qt",           this, SLOT(slotAboutQt()));
  

  //---------------//
  // init document //
  //---------------//

  // load database file
  document_.loadDatabase("behaviours.dat");
  document_.setModified(false);

  // if given -> load policy file
  if (argc > 1) {
    document_.loadXML(argv[1]);
    policyFileName_=argv[1];
    statusBar()->message(policyFileName_+" geladen!", 1000);
  }
  else {
    document_.loadXML(miroRoot_ + "/etc/PolicyEditorNewFile.xml");
    document_.setModified(false);

    setCaption("new document");
    policyFileName_="_";
    QString message;
    message.sprintf("New document opend");
    statusBar()->message(message, 3000);
  }

  //-----------//
  // init view //
  //-----------//
  view_ = new PolicyViewClass(this, document_);
  setCentralWidget(view_);

#ifdef ASDF
  // load pattern database //
  QMessageBox::information(this, "Policy Editor", "Hallo,\n\nfirst, you have to select your\nbehaviour database ...");
  QString filename = QFileDialog::getOpenFileName(0, "*.dat", this);
  if (filename.isNull()) 
  { 
    std::cout << "database must be chosen !" << std::endl; 
    exit(0); 
  }
  document_.LoadDatabase(string(filename));
#endif
 
}


void 
PolicyEditorClass::slotNew()
{
  if (document_.getModified()) {

    int rc = QMessageBox::warning(this, "PolicyEditor", "The document"
				+policyFileName_+" has been modified\n"+"Do you want to save it?",
				  "&Save...", "&Dont't Save", "&Cancel", 0,2);

    switch(rc) {
    case 0: 
      slotSaveAs(); 
      break;
    case 1:
      break;
    case 2:
      return;
    }
  }  
 
  view_->prepareFileClosing();
  document_.loadXML(miroRoot_ + "/etc/PolicyEditorNewFile.xml");
  document_.setModified(false);

  //   std::cout <<"opened new document" << std::endl;

  setCaption("new document");
  policyFileName_="_";
  QString message;
  message.sprintf("New document opend");
  statusBar()->message(message, 3000);
  view_->setVerticalScrollValue(0);
  view_->setHorizontalScrollValue(0);
  view_->update();     
}


void 
PolicyEditorClass::slotLoad()
{
  // show file dialog //
  if (document_.getModified())
  {
    int rc = QMessageBox::warning(this, "PolicyEditor", "The document"
				  + policyFileName_ + " has been modified\n"+"Do you want to save it?",
				  "&Save...", "&Dont't Save", "&Cancel", 0, 2);

    switch(rc)
    {
    case 0: 
      slotSaveAs();
      break;
    case 1:
      break;
    case 2:
      return;
    }
  } 
  QString filename = QFileDialog::getOpenFileName(0, "Polycies *.xml\nAlle Dateien *", this);
  if (filename.isNull())
    return;
  view_->prepareFileClosing();

  // load selected XML file
  document_.loadXML(filename);
  document_.setModified(false);
  policyFileName_=filename;

  // set new caption and status bar
  setCaption(policyFileName_);
  QString message;
  message.sprintf("%s loaded", (const char*)filename);
  statusBar()->message(message, 3000);
  
  // update the view
  view_->setVerticalScrollValue(0);
  view_->setHorizontalScrollValue(0);
  view_->update();
}

void 
PolicyEditorClass::slotSave()
{
  if (policyFileName_!="_")
  {
    document_.saveXML(policyFileName_);
    document_.setModified(false);
    QString message;
    message.sprintf("%s saved", (const char*) policyFileName_);
    statusBar()->message(message, 3000);
  }
  else
  {
    slotSaveAs();
  }
}
  

void
PolicyEditorClass::slotSaveAs()
{
  // show file dialog
  QString filename = QFileDialog::getSaveFileName(0, "*.xml", this);
	
  if (filename.isNull()) return;
  if (filename.contains('.',false)==0) filename.append(".xml");
  QFile *qFile=new QFile(filename);
  if (qFile->exists())  {
    int rc = QMessageBox::warning(this, "PolicyEditor", "The file"
				+filename+" alread exists!\n"+"Do you want to overwrite the existing file?",
				  "&Yes", "&No", "&Cancel", 0,2);

    switch(rc)
    {
    case 0: 
      break;
    case 1:
      slotSaveAs();
      return;
    case 2:
      return;		 
    }
  }

  // save with selected file name
  document_.saveXML(filename);
  document_.setModified(false);
  policyFileName_=filename;

  // set new caption and status bar
  setCaption(filename);
  QString message;
  message.sprintf("%s saved", (const char*)filename);
  statusBar()->message(message, 3000);
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

/*Not supported yet
  void PolicyEditorClass::slotConfiguration()
  {
  document_.getPolicyConfig().setConfiguration(this);
  }
*/

void 
PolicyEditorClass::getBehaviourDescriptionFileName()
{
  QString fName=document_.getPolicyConfig().getBehaviourDescriptionFileName(); 
  QMessageBox::information(this, "Behaviour-Description-Filename", 
			   "The filename of the loaded Behaviour-Description-File is: "+fName);
}

void
PolicyEditorClass::setBehaviourDescriptionFileName()
{
  QString fileName=QFileDialog::getOpenFileName(0,"*.xml", this);
  if (fileName.isNull()) 
    return;
  document_.getPolicyConfig().setNewBehaviourDescriptionFileName(fileName);
  document_.loadDatabase("");
}

void 
PolicyEditorClass::slotAbout()
{
  QMessageBox::about(this, "About Policy Editor", "Policy Editor Ver. 1.0\nCopyright 1999 - 2002 Stefan Enderle, Markus Lauer");

}

void 
PolicyEditorClass::slotAboutQt()
{
  QMessageBox::aboutQt(this, "About Qt");
}

void 
PolicyEditorClass::quit()
{
  /*if (document_.getModified())
    {
    switch(QMessageBox::warning(this, "PolicyEditor", "The document"
    +policyFileName_+" has been modified\n"+"Do you want to save it?",
    "&Save...", "&Dont't Save", "&Cancel", 0,2))
    {
    case 0: slotSaveAs(); break;
    case 1: break;
    case 2: return;
    }
    } 
    //std::cout <<"close program" << std::endl;*/
  close();
}

void 
PolicyEditorClass::closeEvent(QCloseEvent *e)
{
  if (document_.getModified()) {
    int rc = QMessageBox::warning(this, 
				  "PolicyEditor", "The document"
				+policyFileName_+" has been modified\n"+"Do you want to save it?",
				  "&Save...", "&Don't Save", "&Cancel", 0, 2);
    switch(rc) {
    case 0: 
      if (policyFileName_!="_") {
	document_.saveXML(policyFileName_);
	document_.setModified(false);
	QString message;
	message.sprintf("%s saved", (const char*) policyFileName_);
	statusBar()->message(message, 3000);
      }
      else {
	slotSaveAs();
	if (policyFileName_=="_") 
	  return;//SaveAs-Dialog wurde mit Cancel geschlossen;
      }
    case 1: 
      e->accept();
      return;
    case 2:
      e->ignore(); 
      return;
    }
  }
  else {
    e->accept();
  }
}


//--------------------------------------------------------

int
main(int argc, char** argv) 
{
  int rc = 0;
  Miro::Client client(argc, argv);

  try {
    QApplication App(argc, argv);
    PolicyEditorClass PolicyEditor(argc, argv, client);
    

    // TODO warning dialog
    if (argc>2) { 
      std::cout << argv[0] << " [policyfile]" << std::endl; 
      return 1;
    }

    // TODO load comman line argument policy here

    App.setMainWidget(&PolicyEditor);
    PolicyEditor.show();
    App.exec();
    
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

