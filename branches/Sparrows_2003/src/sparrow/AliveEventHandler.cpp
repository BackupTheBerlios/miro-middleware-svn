#include "AliveEventHandler.h"


namespace Sparrow{

   AliveEventHandler::AliveEventHandler(AliveCollector * collector_, Connection2003 * connection_) :
   ACE_Event_Handler(),
   connection(connection_),
   collector(collector_)
   {



   };

   int AliveEventHandler::handle_timeout (const ACE_Time_Value &current_time, const void *act){

      std::cout << "Handle Timeout" << std::endl;
      //if(!collector->kickAlive())
      //   connection->alivetimeout();
      

      return 0;
   };





};

