
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: Laser360Task.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#include "Laser360Task.h"
#include "Parameters.h"

#include "miro/TimeHelper.h"
#include "miro/Log.h"

#include <iostream>
#include <sstream>

#include <netinet/in.h>


namespace Miro
{
  //---------------------------------------------------------------------------
  // Laser360Task
  Laser360Task::Laser360Task( RangeSensorImpl & _rangeSenorI ) :
     control_( new ldoem_::ScannerControl( _rangeSenorI ) )
     {

       MIRO_LOG_CTOR( "Miro::Laser360Task" );
  }

  //---------------------------------------------------------------------------
  // ~Laser360Task
  Laser360Task::~Laser360Task()
  {
    MIRO_LOG_DTOR( "Miro::Laser360Task" );

    delete control_;
  }

  //---------------------------------------------------------------------------
  // Now the svc() method where everything interesting happens.
  int  Laser360Task::svc()
  {

    control_->controlLoop();
  }

};
