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
#ifndef FaulControllerConnection_h
#define FaulControllerConnection_h

namespace FaulController
{
  // forward declarations
  class Connection
  {
  public:
    Connection();
    virtual ~Connection();

    virtual void sendAccVelTicks(short _accL, short _accR, 
				 short _velL, short _velR) = 0;
    virtual void sendAccVelTicks(short _acc, short _vel) = 0;

    //! Write binary controller commands.
    virtual void writeBinary(char const * _message, int _len) = 0;
    //! Write ascii message vectors.
    virtual void writeMessage(char const * const _message[]) = 0;
  };
}
#endif
