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
#ifndef MspEventHandler_h
#define MspEventHandler_h

#include "abus/AbusEventHandler.h"

namespace Msp
{
  class Connection;
  class Consumer;

  class EventHandler : public Abus::EventHandler
  {
    typedef Abus::EventHandler Super;

  public:
    EventHandler(Connection& _msp, Miro::DevConsumer * _mspConsumer);
    virtual ~EventHandler();

  protected:
    virtual void disconnect    (const Abus::Message& message);
    virtual void linkApproveAck(const Abus::Message& message);

  private:
    Connection& msp;
  };
};
#endif




