// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "PanTiltImpl.h"
#include "PanTiltParameters.h"

#include "miro/Exception.h"

#include <ace/Reactor.h>
#include <ace/Timer_Queue.h>

#include <termios.h>
#include <linux/serial.h>

#include <iostream>
#include <cmath>

#undef DEBUG

namespace DpPanTilt 
{
  using std::string;

  using Miro::Log;
  using Miro::PanTiltPositionIDL;
  using Miro::PanTiltSpdAccIDL;
  using Miro::PanTiltLimitsIDL;
  using Miro::PanTiltPowersIDL;

  /***************************************************************************
   *
   *  PanTilt Implementation 
   *
   ***************************************************************************/

  PanTiltImpl::PanTiltImpl(Connection& _connection, Data& _data) :
    Log(WARNING,"PanTiltImpl"),
    connection(_connection),
    panTiltData(_data)
  {
  }

  PanTiltImpl::~PanTiltImpl()
  {
    log(WARNING, "Destructing PanTiltImpl.");

    // before this destructor is called

    log(WARNING, "destructor done.");

  }

  double
  PanTiltImpl::getPan() throw ()
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(5,0); // 5 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    log(Log::INFO, "getPan called.");

    Message p1("PP");
    connection.writeMessage( p1 );

    //  wait for result here
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PP Command failed");

