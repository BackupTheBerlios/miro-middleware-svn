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


#include "LogNotifyConsumer.h"

#include "miro/Server.h"
#include "miro/Utils.h"

#include <ace/Date_Time.h>

using std::cout;
using std::cerr;
using std::string;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

namespace 
{
  string path()
  {
    char * miro_log = ACE_OS::getenv("MIRO_LOG");
    string p;

    if (miro_log)
	p = miro_log;

    if (p.size() > 0 && p[p.size() - 1] != '/')
      p += "/";
    return p;
  }


  string createFileName()
  {
    ACE_Date_Time dt;

    char buffer[256];
    memset(buffer, 0, 256);
    ostrstream ostr(buffer, 255);

    ostr << dt.year() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.month() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.day() << "-";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.hour() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.minute()
	 << ".log";

    return string(buffer);
  }
};

int
main(int argc, char *argv[])
{
  string fileName;

  // Parameters to be passed to the services
  Miro::RobotParameters robotParameters;
  LogNotifyParameters parameters;

  // Config file processing
  Miro::ConfigDocument * config = 
    new Miro::ConfigDocument(argc, argv);
  config->setRobotType("Robot");
  config->getParameters("robot", robotParameters);
  config->setRobotType("Notification");
  config->getParameters("logging", parameters);
  delete config;

#ifdef DEBUG
    cout << "parameters:" << endl << parameters << endl;
#endif

  cout << "Initialize server daemon." << flush;
  Miro::Server server(argc, argv, robotParameters);

  // override default logfile name with command line parameter
  if (argc > 1)
    fileName = argv[1];
  else
    fileName = path() + server.namingContextName + "_" + createFileName();

  try {
    // The one channel that we create using the factory.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));
    
    // The consumer, that gets the events
    LogNotify pushConsumer(server,
			   ec.in(), 
			   server.namingContextName,
			   fileName,
			   parameters);
    
    cout << "Loop forever handling events." << endl;
    server.run();
    cout << "Server stoped, exiting." << endl;

    server.detach(1);
    pushConsumer.disconnect();
    
    cout << "shutting down server." << endl;
    server.shutdown();
    server.wait();
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception!" << endl;
    return 1;
  }
  return 0;
}
