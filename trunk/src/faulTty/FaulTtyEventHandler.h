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
#ifndef FaulTtyEventHandler_h
#define FaulTtyEventHandler_h

#include "miro/DevEventHandler.h"
#include "FaulTtyMessage.h"

namespace FaulTty
{
  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;

  public:
    EventHandler(Miro::DevConsumer* _consumer, OdometryMessage::Wheel _wheel);
    virtual ~EventHandler();

    virtual int handle_input (ACE_HANDLE fd); // fd select

  protected:
    char buff_[512];
    bool negate_;
  };
};
#endif



