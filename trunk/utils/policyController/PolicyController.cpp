// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
// 
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PolicyController.h"

#include <ace/Arg_Shifter.h>

#include <iostream>
#include <sstream>

#include <qfiledialog.h>
#include <qlayout.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qinputdialog.h>
#include <qapplication.h>
#include <qmessagebox.h>

ControllerWidget::ControllerWidget(Miro::Client& _client, bool _start,
				   QWidget *parent, const char *name) :
  QWidget(parent, name),
  client_(_client),
  robotName_(client_.namingContextName.c_str()),
  start_(_start)
{
  QBoxLayout *topLayout = new QHBoxLayout(this, 5);

  // the file menu
  fileMenu = new QPopupMenu( this );
  fileMenu->insertItem( "&Connect robot", this, SLOT( robot() ) );
  fileMenu->insertSeparator();
  iLoad_ = fileMenu->insertItem( "&Load policy", this, SLOT( load() ) );
  iSend_ = fileMenu->insertItem( "&Send policy", this, SLOT( send() ) );
  fileMenu->insertSeparator();
  fileMenu->insertItem( "&Quit", qApp, SLOT( quit() ) );

  // the edit menu
  editMenu = new QPopupMenu( this );
  iTransition_ = editMenu->insertItem( "&Send transition", this, SLOT( sendTransition() ) );

  menuBar = new QMenuBar( this );
  menuBar->insertItem( "&File", fileMenu );    
  menuBar->insertItem( "&Edit", editMenu );    

  // ... add to layout
  topLayout->setMenuBar(menuBar);

  // the widgets
  playButton = new QPushButton( "Play", this, "play" );
  playButton->setMinimumSize(90, 30);
  stopButton = new QPushButton( "Stop", this, "stop" );
  stopButton->setMinimumSize(90, 30);

  // ... add to the layout 
  QBoxLayout *layout1 = new QVBoxLayout(topLayout, 5);
  QGridLayout *layout2 = new QGridLayout(layout1, 2, 3, 5);

  layout2->addWidget(playButton, 0, 0);
  layout2->addWidget(stopButton, 0, 1);

  connect( playButton, SIGNAL( clicked() ),
	   this, SLOT( openPolicy() ) );
  connect( stopButton, SIGNAL( clicked() ),
	   this, SLOT( closePolicy() ) );

  connectToRobot(robotName_, true);
}

//------------------------------------------------------------------------------
// public methods
//------------------------------------------------------------------------------

void 
ControllerWidget::connectToRobot(const QString& _robot, bool _silent)
{
  QString error;
  bool ok = true;
  
  // build the lookup string for the naming service
  CosNaming::Name name;
  name.length(2);
  name[0].id = CORBA::string_dup(_robot.latin1());
  name[1].id = CORBA::string_dup("BehaviourEngine");
  
  // try binding the robots naming context
  try {
    engine_ = client_.resolveName<Miro::BehaviourEngine>(name);
  }
  catch(const CORBA::Exception& e) {
    std::ostringstream sstr;
    sstr << "Robot " << _robot << endl
	 << "Communication Failed." << endl
	 << "CORBA exception: " << e << flush;
    
    error = sstr.str().c_str();
    ok = false;
  }
  
  setItemsEnable(ok);
  if (ok) {
    robotName_ = _robot;
  }
  else if (!_silent) {
    QMessageBox::warning(this, "Couln't set robot:", error);
  }
}

void
ControllerWidget::sendPolicy(const QString& _policy)
{
  bool ok = true;
  QString error;
  char * policy = NULL;
  
  QFile f(_policy);
  if (f.exists()) {
    f.open(IO_ReadOnly);
    policy = new char[f.size() + 1];
    memset(policy, 0, f.size() + 1);
    f.readBlock(policy, f.size());

    try {
      engine_->loadPolicy(policy);
    }
    catch(const Miro::BehaviourEngine::EMalformedXML& ) {
      ok = false;
      error = "Malformed XML in file:\n" + _policy;
    }
    catch(const Miro::BehaviourEngine::EMalformedPolicy& e) {
      ok = false;
      error = "Error parsing policy" + _policy + ":\n" + e.what;
    }
    catch(const CORBA::Exception& e) {
      ok = false;
      setItemsEnable(false);
      std::ostringstream sstr;
      sstr << "Communication Failed." << endl
	   << "CORBA exception: " << endl
	   << e;
      error = sstr.str().c_str();
    }
    delete[] policy;
  }
  else {
    ok = false;
    error = "File not found: " + _policy;
  }

  if (!ok)
    QMessageBox::warning(this, "Error sending policy:", error);
}

