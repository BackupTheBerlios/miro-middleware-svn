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

#include "DtlkSpeech.h"
#include "SpeechDevice.h"

#include "miro/Exception.h"
#include "miro/Utils.h"

#include <ace/ARGV.h>

#include <iostream>
#include <cstring>

using std::cout;
using std::cerr;
using std::endl;

const char * intro = "Hello everybody. I am Stanislav. At your service.";

Speech::Speech(int argc, char *argv[]) :
  super(argc, argv),
  //  reactorTask(this),
  speechImpl(),
  pSpeech(speechImpl._this())
{
  CosNaming::Name n(1);
  n.length(1);
  n[0].id = CORBA::string_dup("Speech");

  if (rebind_) {
    // Force binding of references to make
    // sure they are always up-to-date.
    try {
      namingContext->unbind(n);
      cerr << "A Speech object was still regsitered at the NamingService.";
    }
    catch (...) {
    }
  }
  try {
    namingContext->bind(n, pSpeech.in());
  }
  catch (CosNaming::NamingContext::AlreadyBound& ) {
    cerr << "Objects were still bound in naming service: " 
	 << n[0].id << endl
	 << "Use -MiroRebindIOR if you really want to rebind them." << endl;
    
    throw(0);
  }

#ifdef DEBUG
  cout << "Speech initialized.." << endl;
#endif
}

Speech::~Speech()
{
#ifdef DEBUG
  cout << "Destructing Speech." << endl;
#endif
  //  pConsumer->disconnect_push_consumer();

  try {
    CosNaming::Name n;
     
    n.length(1);
    n[0].id = CORBA::string_dup("Speech");
    namingContext->unbind(n);
    //    n[0].id = CORBA::string_dup("SpeechEC");
    //    namingContext->unbind(n);

#ifdef DEBUG
    cout << "Unbound objects from NamingSevice." << endl;
#endif
  }
  catch (const CORBA::Exception& e) {
    cerr << "Caught CORBA exception on unbind: " << e << endl;
    cerr << "Porbably the NameSevice went down while we ran." << endl;
  }
}

int
main(int argc, char *argv[])
{
  // set up argv for CORBA init
 
  int i;
  for (i=0; i<argc; ++i)               // search for -ORBSvcConf option
    if (strcmp("-ORBSvcConf", argv[i]) == 0)
      break;

  ACE_ARGV cl;                         // set up new argv
  cl.add(argv);                        // add main argv

  if (i == argc)                       // if no -ORBSvcConf file is specified,
    cl.add("-ORBSvcConf speech.conf"); //  specify one

#ifdef DEBUG
  cout << "Initialize server daemon." << endl;
#endif
  Speech speech(cl.argc(), cl.argv());

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-SpeechInfo") == 0) {
      speech.speechImpl.connection.catInfo();
      return 0;
    }

    if (strcmp(argv[i], "-SpeechSetIntro") == 0) {
      char buffer[256];
      std::cin.getline(buffer, 256);
      speech.speechImpl.connection.setBootMessage(buffer);
      return 0;
    }
  }

  try {
#ifdef DEBUG
    cout << "Loop forever handling events." << endl;
#endif
    speech.run();
#ifdef DEBUG
    cout << "Speech server ended, exiting." << endl;
#endif
  }
  catch (const Miro::EDevIO& e) {
    cerr << "DevIO excetpion: Device access failed." << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (const Miro::ACE_Exception& e) {
    cerr << "ACE exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }
  return 0;
}
