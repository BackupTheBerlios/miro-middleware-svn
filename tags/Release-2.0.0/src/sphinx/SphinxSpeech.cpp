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
#include "miro/Server.h"
#include "miro/StructuredPushSupplier.h"

#include "SphinxSpeech.h"

#include <string>

using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_var;

using std::cout;
using std::cerr;
using std::endl;


namespace Miro
{
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
