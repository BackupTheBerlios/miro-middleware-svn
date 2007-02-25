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
#ifndef SpeechImpl_h
#define SpeechImpl_h

#include "idl/DtlkSpeechS.h"

#include "SpeechConnection.h"

// #include "SpeechEvent.h"
// #include <orbsvcs/CosEventChannelAdminC.h>

// forward declerations
class ACE_Reactor;
namespace Miro
{
  class ACE_Exception;
};

/**
 * A servant implementation of the Speech IDL interface.
 */
class SpeechImpl : public virtual POA_Miro::DtlkSpeech
{
public:
  /**
   * Initializing constructor.
   *
   * It takes a pointer to a reactor to register the button timers and
   * a pointer to an event channel to push button events to.
   */
  SpeechImpl()
    // ACE_Reactor* _reactor, 
    // CosEventChannelAdmin::ProxyPushConsumer_ptr _consumer)
    throw(Miro::ACE_Exception, Miro::EDevIO);
  /** Cleaning up. */
  virtual ~SpeechImpl();
  
  /** IDL interface method, see IDL documentation for details. */
  virtual void insertText(const Miro::DtlkSpeechSettingsIDL& settings, const char * text) throw(Miro::EDevIO);

  /** IDL interface method, see IDL documentation for details. */
  virtual void speak(const char * text) throw(Miro::EDevIO);

  /** IDL interface method, see IDL documentation for details. */
  virtual void insertCommand(char command, char value) throw(Miro::EDevIO);
  virtual Miro::DtlkSpeechSettingsIDL getSettings() throw();

  virtual void skip() throw(Miro::EDevIO);
  virtual void clear() throw(Miro::EDevIO);
  virtual void reset() throw(Miro::EDevIO);
  virtual void setVoice(char voice) throw(Miro::EDevIO, Miro::EOutOfBounds);

public:
  /** 
   * @ref SpeechConnection instance holding the io connection to the
   *  speech. 
   */
  SpeechConnection connection;
  // /** @ref SpeechEvent instance for timer event processing. */
  // SpeechEvent event;
  // /** Reactor pointer for timer event registration. */ 
  // ACE_Reactor * reactor;
  // /** Event channel pointer to push button events into. */
  // CosEventChannelAdmin::ProxyPushConsumer_ptr consumer;

private:
};
#endif




