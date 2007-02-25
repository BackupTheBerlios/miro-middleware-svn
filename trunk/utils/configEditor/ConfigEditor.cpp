// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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

