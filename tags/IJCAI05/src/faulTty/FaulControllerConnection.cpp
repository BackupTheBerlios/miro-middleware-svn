// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "FaulControllerConnection.h"
#include "miro/Log.h"

namespace FaulController
{
  Connection::Connection()
  {
    MIRO_LOG_CTOR("FaulController::Connection");
  }

  Connection::~Connection()
  {
    MIRO_LOG_DTOR("FaulController::Connection");
  }
}
