// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (Middleware for Robots)
// 
// (c) 2001, 2002, 2003, 2004
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
#include "idl/ExceptionC.h"

#include <ace/OS.h>
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
  bool shared = false;
  bool unified = false;
  bool localizeDebug = false;

  const char colocatedOpt[] = "-shared_ec";
  const char unifiedOpt[] = "-unified_ec";
  const char verboseOpt[] = "-v";
  const char localizeOpt[] = "-l";
  const char helpOpt[] = "-?";
};

int
main(int argc, char *argv[])
{
  int rc = 0;
  try {
    Miro::Log::init(argc, argv);

    // parse optional args
    ACE_Arg_Shifter arg_shifter (argc, argv);
    while (arg_shifter.is_anything_left ()) {
      char *current_arg = (char *)arg_shifter.get_current ();
      
      if (ACE_OS::strcasecmp(current_arg, colocatedOpt) == 0) {
	arg_shifter.consume_arg();
	shared = true;
      } 
      else if (ACE_OS::strcasecmp(current_arg, unifiedOpt) == 0) {
	arg_shifter.consume_arg();
	unified = true;
      } 
      else if (ACE_OS::strcasecmp(current_arg, localizeOpt) == 0) {
	arg_shifter.consume_arg();
	localizeDebug = true;
      } 
      else if (ACE_OS::strcasecmp(current_arg, verboseOpt) == 0) {
	arg_shifter.consume_arg();
	verbose = true;
      } 
      else if (ACE_OS::strcasecmp(current_arg, helpOpt) == 0) {
	arg_shifter.consume_arg();
	std::cerr << "usage: " << argv[0] << "[-shared_ec] [-unified_ec] [-v?]" << std::endl
		  << "  -shared_ec use existing event channels" << std::endl
		  << "  -unified_ec use one event channel for all robots" << std::endl
		  << "  -v verbose mode" << std::endl
		  << "  -? help: emit this text and stop" << std::endl;
	return 0;
      } 
      else
	arg_shifter.ignore_arg ();
    }
    
    if (unified && shared) {
      std::cerr << "Can't use unified_ec and shared_ec options simultaniously" << std::endl;
      return 1;
    }
    
    if (verbose) {
      if (shared) 
	std::cout << "Using a shared event channel factory." << std::endl;
      if (unified)
	std::cout << "Using a unified event channel factory." << std::endl;
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
      std::cout << "Initialize server daemon." << std::endl;
    
    ChannelManager channelManager(argc, argv, shared, unified);
    //    channelManager.setDebugLoclaize(localizeDebug);
    try {
      QApplication app(argc, argv);     // Create Qt application  
      FileSet fileSet(&channelManager);
      MainForm mainWindow(app, fileSet);
      
      // DOTO: implement that, please...
      if (verbose)
	std::cout << "adding excluded events" << std::endl;
      for (int i = 2; i < argc; ++i) {
	if (verbose)
	  std::cout << argv[i] << std::endl;
	mainWindow.addExclude(argv[i]);
      }
      
      // Scope operator to delimit the lifetime
      // of MyWidget, to prevent conflicts with CORBA cleanup
      {
	if (verbose)
	  std::cout << "set main widget" << std::endl;
	app.setMainWidget( &mainWindow );
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	mainWindow.show();
	channelManager.detach(4);

	// parse remaining args
	for (int i = 1; i < argc; ++i) {
	  if (verbose)
	    std::cout << "using file " << argv[i] << std::endl;
	  mainWindow.loadFile(argv[i]);
	}
	
	if (verbose)
	  std::cout << "exec application" << std::endl;
	app.exec();
      }
      
      channelManager.shutdown();
      channelManager.wait();
    }
    catch (const CORBA::Exception& e) {
      std::cerr << "CORBA exception: " << e << std::endl;
      rc = 1;
    }
  }
  catch (Miro::Exception const& e) {
    std::cerr << "Miro exception: " << e << std::endl;
    rc = 1;
  }

  return rc;
}
