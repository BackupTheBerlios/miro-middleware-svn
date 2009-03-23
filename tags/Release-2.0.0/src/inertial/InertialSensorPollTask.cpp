#include "InertialSensorPollTask.h"

#include <string>
#include <iostream>
#include <stdio.h>

#include "miro/Log.h"



namespace Inertial
{

  // *******************************************************************************************************************
  /** Concret Implementation of the InertialSensorreader
  *
  */

  Inertial::InertialSensorPollTask::InertialSensorPollTask( XsensInertialSensorConnection& _connection,
        EventHandler& _event, InertialSensorImpl& _i_impl ) :
        inertial_ ( _connection ),
        event_ ( _event ),
        i_impl_ ( _i_impl ),
        parameters_ ( *InertialParameters::instance ( ) )
  {
    MIRO_LOG_CTOR ( "Inertial::InertialSensorPollTask" );
  }

  Inertial::InertialSensorPollTask::~InertialSensorPollTask( )
  {
    MIRO_LOG_DTOR ( "Inertial::InertialSensorPollTask" );
  }

  int Inertial::InertialSensorPollTask::svc()
  {
    // DEBUG
    //std::cout << "InertialSensorPollTask: " << std::endl;
    ACE_Time_Value abstimeout, last, now;
//    ACE_Time_Value timeout( 1, 0 );
    Miro::InertialSensorEventIDL *data = new Miro::InertialSensorEventIDL;

    MIRO_LOG_OSTR( LL_NOTICE, "polling intervall is : " << (int)parameters_.pollInterval.msec() );

    std::cout << "Interval: " << (int)parameters_.pollInterval.usec() << std::endl;

    while ( true )
    {
      //std::cout << "Interval: " << (int)parameters_.pollInterval.usec() << std::endl;
      last = ACE_OS::gettimeofday();

      inertial_.readData( *data );

      i_impl_.integrateData( *data );

//      abstimeout = ACE_OS::gettimeofday();
//      abstimeout += timeout;
/*      {
        Miro::Guard guard ( inertial_.syncInertialSensorScan );
      } */

      //delay some time
      now = ACE_OS::gettimeofday( );

      ACE_OS::sleep( parameters_.pollInterval - (now - last ) );
    }

    MIRO_LOG ( LL_NOTICE, "Inertial::aloah ey" );

    return( 0 );
  }

}
