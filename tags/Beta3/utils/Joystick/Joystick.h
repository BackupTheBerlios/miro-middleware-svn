#ifndef Joystick_h
#define Joystick_h

#include <idl/MotionC.h>
#include <idl/KickerC.h>
#include <ace/Event_Handler.h>



class EventHandler : public ACE_Event_Handler {
        
	private:
	   Miro::Motion_var motion_;
	   Miro::Kicker_var kicker_;

	   Miro::VelocityIDL OldVelocity;
	   Miro::VelocityIDL CurVelocity;

	   ACE_HANDLE fd;
	   static double oldrot;
	   static long oldtrans;
	   bool kickeron_;
	   double minRotation;
	   double maxRotation;
	   long minTranslation;
	   long maxTranslation;
    
    	public:
	   EventHandler(Miro::Motion_var _motion, Miro::Kicker_var _kicker, bool
	   _kickeron, ACE_HANDLE fd_);
	   virtual int handle_input(ACE_HANDLE handle);
	   virtual int handle_timeout(const ACE_Time_Value&, const void *);
    
};




#endif
