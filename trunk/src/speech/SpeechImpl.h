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




