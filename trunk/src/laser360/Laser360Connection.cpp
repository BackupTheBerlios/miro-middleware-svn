
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: Laser360Connection.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#include <ace/Reactor.h>

#include "Laser360Connection.h"
#include "Parameters.h"

#include <iostream>
#include <cmath>

#include "miro/Exception.h"
#include "miro/Log.h"

namespace Miro
{
  using std::string;
  using std::cout;
  using std::cerr;
  using std::endl;

  //---------------------------------------------------------------------------
  Laser360Connection::Laser360Connection(  RangeSensorImpl & _rangeSenorI ) :  parameters_( *::Laser360::Parameters::instance() ),
     laser360Task_( new Laser360Task( _rangeSenorI ) )
     {
       // start the thread handling (complete) messages
       laser360Task_->open( NULL );
  }

  //---------------------------------------------------------------------------
  Laser360Connection::~Laser360Connection()
  {
    MIRO_LOG_DTOR( "Laser360Connection" );

    laser360Task_->cancel();
    MIRO_DBG( SICK, LL_DEBUG, " laserPollTask exited." );

    MIRO_LOG_DTOR_END( "Laser360Connection" );
  }

  //---------------------------------------------------------------------------
  void Laser360Connection::stopTasks()
  {
    laser360Task_->wait ();
  }

};
