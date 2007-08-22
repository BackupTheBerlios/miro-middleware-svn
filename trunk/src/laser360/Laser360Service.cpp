// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005
// Department of Software Technology, Technical University of Graz, Austria
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "Laser360Service.h"
#include "Parameters.h"

namespace Miro
{

  //---------------------------------------------------------------------------
  LaserServer::LaserServer( int argc, char * argv[] ) : Miro::Server( argc, argv ), schedparams_( ACE_SCHED_FIFO, 10 ),
     ec_( Laser360::Parameters::instance()->notify ? tryResolveName < CosNotifyChannelAdmin::EventChannel > ( "EventChannel" ) :
     CosNotifyChannelAdmin::EventChannel::_nil() ),
     structuredPushSupplier_( ( !CORBA::is_nil( ec_.in() ) ) ? new StructuredPushSupplier( ec_.in(), namingContextName ) : NULL ),
     pLaser_( Laser360::Parameters::instance()->laserDescription, structuredPushSupplier_ ),
     connection_( pLaser_ )
     {

       if ( Laser360::Parameters::instance()->notify && CORBA::is_nil( ec_.in() ) )
       {
         MIRO_LOG_OSTR( LL_ERROR,
            "Could not resolve event channel in naming context " << namingContextName << "," << std::endl
            << "will not broadcast events." );
       }

       LaserObject_ = pLaser_._this();
       addToNameService( LaserObject_.in(), "Laser360" );

       MIRO_LOG_CTOR_END( "Miro::Laser360Server" );
  }

  //---------------------------------------------------------------------------
  LaserServer::~LaserServer()
  {

    MIRO_LOG_DTOR( "Miro::Laser360Server" );
    connection_.stopTasks();

    // delete laser;
    PortableServer::ObjectId_var oid = poa->reference_to_id( LaserObject_ );
    poa->deactivate_object( oid.in() );

    MIRO_LOG_DTOR_END( "Miro::Laser360Server" );
  }

}

//---------------------------------------------------------------------------
void calculateLaserDescription( Laser360::Parameters * parameters )
{
  parameters->laserDescription.scanType = Miro::RangeSensor::GROUPWISE;
  parameters->laserDescription.eventName = "Laser";
  parameters->laserDescription.group.length( 1 );
  parameters->laserDescription.group[0].description.minRange;
  parameters->laserDescription.group[0].description.maxRange;
  parameters->laserDescription.group[0].description.focus = Miro::deg2Rad( parameters->scanResolution );

  int values = ( int )( parameters->fov / parameters->scanResolution );
  parameters->laserDescription.group[0].sensor.length( values );
  for ( int i = 0; i < values; ++i )
  {
    parameters->laserDescription.group[0].sensor[i].height;
    parameters->laserDescription.group[0].sensor[i].distance;
    parameters->laserDescription.group[0].sensor[i].alpha =
       Miro::deg2Rad( -( parameters->fov / 2 ) + i * parameters->scanResolution );
    parameters->laserDescription.group[0].sensor[i].beta;
    parameters->laserDescription.group[0].sensor[i].gamma;
  }
}

//---------------------------------------------------------------------------
int main( int argc, char * argv[] )
{
  int rc = 0;

  //Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Laser360::Parameters * parameters = Laser360::Parameters::instance();

  try
  {

    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument( argc, argv );
    config->setSection( "Laser360" );
    delete config;

    calculateLaserDescription( parameters );

    MIRO_LOG( LL_NOTICE, "Initialize server daemon.\n" );
    Miro::LaserServer laserServer( argc, argv );

    try
    {
      MIRO_LOG( LL_NOTICE, "Loop forever handling events.\n" );
      laserServer.run( 8 );
      MIRO_LOG( LL_NOTICE, "Server loop ended, exiting.\n" );
    }
    catch ( const Miro::EOutOfBounds & e )
    {
      MIRO_LOG_OSTR( LL_CRITICAL,  "OutOfBounds exception: Wrong parameter for device initialization." << e );
      rc = 1;
    }
    catch ( const Miro::EDevIO & e )
    {
      MIRO_LOG_OSTR( LL_CRITICAL, "DevIO exception: Device access failed."  << e );
      rc = 1;
    }
    catch ( const CORBA::Exception & e )
    {
      MIRO_LOG_OSTR( LL_CRITICAL, "Uncaught CORBA exception: " << e );
      rc = 1;
    }
  }
  catch ( const Miro::CException & e )
  {
    MIRO_LOG_OSTR( LL_CRITICAL, "Miro C exception: " << e );
    rc = 1;
  }
  catch ( const Miro::Exception & e )
  {
    MIRO_LOG_OSTR( LL_CRITICAL, "Miro exception: " << e );
    rc = 1;
  }
  return rc;


}
