// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
//
// Authors: 
//   Philipp Baer
//
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
