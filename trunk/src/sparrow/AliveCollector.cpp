#include "AliveCollector.h"
#include <ace/OS.h>


namespace Sparrow{

    AliveCollector::AliveCollector():
    AliveCollectorMutex(),
    lastKickAlive(ACE_Time_Value(0,0)),
    lastPanAlive(ACE_Time_Value(0,0)),
    lastInfrared1Alive(ACE_Time_Value(0,0)),
    lastInfrared2Alive(ACE_Time_Value(0,0)),
    lastMotorAlive(ACE_Time_Value(0,0))
    {


    };


    bool AliveCollector::kickAlive(){

       Miro::Guard guard(AliveCollectorMutex);
       return (ACE_OS::gettimeofday()-lastKickAlive < ACE_Time_Value(2,0));

    };

    bool AliveCollector::panAlive(){

       Miro::Guard guard(AliveCollectorMutex);
       return (ACE_OS::gettimeofday()-lastPanAlive < ACE_Time_Value(2,0));

    };

    bool AliveCollector::infrared1Alive(){

       Miro::Guard guard(AliveCollectorMutex);
       return (ACE_OS::gettimeofday()-lastInfrared1Alive < ACE_Time_Value(2,0));

    };

    bool AliveCollector::infrared2Alive(){

       Miro::Guard guard(AliveCollectorMutex);
       return (ACE_OS::gettimeofday()-lastInfrared2Alive < ACE_Time_Value(2,0));

    };

    bool AliveCollector::motorAlive(){

       Miro::Guard guard(AliveCollectorMutex);
       return (ACE_OS::gettimeofday()-lastMotorAlive < ACE_Time_Value(2,0));

    };

    void AliveCollector::setLastKickAlive(ACE_Time_Value last_){

       Miro::Guard guard(AliveCollectorMutex);
       lastKickAlive = last_;

    };

    void AliveCollector::setLastPanAlive(ACE_Time_Value last_){

       Miro::Guard guard(AliveCollectorMutex);
       lastPanAlive = last_;

    };

    void AliveCollector::setLastInfrared1Alive(ACE_Time_Value last_){

       Miro::Guard guard(AliveCollectorMutex);
       lastInfrared1Alive = last_;

    };

    void AliveCollector::setLastInfrared2Alive(ACE_Time_Value last_){

       Miro::Guard guard(AliveCollectorMutex);
       lastInfrared2Alive = last_;

    };

    void AliveCollector::setLastMotorAlive(ACE_Time_Value last_){

       Miro::Guard guard(AliveCollectorMutex);
       lastMotorAlive = last_;

    };



};

