// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PolicyEditor.h"

#include <qapplication.h>
#include <qfiledialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qstatusbar.h>

#include <iostream>

PolicyEditorClass::PolicyEditorClass(int argc, char** argv) :
  QMainWindow(NULL, "PolicyEditor"),
  policyFileName("_")
  
{
  resize(600, 500);

  //-----------//
  // init menu //
  //-----------//
  QPopupMenu* MenuFile = new QPopupMenu();
  menuBar()->insertItem("&File", MenuFile);

  MenuFile->insertItem("New",    this, SLOT(slotNew()));
  MenuFile->insertItem("Open ...",    this, SLOT(slotLoad()));
  MenuFile->insertItem("Save", this, SLOT(slotSave())); 
  MenuFile->insertItem("Save As ...", this, SLOT(slotSaveAs()));
  MenuFile->insertSeparator();
  MenuFile->insertItem("Quit",       this, SLOT(quit()));

  QPopupMenu* MenuOptions = new QPopupMenu();
  menuBar()->insertItem("&Options", MenuOptions);

  //MenuOptions->insertItem("&Configuration ..",  this, SLOT(slotConfiguration()));
  //  MenuEdit->insertItem("Add Basic Behaviour ..", this, NULL);
  MenuOptions->insertItem("&Behaviour-Description-Filename..", this, SLOT(getBehaviourDescriptionFileName()));
  MenuOptions->insertItem("&Load Behaviour-Description-Filename..", this, SLOT(setBehaviourDescriptionFileName()));

  QPopupMenu* MenuHelp = new QPopupMenu();
  menuBar()->insertSeparator();
  menuBar()->insertItem("&Help", MenuHelp);
  
  MenuHelp->insertItem("About PolicyEditor", this, SLOT(slotAbout()));
  MenuHelp->insertItem("About Qt",           this, SLOT(slotAboutQt()));
  

  //---------------//
  // init document //
  //---------------//

  // load database file //
  Document.loadDatabase("behaviours.dat");
  Document.setModified(false);
  // if given -> load policy file //
  if (argc==2) {
    Document.loadXML(argv[1]);
    policyFileName=argv[1];
    statusBar()->message(policyFileName+" geladen!", 1000);
  }
  else 
  {
    char *nixRoot=getenv("NIX_ROOT");	
    Document.loadXML(QString(nixRoot)+"/etc/PolicyEditor_newFile_template.xml");
    Document.setModified(false);
    std::cout <<"opened new document" << std::endl;
    setCaption("new document");
    policyFileName="_";
    QString message;
    message.sprintf("New document opend");
    statusBar()->message(message, 3000);
  }

  //-----------//
  // init view //
  //-----------//
  View = new PolicyViewClass(this, Document);
  setCentralWidget(View);


 
#ifdef ASDF
  // load pattern database //
  QMessageBox::information(this, "Policy Editor", "Hallo,\n\nfirst, you have to select your\nbehaviour database ...");
  QString filename = QFileDialog::getOpenFileName(0, "*.dat", this);
  if (filename.isNull()) 
  { 
    std::cout << "database must be chosen !" << std::endl; 
    exit(0); 
  }
  Document.LoadDatabase(string(filename));
#endif
 
}


void PolicyEditorClass::slotNew()
{
  if (Document.getModified())
  {
    switch(QMessageBox::warning(this, "PolicyEditor", "The document"
				+policyFileName+" has been modified\n"+"Do you want to save it?",
				"&Save...", "&Dont't Save", "&Cancel", 0,2))
    {
    case 0: slotSaveAs(); break;
    case 1: break;
    case 2: return;
    }
  }   
  View->prepareFileClosing();
  char *nixRoot=getenv("NIX_ROOT");
  Document.loadXML(QString(nixRoot)+"/etc/PolicyEditor_newFile_template.xml");
  Document.setModified(false);
  std::cout <<"opened new document" << std::endl;
  setCaption("new document");
  policyFileName="_";
  QString message;
  message.sprintf("New document opend");
  statusBar()->message(message, 3000);
  View->setVerticalScrollValue(0);
  View->setHorizontalScrollValue(0);
  View->update();     
}


void PolicyEditorClass::slotLoad()
{
  // show file dialog //
  if (Document.getModified())
  {
    switch(QMessageBox::warning(this, "PolicyEditor", "The document"
				+policyFileName+" has been modified\n"+"Do you want to save it?",
				"&Save...", "&Dont't Save", "&Cancel", 0,2))
    {
    case 0: slotSaveAs(); break;
    case 1: break;
    case 2: return;
    }
  } 
  QString filename = QFileDialog::getOpenFileName(0, "Polycies *.xml\nAlle Dateien *", this);
  if (filename.isNull()) return;
  View->prepareFileClosing();
  // load selected XML file //
  Document.loadXML(filename);
  Document.setModified(false);
  policyFileName=filename;
  // set new caption and status bar //
  setCaption(policyFileName);
  QString message;
  message.sprintf("%s loaded", (const char*)filename);
  statusBar()->message(message, 3000);
  
  // update the view //
  View->setVerticalScrollValue(0);
  View->setHorizontalScrollValue(0);
  View->update();
}

