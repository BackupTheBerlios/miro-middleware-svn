// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/Server.h"
#include "miro/StructuredPushSupplier.h"

#include "SphinxSpeech.h"

#include <string>

using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_var;

namespace Miro
{
  using std::cout;
  using std::cerr;
  using std::endl;

  using ::operator<<;

  SphinxSpeechServer::SphinxSpeechServer(int argc, char *argv[], 
					 Miro::StructuredPushSupplier * _supplier) :
    super(argc,argv),
    miroRoot(getenv("MIRO_ROOT")),
    speechImpl((miroRoot+"/etc/sphinx.dic").c_str(),true,_supplier),
    pSpeech(speechImpl._this())
  {
    speechImpl.addLm((miroRoot+"/etc/sphinx.lm").c_str(),"default");
    speechImpl.setLm("default");
    

    addToNameService(pSpeech.in(), "Speech");
  }

  SphinxSpeechServer::~SphinxSpeechServer()
  {
  }
}


int main(int argc, char* argv[]) 
{
  Miro::Client * client = new Miro::Client(argc,argv);
  EventChannel_var ec=NULL;
  Miro::StructuredPushSupplier * pSupplier=NULL;
  try {
    ec = client->resolveName<EventChannel>("EventChannel");
    pSupplier = new Miro::StructuredPushSupplier(ec.in(),client->namingContextName);
  }
  catch(...) {
    ec = NULL;
    pSupplier = NULL;
  }

  delete client;//not needed anymore

  Miro::SphinxSpeechServer speech(argc, argv, pSupplier);
 
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
