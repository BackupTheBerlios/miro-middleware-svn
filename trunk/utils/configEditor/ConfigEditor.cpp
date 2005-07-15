// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "MainWindow.h"

#include "utils/widgets/ConfigFile.h"
#include "utils/widgets/DocumentView.h"

#include "miro/Exception.h"
#include "miro/Client.h"

#include <qapplication.h>

#include <iostream>

int
main(int argc, char * argv[]) 
{
  int rc = 1;

  ConfigFileName::instance()->setFileName(".ConfigEditor.xml");

  Miro::Client client(argc, argv);
  QApplication app(argc, argv);

  try {
    MainWindow mainWindow(client);
    
    if (argc > 2)
      throw Miro::Exception(std::string("Usage: ") + argv[0] + " [config file]");
    
    if (argc == 2)
      mainWindow.view()->openDocument(argv[1]);
    
    app.setMainWidget(&mainWindow);
    mainWindow.show();
    app.exec();
  }
  catch (const Miro::Exception& e) {
    std::cerr << "Miro exception:\n" << e << std::endl;
    rc = 1;
  }

  return rc;
}

