// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