void PolicyEditorClass::slotSave()
{
  if (policyFileName!="_")
  {
    Document.saveXML(policyFileName);
    Document.setModified(false);
    QString message;
    message.sprintf("%s saved", (const char*) policyFileName);
    statusBar()->message(message, 3000);
  }
  else
  {
    slotSaveAs();
  }
}
  

void PolicyEditorClass::slotSaveAs()
{
  // show file dialog //
  QString filename = QFileDialog::getSaveFileName(0, "*.xml", this);
	
  if (filename.isNull()) return;
  if (filename.contains('.',false)==0) filename.append(".xml");
  QFile *qFile=new QFile(filename);
  if (qFile->exists())  {
    switch(QMessageBox::warning(this, "PolicyEditor", "The file"
				+filename+" alread exists!\n"+"Do you want to overwrite the existing file?",
				"&Yes", "&No", "&Cancel", 0,2))
    {
    case 0: break;
    case 1: slotSaveAs(); return;
    case 2: return;		 
    }
  }

  // save with selected file name //
  Document.saveXML(filename);
  Document.setModified(false);
  policyFileName=filename;
  // set new caption and status bar //
  setCaption(filename);
  QString message;
  message.sprintf("%s saved", (const char*)filename);
  statusBar()->message(message, 3000);
}

/*Not supported yet
  void PolicyEditorClass::slotConfiguration()
  {
  Document.getPolicyConfig().setConfiguration(this);
  }
*/

void PolicyEditorClass::getBehaviourDescriptionFileName()
{
  QString fName=Document.getPolicyConfig().getBehaviourDescriptionFileName(); 
  QMessageBox::information(this, "Behaviour-Description-Filename", 
			   "The filename of the loaded Behaviour-Description-File is: "+fName);
}

void PolicyEditorClass::setBehaviourDescriptionFileName()
{
  QString fileName=QFileDialog::getOpenFileName(0,"*.xml", this);
  if (fileName.isNull()) return;
  Document.getPolicyConfig().setNewBehaviourDescriptionFileName(fileName);
  Document.loadDatabase("");
}

void PolicyEditorClass::slotAbout()
{
  QMessageBox::about(this, "About Policy Editor", "Policy Editor Ver. 1.0\nCopyright 1999 - 2002 Stefan Enderle, Markus Lauer");

}

void PolicyEditorClass::slotAboutQt()
{
  QMessageBox::aboutQt(this, "About Qt");
}

void PolicyEditorClass::quit()
{
  /*if (Document.getModified())
    {
    switch(QMessageBox::warning(this, "PolicyEditor", "The document"
    +policyFileName+" has been modified\n"+"Do you want to save it?",
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

void PolicyEditorClass::closeEvent(QCloseEvent *e)
{
  if (Document.getModified())
  {
    switch(QMessageBox::warning(this, "PolicyEditor", "The document"
				+policyFileName+" has been modified\n"+"Do you want to save it?",
				"&Save...", "&Don't Save", "&Cancel", 0,2))
    {
    case 0: 
      if (policyFileName!="_")
      {
	Document.saveXML(policyFileName);
	Document.setModified(false);
	QString message;
	message.sprintf("%s saved", (const char*) policyFileName);
	statusBar()->message(message, 3000);
      }
      else
      {
	slotSaveAs();
	if (policyFileName=="_") return;//SaveAs-Dialog wurde mit Cancel geschlossen;
      }
    case 1: e->accept(); return;
    case 2: e->ignore(); return;
    }
  }
  else {
    e->accept();
  }
}


//--------------------------------------------------------

int main(int argc, char** argv) 
{
  try {
    QApplication App(argc, argv);
    PolicyEditorClass PolicyEditor(argc, argv);
    
    if (argc>2) { 
      std::cout << argv[0] << " [policyfile]" << std::endl; 
      return 1;
    }

    App.setMainWidget(&PolicyEditor);
    PolicyEditor.show();
    App.exec();
    
  }

  catch (const std::string& s) {
    std::cout << "main: Exception: " << s << std::endl;
  }

  catch (...) {
    std::cout << "main: Unknown Exception!" << std::endl;
  }

  return 0;
}

