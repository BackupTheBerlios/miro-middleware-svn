#include "miro/Server.h"
#include "miro/StructuredPushConsumer.h"
#include "idl/RangeEventC.h"
#include "miro/IO.h"

#include <string>
#include <iostream>

using std::cout;
using std::cerr;
using std::string;
using CORBA::Environment;
using CORBA::SystemException;
using CosEventComm::Disconnected;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;
using Miro::Server;
using Miro::StructuredPushConsumer;
using Miro::RangeBunchEventIDL;

struct SonarNotify : public StructuredPushConsumer
{
  // Initializing Constructor.
  // Registers for the events, that it wants to get pushed.
  SonarNotify(EventChannel_ptr _ec, const string& domainName) :
    StructuredPushConsumer(_ec)
  {
    EventTypeSeq added, removed;

    added.length(1);
    added[0].domain_name =  CORBA::string_dup(domainName.c_str());
    added[0].type_name = CORBA::string_dup("Sonar");

    consumerAdmin_->subscription_change(added, removed);
  }

  // Inherited IDL interface.
  // Called for every event by the event channel. 
  void push_structured_event(const StructuredEvent& notification
			     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    throw(SystemException, Disconnected) 
  {
    // Get a pointer to the sensor data of the structured event.
    const RangeBunchEventIDL * pSonarEvent;
    if (notification.remainder_of_body >>= pSonarEvent) 
      cout << *pSonarEvent << endl; // Print to standart out.
    else  // Crises? What crises?
      cerr << "No  RangeBunchEventIDL message." << endl;
  }
};

int main(int argc, char *argv[])
{
  Server server(argc, argv);  // Create a server orb.
  try { // Resolve the channel by name.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));
    // The consumer, that gets the events.
    SonarNotify pushConsumer(ec.in(), server.namingContextName);
    
    server.run();  // Enter CORBA event loop.
  }              
  catch (const CORBA::Exception & e) { // Catch CORBA and Miro exceptions.
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
