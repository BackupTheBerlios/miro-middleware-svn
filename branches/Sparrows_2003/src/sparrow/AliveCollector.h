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
	  bool infrared1Alive();
	  bool infrared2Alive();
	  bool motorAlive();
	  void setLastKickAlive(ACE_Time_Value last_);
	  void setLastPanAlive(ACE_Time_Value last_);
	  void setLastInfrared1Alive(ACE_Time_Value last_);
	  void setLastInfrared2Alive(ACE_Time_Value last_);
	  void setLastMotorAlive(ACE_Time_Value last_);

	  Miro::Mutex AliveCollectorMutex;

      protected:

          ACE_Time_Value lastKickAlive;
	  ACE_Time_Value lastPanAlive;
	  ACE_Time_Value lastInfrared1Alive;
	  ACE_Time_Value lastInfrared2Alive;
	  ACE_Time_Value lastMotorAlive;



   };


};

#endif


