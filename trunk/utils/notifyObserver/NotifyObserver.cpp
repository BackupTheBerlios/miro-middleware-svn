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
#include "EventChannelFactory.h"
#include "ApplicationWindow.h"

#include "miro/Server.h"

//#include <tao/qt_resource.h>

#include <qapplication.h>

int main (int argc, char * argv[])
{
  int rc = 1;

  // Qt specific stuff for running with TAO
  QApplication app(argc, argv);
  //  TAO_QtResource_Factory::set_context (&app);

  try {
    Miro::Server server(argc, argv);
    ApplicationWindow * mw = new ApplicationWindow(server);

    app.setMainWidget(mw);
    mw->show();
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
