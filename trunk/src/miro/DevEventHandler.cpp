// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "DevEventHandler.h"
#include "DevConsumer.h"
#include "DevMessage.h"
#include "Exception.h"
#include <miro/Log.h>

namespace Miro
{
  DevEventHandler::DevEventHandler(DevConsumer * _consumer, 
				   DevMessage * _message) :
    Super(),
    handle_(ACE_INVALID_HANDLE),
    consumer_(_consumer),
    message_(_message)
  {
    MIRO_DBG(MIRO,LL_CTOR_DTOR,"Constructing DevEvent.\n");
  }

  DevEventHandler::~DevEventHandler()
  {
    MIRO_DBG(MIRO,LL_CTOR_DTOR,"Destructing DevEvent.\n");

    delete consumer_;
    delete message_;
  }

  int
  DevEventHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
  {
    MIRO_DBG(MIRO,LL_NOTICE,"Device handle_close called.\n");

    // commit suizide
    delete this;
    return 0;
  }

  void
  DevEventHandler::set_handle(ACE_HANDLE _handle)
  {
    handle_ = _handle;
  }

  ACE_HANDLE
  DevEventHandler::get_handle() const
  {
    return handle_;
  }

  void
  DevEventHandler::dispatchMessage() 
  {
    consumer_->handleMessage(message_);
  }
}
