#ifndef Joystick_h
#define Joystick_h

#include <miro/MotionC.h>
#include <miro/KickerC.h>
#include <ace/Event_Handler.h>



class EventHandler : public ACE_Event_Handler {
        
	private:
	   Miro::Motion_var motion_;
	   Miro::Kicker_var kicker_;
	   
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
    
};




#endif
