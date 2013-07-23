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
#include "ButtonsImpl.h"
#include "ButtonsDevice.h"
#include "ButtonsConnection.h"
#include "ButtonsEventHandler.h"

#include "miro/Exception.h"
#include "miro/TimeHelper.h"

#include <iostream>

#ifdef DEBUG
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

  using std::cout;
  using std::cerr;
  using std::endl;

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
      for (unsigned long j = 0; j < NUM_BUTTONS; ++j) {
	setButtonMode(j, Miro::B21Button::TOGGLE_ON);
	ACE_OS::sleep(oneSecond);
      }
      for (unsigned long k = 0; k < NUM_BUTTONS; ++k) {
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
