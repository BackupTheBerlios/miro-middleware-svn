// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include <qapplication.h>

#include "MainForm.h"


using namespace Miro;

int main(int argc, char *argv[])
{

  QApplication a(argc, argv);

  MainForm *mainForm=new MainForm(argc,argv);

  a.setMainWidget(mainForm);
  mainForm->setCaption("QtVideo");
  mainForm->show();

  
  int result=a.exec();

  return result;
}




