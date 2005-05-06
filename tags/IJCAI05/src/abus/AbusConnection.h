// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef AbusConnection_h
#define AbusConnection_h

#include "miro/DevConnection.h"
#include "miro/Exception.h"

// forward declerations
namespace Miro
{
  class Exception;
};

namespace Abus
{
  // forward declerations
  class Message;

  class Connection : Miro::DevConnection
  {
    typedef Miro::DevConnection Super;

  public:
    Connection(ACE_Reactor * _reactor, 
	       Miro::DevEventHandler * _eventHandler,
	       const Miro::DevParameters& _parameters);
    virtual ~Connection();
  
    // possible commands to send to all mcp devices

    void reset       () throw(Miro::Exception);
    void linkRequest (const char *prot, const char *type, const char *model) throw(Miro::Exception);

    void linkApprove (unsigned char devId, unsigned char approve) throw(Miro::Exception);
    void getLongId   (unsigned char devId) throw(Miro::Exception);
    void getId       (unsigned char devId) throw(Miro::Exception);
    void getType     (unsigned char devId) throw(Miro::Exception);
    void getStatus   (unsigned char devId) throw(Miro::Exception);
    void enable      (unsigned char devId, unsigned char enable) throw(Miro::Exception);
    void disconnect  (unsigned char devId) throw(Miro::Exception);

    //  protected:
    void sendMessage (const Abus::Message* msp) throw(Miro::Exception);
  };
};

#endif




