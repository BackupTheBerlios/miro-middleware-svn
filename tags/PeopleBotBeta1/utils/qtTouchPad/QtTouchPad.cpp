// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include <qapplication.h>

#include "MainForm.h"

int main(int argc, char * argv[])
{
  QApplication a(argc, argv);

  MainForm *mainForm=new MainForm(argc,argv);

  a.setMainWidget(mainForm);
  mainForm->setCaption("qtTouchPad");
  mainForm->show();

  
  int result=a.exec();

  return result;
}
