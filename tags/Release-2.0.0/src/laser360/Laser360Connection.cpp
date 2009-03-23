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
