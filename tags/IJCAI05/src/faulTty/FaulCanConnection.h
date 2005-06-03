// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef FaulCanConnection_h
#define FaulCanConnection_h

#include "FaulControllerConnection.h"
#include "sparrow/SparrowConnection2003.h"

#include "miro/Synch.h"

// forward declarations
namespace FaulMotor {
  class Connection;
}

namespace FaulController
{
  class FaulCanConnection : public Connection
  {
    //! Super class accessor.
    typedef Connection Super;

  public:
    FaulCanConnection(Sparrow::Connection2003 * _connection2003, int _motor);
    virtual ~FaulCanConnection();

    virtual void sendAccVelTicks(short accL, short accR, 
				 short velL, short velR);
    virtual void sendAccVelTicks(short acc, short vel);

    virtual void writeBinary(char const * buffer, int _len);
    virtual void writeMessage(char const * const _message[]);

  protected:
    Sparrow::Connection2003 * connection2003_;
    int motor_;

    Miro::Mutex mutex_;
    ACE_Time_Value lastWrite_;
  };
}
#endif