// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "SparrowBaseConnection.h"
#include "Parameters.h"

#include "miro/Log.h"

namespace Sparrow
{

  //------------------------//
  //----- constructors -----//
  //------------------------//

  BaseConnection::BaseConnection(ACE_Reactor* _reactor,
				 Miro::DevEventHandler* _devEventHandler):
    Super(_reactor, _devEventHandler, *Parameters::instance()),
    reactor(_reactor),
    params_(Parameters::instance())
  {
    MIRO_LOG_CTOR("BaseConnection");
  }

  BaseConnection::~BaseConnection()
  {
    MIRO_LOG_DTOR("BaseConnection");
  }

  void
  BaseConnection::init()
  {

  }
}
