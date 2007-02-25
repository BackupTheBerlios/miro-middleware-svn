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
#ifndef ButtonsConnection_h
#define ButtonsConnection_h

#include "miro/Exception.h"
#include "idl/ExceptionC.h"

#include <ace/FILE_Connector.h>

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class CException;
  class EDevIO;
};

namespace B21Buttons
{
  // forward declarations
  class EventHandler;

  /**
   * Class for Communication with the buttons hardware.
   *
   * On instanciation it sets up a connection to the buttons located on
   * top of the B21 robot. Those are connected via the paralel port, who
   * is address directly via /dev/ports.
   *
   * On destruction the connection is automatically closed down again.  
   */
  class Connection
  {
  public:

    /**
     * Default constructor.
     *
     * Opens up a file descriptor of /dev/ports. The kill buttons are lighted
     * all other buttons are initialised in mode TOGGLE_OFF.
     */
    Connection(ACE_Reactor * _reactor,
	       EventHandler * _pEventHandler) throw(Miro::CException);
    /** Cleans up the file handle. */
    virtual ~Connection();
  
    /**
     * Get the current status (pressed/unpressed) of the buttons.
     *
     * The buttons status are bit encoded, see buttonsDevice for the
     * relevant bits.
     */
    unsigned char getButtonStatus() throw(Miro::EDevIO);
    /**
     * Set the button lights.
     *
     * The button lights are bit encoded, see buttonsDevice for the
     * relevant bits.
     */
    void setButtonLights(unsigned char value) throw(Miro::EDevIO);
    /**
     * Get the status of the button lights.
     *
     * The button lights are bit encoded, see buttonsDevice for the
     * relevant bits.  
     */
    unsigned char getButtonLights() const { return buttonLights; }

  protected:
    ACE_Reactor * reactor;
    /** @ref ButtonsEvent instance for timer event processing. */
    EventHandler *  pEventHandler;

    ACE_FILE_Addr fileName;
    ACE_FILE_IO ioBuffer;
    ACE_FILE_Connector connector;

    // Holds the current button lights status.
    unsigned char buttonLights; 

    int pollingTimerId;
    int blinkingTimerId;
  };
};
#endif
