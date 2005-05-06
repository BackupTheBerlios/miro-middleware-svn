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
#ifndef CanEventHandler_h
#define CanEventHandler_h

#include "miro/DevEventHandler.h"

namespace Can
{
  // forward declarations
  class Message;
  class Parameters;

  //! Event handler for reading can messages from a device.
  /** Can messages never come fragmented and can therefore be read at once. */
  class EventHandler : public Miro::DevEventHandler
  {
    //! Super class accessor.
    typedef Miro::DevEventHandler Super;

  public:
    //! Initializing constructor.
    EventHandler(Miro::DevConsumer* _consumer,
		 Parameters const * _params);
    //private:
    virtual ~EventHandler();

    //! Inherited method.
    /** Called to handle a pending can message, when returning from select. */
    virtual int handle_input (ACE_HANDLE fd); // fd select

    //! Factory method for can messages.
    /** Needed as we support two different can drivers. */
    static Message * newMessage(Parameters const * _params);

  protected:
    Message * msg_;
    Parameters const * params_;
  };
}
#endif



