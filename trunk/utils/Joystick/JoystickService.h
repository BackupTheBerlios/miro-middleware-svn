#ifndef JoystickService_h
#define JoystickService_h

#include <ace/Event_Handler.h>
#include <miro/StructuredPushSupplier.h>
#include <idl/MotionC.h>
#include <idl/JoystickC.h>


class EventHandler : public ACE_Event_Handler {
        
	private:
           Miro::StructuredPushSupplier *JoystickSupplier_;
	   ACE_HANDLE fd;
	   ACE_Time_Value time_;
	   static double oldrot;
	   static long oldtrans;
	   static int counter;
	   double minRotation;
	   long minTranslation;
	   CosNotification::StructuredEvent event_;
	   Miro::JoystickIDL OldStatus;
	   Miro::JoystickIDL CurStatus;
    
    	public:
	   EventHandler(Miro::StructuredPushSupplier *_JoystickSupplier,ACE_HANDLE fd_);
	   virtual int handle_input(ACE_HANDLE handle);	
	   virtual int handle_timeout(const ACE_Time_Value&, const void *);
           double maxRotation;
	   long maxTranslation;
	   double PropFaktor;
};




#endif
