#ifndef AliveCollector_h
#define AliveCollector_h

#include <ace/Time_Value.h>
#include <miro/Synch.h>


namespace Sparrow{

   class AliveCollector
   {
      public:

          AliveCollector();

	  bool kickAlive();
	  bool panAlive();
	  bool infraredAlive();
	  bool motorAlive();
	  void setLastKickAlive(ACE_Time_Value last_);
	  void setLastPanAlive(ACE_Time_Value last_);
	  void setLastInfraredAlive(ACE_Time_Value last_);
	  void setLastMotorAlive(ACE_Time_Value last_);

	  Miro::Mutex AliveCollectorMutex;

      protected:

          ACE_Time_Value lastKickAlive;
	  ACE_Time_Value lastPanAlive;
	  ACE_Time_Value lastInfraredAlive;
	  ACE_Time_Value lastMotorAlive;



   };


};

#endif