    // we have got a feedback, convert to rad
    return pan2rad( panTiltData.panPosition );
  }

  double
  PanTiltImpl::getTilt()throw ()
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(5,0); // 5 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    log(Log::INFO, "getTilt called.");

    Message p2("TP");
    connection.writeMessage( p2 );

    //  wait for result here
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TP Command failed");

    // we have got a feedback
    return tilt2rad( panTiltData.tiltPosition );
  }

  PanTiltPositionIDL 
  PanTiltImpl::getPosition() throw ()
  {
    PanTiltPositionIDL result;

    log(Log::INFO, "getPosition called.");

    result.panvalue = getPan();
    result.tiltvalue = getTilt();

    return result;
  }

  void
  PanTiltImpl::setPan(double panvalue) throw ()
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(5,0); // 5 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    Message p1( "PP" );
    p1.addLong( rad2pan( panvalue ) );
    connection.writeMessage( p1 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PP<x> Command failed");
  }

  void
  PanTiltImpl::setTilt(double tiltvalue) throw ()
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(5,0); // 5 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    Message p2( "TP" );
    p2.addLong( rad2tilt( tiltvalue ) );
    connection.writeMessage( p2 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TP<X> Command failed");
  }

  void PanTiltImpl::setPosition(const PanTiltPositionIDL &dest) throw ()
  {
    log(Log::INFO, "setPosition called.");

    PanTiltImpl::setPan(dest.panvalue);
    PanTiltImpl::setTilt(dest.tiltvalue);
  }
  
  void
  PanTiltImpl::setWaitPosition(const PanTiltPositionIDL &dest) throw ()
  {
    log(Log::INFO, "setWaitPosition called.");

    PanTiltImpl::setPan(dest.panvalue);
    PanTiltImpl::setTilt(dest.tiltvalue);

    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(10,0); // 10 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    Message pm( "A" );      
    connection.writeMessage( pm );
    //  wait for result here
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("A Command failed");
  }
  
  PanTiltSpdAccIDL  
  PanTiltImpl::getSpdAcc() throw ()
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(20,0); // 10 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    PanTiltSpdAccIDL result;

    log(Log::INFO, "getSpdAcc called.");

    Message p1( "PS" );
    connection.writeMessage( p1 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PS Command failed");

    Message p2( "TS" );
    connection.writeMessage( p2 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TS Command failed");


    Message p3( "PB" );
    connection.writeMessage( p3 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PB Command failed");

    Message p4( "PL" );
    connection.writeMessage( p4 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PL Command failed");

    Message p5( "PU" );
    connection.writeMessage( p5 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PU Command failed");

    Message p6( "TB" );
    connection.writeMessage( p6 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TB Command failed");

    Message p7( "TL" );
    connection.writeMessage( p7 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TL Command failed");

    Message p8( "TU" );
    connection.writeMessage( p8 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TU Command failed");

    Message p9( "PA" );
    connection.writeMessage( p9 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PA Command failed");

    Message p0( "TA" );
    connection.writeMessage( p0 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TA Command failed");

    result.targetpanspeed = pan2rad( panTiltData.targetPanSpeed );
    result.targettiltspeed = tilt2rad( panTiltData.targetTiltSpeed );

    result.panbasespeed = pan2rad( panTiltData.panBaseSpeed );
    result.panminspeed = pan2rad( panTiltData.minPanSpeed );
    result.panmaxspeed = pan2rad( panTiltData.maxPanSpeed );

    result.tiltbasespeed = tilt2rad( panTiltData.tiltBaseSpeed );
    result.tiltminspeed = tilt2rad( panTiltData.minTiltSpeed );
    result.tiltmaxspeed = tilt2rad( panTiltData.maxTiltSpeed );

    result.panacc = pan2rad( panTiltData.panAcceleration );
    result.tiltacc = tilt2rad( panTiltData.tiltAcceleration );

    return result;
  }

  void PanTiltImpl::setSpdAcc( const PanTiltSpdAccIDL &dest ) throw ()
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(20,0); // 10 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    log(Log::INFO, "setSpdAcc called.");    

    Message p1( "PS" );
    p1.addLong( rad2pan( dest.targetpanspeed ) );
    connection.writeMessage( p1 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PS<X> Command failed");

    Message p2( "TS" );
    p2.addLong( rad2tilt( dest.targettiltspeed ) );
    connection.writeMessage( p2 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TS<X> Command failed");

    Message p3( "PB" );
    p3.addLong( rad2pan( dest.panbasespeed ) );
    connection.writeMessage( p3 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PB<X> Command failed");

    Message p4( "PL" );
    p4.addLong( rad2pan( dest.panminspeed ) );
    connection.writeMessage( p4 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PL<X> Command failed");

    Message p5( "PU" );
    p5.addLong( rad2pan( dest.panmaxspeed ) );
    connection.writeMessage( p5 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PU<X> Command failed");

    Message p6( "TB" );
    p6.addLong( rad2tilt( dest.tiltbasespeed ) );
    connection.writeMessage( p6 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TB<X> Command failed");

    Message p7( "TL" );
    p7.addLong( rad2tilt( dest.tiltminspeed ) );
    connection.writeMessage( p7 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TL<X> Command failed");

    Message p8( "TU" );
    p8.addLong( rad2tilt( dest.tiltmaxspeed ) );
    connection.writeMessage( p8 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TU<X> Command failed");

    Message p9( "PA" );
    p9.addLong( rad2pan( dest.panacc ) );
    connection.writeMessage( p9 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PA<X> Command failed");

    Message p0( "TA" );
    p0.addLong( rad2tilt( dest.tiltacc ) );
    connection.writeMessage( p0 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TA<X> Command failed");

  }
  


  Miro::PanTiltLimitsIDL  PanTiltImpl::getLimits() throw ()
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(20,0); // 10 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    PanTiltLimitsIDL result;
    log(Log::INFO, "getLimits called.");

    Message p1( "PN" );
    connection.writeMessage( p1 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PN Command failed");

    Message p2( "PX" );
    connection.writeMessage( p2 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PX Command failed");

    Message p3( "TN" );
    connection.writeMessage( p3 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TN Command failed");

    Message p4( "TX" );
    connection.writeMessage( p4 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TX Command failed");

    result.minpanposition = pan2rad( panTiltData.minPanPosition );
    result.maxpanposition = pan2rad( panTiltData.maxPanPosition );
    result.mintiltposition = tilt2rad( panTiltData.minTiltPosition );
    result.maxtiltposition = tilt2rad( panTiltData.maxTiltPosition );

    return result;
  }

  void PanTiltImpl::setPowers( const Miro::PanTiltPowersIDL & dest ) throw ()
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(20,0); // 10 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    log(Log::INFO, "setPowers called.");

    Message p1( "PH" );
    if (dest.panhold == 0)
      p1.addString( "O" );
    else if (dest.panhold == 1)
      p1.addString( "L" );
    else
      p1.addString( "R" );

    connection.writeMessage( p1 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PH<x> Command failed");


    Message p4( "TH" );
    if (dest.tilthold == 0)
      p4.addString( "O" );
    else if (dest.tilthold == 1)
      p4.addString( "L" );
    else
      p4.addString( "R" );
    connection.writeMessage( p4 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TH<x> Command failed");


    Message p7( "PM" );
    if (dest.panmove == 0)
      p7.addString( "L" );
    else if (dest.panmove == 1)
      p7.addString( "R" );
    else
      p7.addString( "H" );
    connection.writeMessage( p7 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PM<x> Command failed");


    Message p10( "TM" );
    if (dest.tiltmove == 0)
      p10.addString( "L" );
    else if (dest.tiltmove == 1)
      p10.addString( "R" );
    else
      p10.addString( "H" );
    connection.writeMessage( p10 );

    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TM<x> Command failed");

  }

  Miro::PanTiltPowersIDL  PanTiltImpl::getPowers() throw ()
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(20,0); // 10 seconds for data request (far to long, but its just here to return sometime)
    ACE_Guard<ACE_Thread_Mutex> guard(panTiltData.sync);

    PanTiltPowersIDL result;
    log(Log::INFO, "getPowers called.");

    Message p1( "PH" );
    connection.writeMessage( p1 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PH Command failed");


    Message p4( "TH" );
    connection.writeMessage( p4 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TH Command failed");


    Message p7( "PM" );
    connection.writeMessage( p7 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("PM Command failed");


    Message p10( "TM" );
    connection.writeMessage( p10 );
    abstimeout = ACE_OS::gettimeofday();
    abstimeout += timeout;
    if (panTiltData.syncCond.wait(&abstimeout) < 0) 
      throw Miro::ETimeOut();
    if (panTiltData.failed)
      throw Miro::EDevIO("TM Command failed");

    result.panhold = panTiltData.panHold;

    result.tilthold = panTiltData.tiltHold;

    result.panmove = panTiltData.panMove;

    result.tiltmove = panTiltData.tiltMove;

    return result;
  }

};
