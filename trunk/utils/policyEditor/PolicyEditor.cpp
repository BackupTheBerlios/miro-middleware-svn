// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1998, 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "MainWindow.h"

#include "utils/widgets/ConfigFile.h"
#include "utils/widgets/DocumentView.h"

#include "miro/Client.h"
#include "miro/Exception.h"

#include <qapplication.h>

#include <iostream>

int
main(int argc, char* argv[]) 
{
  int rc = 1;

  ConfigFileName::instance()->setFileName(".PolicyEditor.xml");

  Miro::Client client(argc, argv);
  QApplication app(argc, argv);

  try {
    MainWindow mainWindow(client);
    
    if (argc > 2)
      throw Miro::Exception(std::string("Usage: ") + argv[0] + " [policyfile]");

    if (argc == 2)
      mainWindow.view()->openDocument(argv[1]);

    app.setMainWidget(&mainWindow);
    mainWindow.show();
    app.exec();
    rc = 0;
  }
  catch (const Miro::Exception& e) {
    std::cerr << "Uncought Miro exception: " << e << std::endl;
  }
  catch (const CORBA::Exception& e) {
    std::cerr << "Uncought CORBA exception: " << e << std::endl;
  }

  return rc;
}

