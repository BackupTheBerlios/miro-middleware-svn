// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef SparrowBaseConnection_hh
#define SparrowBaseConnection_hh


#include "sparrow/Parameters.h"

#include "can/CanConnection.h"         // CanConnection, CanMessageClass

//------------------------ SparrowClass ---------------------------//

namespace Sparrow
{
  class BaseConnection : public Can::Connection
  {
    typedef Can::Connection Super;

  public:
    BaseConnection(ACE_Reactor * _reactor,
	       Miro::DevEventHandler * _eventHandler);

    virtual ~BaseConnection();

    virtual void init();


  protected:


    ACE_Reactor * reactor;
    Parameters const * const params_;


  };
};

#endif

