// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "NotifyMulticastEventHandler.h"
#include "NotifyMulticastReceiver.h"

#include "Log.h"

namespace Miro 
{
  namespace NMC 
  {
    /**
     *   Parameters:
     *     _receiver: Pointer to receiver object
     *     _config:   Pointer to NotifyMulticast configuration
     */
    EventHandler::EventHandler(ACE_HANDLE _handle,
			       Receiver *_receiver) :
      handle_(_handle),
      receiver_(_receiver)
    {
      MIRO_LOG_CTOR("NMC::EventHandler");
    }

    /**
     *     Is called when input is available
     */
    int
    EventHandler::handle_input(ACE_HANDLE /*_handle*/) 
    {
      this->receiver_->handle_input();
      return 0;
    }


    /**
     *     Returns ACE handle
     */
    ACE_HANDLE
    EventHandler::get_handle() const 
    {
      return handle_;
    }
  }
}
