#ifndef AliveEventHandler_h
#define AliveEventHandler_h

#include <ace/Event_Handler.h>
#include <orbsvcs/CosNotifyCommC.h>
#include "miro/StructuredPushSupplier.h"
#include "SparrowConnection2003.h"
#include "AliveCollector.h"

namespace Sparrow{

   class AliveEventHandler : public ACE_Event_Handler
   {
      public:
         AliveEventHandler(AliveCollector* collector_, Connection2003 * connection_,
	                              Miro::StructuredPushSupplier * pSupplier_);
	 virtual int handle_timeout (const ACE_Time_Value &current_time, const void *act=0);


        Connection2003 * connection;
	AliveCollector * collector;
	Miro::StructuredPushSupplier * pSupplier;
	CosNotification::StructuredEvent notifyEvent;

   };





};

#endif


