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
#ifndef ButtonsImpl_h
#define ButtonsImpl_h

#include "miro/B21ButtonsS.h"
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




