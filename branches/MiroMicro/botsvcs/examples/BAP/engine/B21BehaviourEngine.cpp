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
#include "../simple/BehavioursFactory.h"
#include "../b21/BehavioursFactory.h"

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"

#include "botsvcs/BAP/BehaviourEngineImpl.h"

#include <orbsvcs/CosNotifyCommC.h>

using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_var;

using Miro::Server;
using Miro::ReactorTask;

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
  Server server(argc, argv);
  ReactorTask * task = new ReactorTask(&server);

  if (argc < 2) {
    cout << "usage: " << argv[0] << " <BEHAVIOURFILE>" << endl;
    return 1;
  }

  try {
    SimpleBehavioursFactory simpleBehaviours(server, *task->reactor());
    B21BehavioursFactory b21Behaviours(server, *task->reactor());

    // Notification Service
    EventChannel_var ec = server.resolveName<EventChannel>("EventChannel");
    
    Miro::StructuredPushSupplier supplier_(ec.in(), server.namingContextName, false);
    
    // Init behaviour infrastructure
    Miro::BAP::BehaviourEngineImpl engineImpl(&supplier_);
    Miro::BehaviourEngine_var engineInterface = engineImpl._this();
    server.addToNameService(engineInterface.in(), "BehaviourEngine");

    // initialize ActionPatterns
    engineImpl.loadPolicyFile(argv[1]);

    // start timed behaviour sceduler
    task->open(NULL);

    simpleBehaviours.connect();
    supplier_.connect();

    cout << "Loop forever handling events." << endl;
    server.run(5);

    cout << "Server stoped, exiting." << endl;

    engineImpl.closePolicy();

    simpleBehaviours.disconnect();
    supplier_.disconnect();

    task->cancel();
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
