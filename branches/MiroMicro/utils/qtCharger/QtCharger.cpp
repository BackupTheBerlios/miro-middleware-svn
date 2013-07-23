// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This is a graphical interface for deploy and retract the buildin charger 
// in the p3dx and for the new developed charger for the p3at
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; 
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// $Id: QtCharger.cpp jaklitsch $
//

#include <qapplication.h>

#include "MainForm.h"

int main(int argc, char * argv[])
{
  QApplication a(argc, argv);

  MainForm *mainForm=new MainForm(argc,argv);

  a.setMainWidget(mainForm);
  mainForm->setCaption("qtCharger");
  mainForm->show();

  
  int result=a.exec();

  return result;
}
