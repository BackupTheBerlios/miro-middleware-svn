// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast EventHandler
//
//
//  (c) 2002
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <phbaer@openums.org>
//
//
//  Version:
//    1.0.3
//
//
//  Description:
//
//    This event handler periodically calls (with the help of a reactor) the
//    handle_timeout routine of the receiver to invalidate events that are
//    incomplete for longer time.
//
//
//  Changes:
//
//    1.0.3:
//    - Some clean ups
//    - Uses Config object instead of ACE_HANDLE
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef NotifyMulticastEventHandler_h
#define NotifyMulticastEventHandler_h

#include <ace/Event_Handler.h>

namespace Miro 
{
  namespace NMC 
  {
    // forward declaration 
    class Receiver;
    class Config;
    
    class EventHandler : public ACE_Event_Handler 
    {
      typedef ACE_Event_Handler Super;
      
    public:
      //! Initializing constructor.
      EventHandler(ACE_HANDLE _handle, Receiver *_receiver);
      
      //! Inherited interface.
      virtual int handle_input(ACE_HANDLE _handle);
      //! Inherited interface.
      virtual ACE_HANDLE get_handle() const;
      
    protected:
      ACE_HANDLE handle_;
      Receiver * receiver_;
    };
  }
}

#endif // NotifyMulticastEventHandler_h
