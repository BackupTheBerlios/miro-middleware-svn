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
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#ifndef ButtonsImpl_h
#define ButtonsImpl_h

#include "idl/B21ButtonsS.h"
#include "miro/Exception.h"

// forward declerations
class ACE_Reactor;

namespace B21Buttons
{
  // forward declaration
  class Connection;
  class EventHandler;

  /**
 * A servant implementation of the Buttons IDL interface.
 */
  class B21ButtonsImpl : public virtual ::POA_Miro::B21Buttons
  {
  public:
    /**
     * Initializing constructor.
     *
     * It takes a pointer to a reactor to register the button timers and
     * a pointer to an event channel to push button events to.
     */
    B21ButtonsImpl(Connection& _connection, 
		   EventHandler * _pEventHandler)
      throw(Miro::ACE_Exception, Miro::EDevIO);
    /** Cleaning up. */
    virtual ~B21ButtonsImpl();
  
    /** Testing routine */
    void test() throw(Miro::EDevIO);

    /** IDL interface method, see IDL documentation for details. */
    virtual CORBA::ULong numberOfButtons() throw();
    /** IDL interface method, see IDL documentation for details. */
    virtual CORBA::Boolean isPressed(CORBA::ULong id) throw(Miro::EOutOfBounds);
    /** IDL interface method, see IDL documentation for details. */
    virtual Miro::B21Button::Mode getButtonMode(CORBA::ULong name) 
      throw(Miro::EOutOfBounds);
    /** IDL interface method, see IDL documentation for details. */
    virtual void setButtonMode(CORBA::ULong name, Miro::B21Button::Mode mode) 
      throw(Miro::EDevIO, Miro::EOutOfBounds);

  protected:
    /** 
     * @ref ButtonsConnection instance holding the io connection to the
     *  buttons. 
     */
    Connection& connection;
    /** @ref ButtonsEvent instance for timer event processing. */
    EventHandler *  pEventHandler;
  };
};
#endif