void
ControllerWidget::loadPolicy(const QString& _policy)
{
  bool ok = true;
  QString error;

  try {
    engine_->loadPolicyFile(_policy.latin1());
  }
  catch(const Miro::BehaviourEngine::EMalformedXML& ) {
    ok = false;
    error = "Malformed XML in file:\n" + _policy;
  }
  catch(const Miro::BehaviourEngine::EMalformedPolicy& e) {
    ok = false;
    error = "Error parsing policy" + _policy + ":\n" + e.what;
  }
  catch(const Miro::BehaviourEngine::EFile&) {
    ok = false;
    error = "Error loading policy file " + _policy;
  }
  catch(const CORBA::Exception& e) {
    ok = false;
    setItemsEnable(false);
    std::ostringstream sstr;
    sstr << "Communication Failed." << endl
	   << "CORBA exception: " << endl
	 << e;
    error = sstr.str().c_str();
  }

  if (!ok)
    QMessageBox::warning(this, "Error loading policy:", error);
}


void
ControllerWidget::setItemsEnable(bool toggle)
{
  fileMenu->setItemEnabled(iLoad_, toggle);
  fileMenu->setItemEnabled(iSend_, toggle);
  editMenu->setItemEnabled(iTransition_, toggle);

  playButton->setEnabled(toggle);
  stopButton->setEnabled(toggle);
}


// protected slots

void 
ControllerWidget::robot() 
{
  bool ok = false;
  QString tmp = QInputDialog::getText(tr( "Select robot" ),
				      tr( "Robot name" ),
				      QLineEdit::Normal, robotName_, &ok, this );
  
  if ( ok && !tmp.isEmpty() ) {
    connectToRobot(tmp);
  }
}

void 
ControllerWidget::load() 
{
  bool ok = false;
  QString tmp = QInputDialog::getText(tr( "Load policy on robot" ),
				      tr( "Policy file name" ),
				      QLineEdit::Normal, robotName_, &ok, this );
  
  if ( ok && !tmp.isEmpty() ) {
    loadPolicy(tmp);
  }
}

void 
ControllerWidget::send() 
{
  QString policy = 
    QFileDialog::getOpenFileName( QString::null, "Policyfiles (*.xml)", this );

  if ( !policy.isEmpty() )
    sendPolicy(policy);
}

void 
ControllerWidget::openPolicy() 
{
  bool ok = true;
  QString error;

  try {
    engine_->openPolicy();
  }
  catch(const Miro::BehaviourEngine::ENoPolicy& ) {
    ok = false;
    error = "No policy loaded.";
  }
  catch(const CORBA::Exception& e) {
    ok = false;
    setItemsEnable(false);
    std::ostringstream sstr;
    sstr << "Communication Failed." << endl
	 << "CORBA exception: " << endl
	 << e;
    error = sstr.str().c_str();
  }

  if (!ok)
    QMessageBox::warning(this, "Error starting policy:", error);
}

void
ControllerWidget::closePolicy() 
{
  bool ok = true;
  QString error;

  try {
    engine_->closePolicy();
  }
  catch(const Miro::BehaviourEngine::ENoPolicy& ) {
    ok = false;
    error = "No policy loaded.";
  }
  catch(const CORBA::Exception& e) {
    ok = false;
    setItemsEnable(false);
    std::ostringstream sstr;
    sstr << "Communication Failed." << endl
	 << "CORBA exception: " << endl
	 << e;
    error = sstr.str().c_str();
  }

  if (!ok)
    QMessageBox::warning(this, "Error closing policy:", error);
}

void 
ControllerWidget::sendTransition() 
{
  bool ok = false;
  QString tmp = QInputDialog::getText(tr( "Send transtition:" ),
				      tr( "Transition name" ),
				      QLineEdit::Normal, robotName_, &ok, this );
  
  if ( ok && !tmp.isEmpty() ) {
//	engine->sendTransition(tmp.latin1());
  }
}

int 
main( int argc, char *argv[] )
{
  bool start = false;

  // Initialize library objects.
  Miro::Client client(argc, argv);
  QApplication app(argc, argv);

  // Argument parsing.

  try {
    // Initialize controller widget.
    ControllerWidget controllerWidget(client, start);

    // setting up GUI event loop
    app.setMainWidget(&controllerWidget);
    controllerWidget.show();
    app.exec();
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;


}