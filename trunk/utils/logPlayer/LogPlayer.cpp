// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
// 
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// $Date$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ChannelManager.h"
#include "FileSet.h"
#include "MainForm.h"

#include "miro/Exception.h"
#include "miro/ExceptionC.h"

#include <ace/Arg_Shifter.h>

#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

#include <qapplication.h>

#include <iostream>

namespace 
{
  bool verbose = false;
};

int
main(int argc, char *argv[])
{
  bool shared = false;
  const char colocatedOpt[] = "-shared_ec";
  const char verboseOpt[] = "-v";
 
  // parse optional args
  ACE_Arg_Shifter arg_shifter (argc, argv);
  while (arg_shifter.is_anything_left ()) {
    char *current_arg = (char *)arg_shifter.get_current ();

    if (ACE_OS::strcasecmp(current_arg, colocatedOpt) == 0) {
      arg_shifter.consume_arg();
      shared = false;
      if (verbose)
	std::cout << "Using a shared event channel factory." << endl;
    } 
    else if (ACE_OS::strcasecmp(current_arg, verboseOpt) == 0) {
      arg_shifter.consume_arg();
      verbose = true;
    } 
    else
      arg_shifter.ignore_arg ();
  }

  // Init TAO Factories
  // for the colocated event channel
  if (!shared) {
    TAO_Notify_Default_CO_Factory::init_svc();
    TAO_Notify_Default_POA_Factory::init_svc();
    TAO_Notify_Default_Collection_Factory::init_svc();
    TAO_Notify_Default_EMO_Factory::init_svc();
  }

  // Create Miro server
  if (verbose)
    std::cout << "Initialize server daemon." << endl;

  ChannelManager channelManager(argc, argv, shared);
  try {
    QApplication app(argc, argv);     // Create Qt application  
    FileSet fileSet(&channelManager);
    MainForm mainWindow(app, fileSet);

    // DOTO: implement that, please...
    if (verbose)
      std::cout << "adding excluded events" << endl;
    for (int i = 2; i < argc; ++i) {
      if (verbose)
	std::cout << argv[i] << endl;
      mainWindow.addExclude(argv[i]);
    }
      
    // Scope operator to delimit the lifetime
    // of MyWidget, to prevent conflicts with CORBA cleanup
    {
      if (verbose)
	std::cout << "set main widget" << endl;
      app.setMainWidget( &mainWindow );
      QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
      mainWindow.show();
      channelManager.detach(4);

      // parse remaining args
      if (argc > 1) {
	if (verbose)
	  std::cout << "using file " << argv[1] << endl;
	mainWindow.loadFile(argv[1]);
      }
    
      if (verbose)
	std::cout << "exec application" << endl;
      app.exec();
    }

    channelManager.shutdown();
    channelManager.wait();
  }
  catch (Miro::Exception const& e) {
    std::cerr << "Miro exception: " << e << endl;
  }
  catch (const CORBA::Exception& e) {
    std::cerr << "CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
