// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "../lib/DummyFactory.h"

#include "botsvcs/BAP/BehaviourRepository.h"
#include "botsvcs/BAP/ArbiterRepository.h"
#include "botsvcs/BAP/ActionPattern.h"
#include "botsvcs/BAP/BehaviourEngineImpl.h"

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int
main(int argc, char* argv[])
{
  // Init factories
  TAO_Notify_Default_CO_Factory::init_svc();
  TAO_Notify_Default_POA_Factory::init_svc();
  TAO_Notify_Default_Collection_Factory::init_svc();
  TAO_Notify_Default_EMO_Factory::init_svc();

  Miro::Log::init(argc, argv);
  Miro::Server server(argc, argv);
  Miro::ReactorTask task(ACE_Reactor::instance(), &server);

  try {
    CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory =    
      TAO_Notify_EventChannelFactory_i::create(server.poa.in()
					       ACE_ENV_ARG_PARAMETER);

    server.addToNameService(notifyFactory.in(),
			    "NotifyEventChannelFactory");

    /** Initial qos specified to the factory when creating the EC. */
    CosNotification::QoSProperties initialQos;
    /** Initial admin props specified to the factory when creating the EC. */
    CosNotification::AdminProperties initialAdmin;
    /** Channel identifer. */
    CosNotifyChannelAdmin::ChannelID id;

    /** The one channel that we create using the factory. */
    CosNotifyChannelAdmin::EventChannel_var ec =
      notifyFactory->create_channel(initialQos, initialAdmin, id);

    server.addToNameService(ec.in(),
			    "EventChannel");

    Miro::StructuredPushSupplier pushSupplier(ec.in(),
					      server.namingContextName);


    // create available behavioiurs and arbiters
    DummyFactory factory;
    factory.init();

    Miro::BAP::BehaviourEngineImpl * engineImpl = 
      new Miro::BAP::BehaviourEngineImpl(&pushSupplier);
    Miro::BehaviourEngine_var engine = engineImpl->_this();
    server.addToNameService(engine.in(), "BehaviourEngine");

    if (argc > 1) {
      // initialize ActionPatterns
      engine->loadPolicyFile(argv[1]);
    }

    std::cout << "loop forever handling events" << endl;
    task.open(NULL);
    server.run();
    task.cancel();
    factory.fini();
  }
  catch (const Miro::Exception& e) {
    std::cout << endl
	      << "Miro::Exception:" << endl
	      << e << endl;
  }

  return 0;
}
