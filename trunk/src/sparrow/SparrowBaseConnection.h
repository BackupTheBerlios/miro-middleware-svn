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
#ifndef SparrowBaseConnection_h
#define SparrowBaseConnection_h

#include "can/CanConnection.h"         // Super class

//------------------------ SparrowClass ---------------------------//

namespace Sparrow
{
  // forward declaration
  class Parameters;

  //! Base class for a connection to the sparrow hardware.
  class BaseConnection : public Can::Connection
  {
    //! Super class accessor.
    typedef Can::Connection Super;

  public:
    //! Initializing constructor.
    BaseConnection(ACE_Reactor * _reactor,
		   Miro::DevEventHandler * _eventHandler);
    //! Cleaning up.
    virtual ~BaseConnection();
    //! Init method to be called once by the event handler.
    virtual void init() = 0;
    virtual void fini() = 0;

  protected:
    //! Pointer to the parameters.
    Parameters const * const params_;
  };
}

#endif // SparrowBaseConnection_h

