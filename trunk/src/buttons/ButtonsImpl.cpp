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


#include "ButtonsImpl.h"
#include "ButtonsDevice.h"
#include "ButtonsConnection.h"
#include "ButtonsEventHandler.h"

#include "miro/Exception.h"
#include "miro/TimeHelper.h"

// #undef DEBUG

#ifdef DEBUG
#include <iostream>
using std::cout;
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace {
  const ACE_Time_Value oneSecond(1, 0);
};

namespace B21Buttons
{
  using Miro::CException;
  using Miro::ACE_Exception;
  using Miro::EDevIO;
  using Miro::EOutOfBounds;

  //
  // Constructors / Destructors
  //

  B21ButtonsImpl::B21ButtonsImpl(Connection& _connection,
				 EventHandler * _pEventHandler)
    throw(ACE_Exception, EDevIO) :
    connection(_connection),
    pEventHandler(_pEventHandler)
  {
    DBG(cout << "Constructing B21ButtonsImpl" << endl);
  }
  
  B21ButtonsImpl::~B21ButtonsImpl()
  {
    DBG(cout << "Destructing B21ButtonsImpl" << endl);
  } 
  
  void
  B21ButtonsImpl::test() throw(EDevIO)
  {
    unsigned char pressed = connection.getButtonStatus();
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < NUM_BUTTONS; ++j) {
	setButtonMode(j, Miro::B21Button::TOGGLE_ON);
	ACE_OS::sleep(oneSecond);
      }
      for (int k = 0; k < NUM_BUTTONS; ++k) {
	setButtonMode(k, Miro::B21Button::TOGGLE_OFF);
	ACE_OS::sleep(oneSecond);
      }
      if (pressed == connection.getButtonStatus())
	break;
    }

    DBG(cout << "B21Buttons ready." << endl);
  }

  CORBA::ULong 
  B21ButtonsImpl::numberOfButtons() throw()
  {
    return NUM_BUTTONS;
  }

  CORBA::Boolean
  B21ButtonsImpl::isPressed(CORBA::ULong id) throw(EOutOfBounds)
  {
    if (id >= NUM_BUTTONS) 
      throw Miro::EOutOfBounds();

    return (pEventHandler->button[id].event == Miro::Button::ON_PRESS);
  }

  void 
  B21ButtonsImpl::setButtonMode(CORBA::ULong id, Miro::B21Button::Mode mode) 
    throw(EDevIO, EOutOfBounds)
  {
    if (id >= NUM_BUTTONS) 
      throw Miro::EOutOfBounds();

    unsigned char lights = connection.getButtonLights();

    if (pEventHandler->button[id].event == Miro::Button::ON_PRESS) {
      CORBA::Any any;

      pEventHandler->button[id].event = Miro::Button::ON_RELEASE;
      //      any <<= pEventHandler->button[id];
      //      consumer->push(any);
    }

    pEventHandler->button[id].mode = mode;
    if (mode == Miro::B21Button::ON ||
	mode == Miro::B21Button::TOGGLE_ON ||
	mode == Miro::B21Button::ON_TILL_PRESSED ||
	mode == Miro::B21Button::OFF_WHILE_PRESSED)
      {
	lights |= 0x01 << id;
      }

    if (mode == Miro::B21Button::OFF ||
	mode == Miro::B21Button::TOGGLE_OFF ||
	mode == Miro::B21Button::OFF_TILL_PRESSED ||
	mode == Miro::B21Button::ON_WHILE_PRESSED )
      {
	lights &= ~(0x01 << id);
      }

    connection.setButtonLights(lights);
  }

  Miro::B21Button::Mode
  B21ButtonsImpl::getButtonMode(CORBA::ULong id) throw(EOutOfBounds)
  {
    if (id >= NUM_BUTTONS) 
      throw Miro::EOutOfBounds();

    return pEventHandler->button[id].mode;
  }
};
