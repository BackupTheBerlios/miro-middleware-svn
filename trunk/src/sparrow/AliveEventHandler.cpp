#include "AliveEventHandler.h"
#include <miro/SparrowAliveC.h>


namespace Sparrow{

   AliveEventHandler::AliveEventHandler(AliveCollector * collector_, Connection2003 * connection_,
                                                               Miro::StructuredPushSupplier * pSupplier_) :
   ACE_Event_Handler(),
   connection(connection_),
   collector(collector_),
   pSupplier(pSupplier_)
   {
     if (pSupplier) {
      notifyEvent.header.fixed_header.event_type.domain_name =
        CORBA::string_dup(pSupplier->domainName().c_str());
      notifyEvent.header.fixed_header.event_type.type_name =
	    CORBA::string_dup("SparrowAlive");
      notifyEvent.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent.header.variable_header.length(0);   // put nothing here
      notifyEvent.filterable_data.length(0);          // put nothing here
    }



   };

   int AliveEventHandler::handle_timeout (const ACE_Time_Value &current_time, const void *act){

      //std::cout << "Handle Timeout" << std::endl;

      Miro::SparrowAliveIDL aliveIDL;
      aliveIDL.MotorAlive = collector->motorAlive();
      aliveIDL.KickerAlive = collector->kickAlive();
      aliveIDL.Infrared1Alive = collector->infrared1Alive();
      aliveIDL.Infrared2Alive = collector->infrared2Alive();
      aliveIDL.PanAlive = collector->panAlive();

      notifyEvent.remainder_of_body <<= aliveIDL;
      if(pSupplier)
         pSupplier->sendEvent(notifyEvent);

      return 0;
   };





};

