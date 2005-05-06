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


#include "SpeechImpl.h"
#include "SpeechDevice.h"

#include "miro/Exception.h"

#include <iostream>

// #undef DEBUG

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using namespace Miro;

//
// Constructors / Destructors
//

SpeechImpl::SpeechImpl()
  // ACE_Reactor* _reactor,
  // CosEventChannelAdmin::ProxyPushConsumer_ptr _consumer)
  throw(ACE_Exception, EDevIO) :
  connection()
  //  event(&connection, _consumer),
  //  reactor(_reactor),
  //  consumer(_consumer),
{
#ifdef DEBUG
  cout << "Constructing SpeechImpl" << endl;
#endif
}
  
SpeechImpl::~SpeechImpl()
{
#ifdef DEBUG
  cout << "Destructing SpeechImpl" << endl;
#endif
} 
  
void 
SpeechImpl::insertText(const Miro::DtlkSpeechSettingsIDL& settings, const char* text) throw(EDevIO)
{
  char buffer[64] = {0};
  char * pBuffer = buffer;
  dtlk_settings currentSettings;

  connection.getSettings(&currentSettings);

  // mode
  if (settings.mode >= 0 && settings.mode != currentSettings.mode ) {
    char mode;
    switch (settings.mode) {
    case 0:
      mode = 'C';
      break;
    case 1:
      mode = 'D';
      break;
    case 2:
      mode = 'T';
      break;
    default:
      throw EOutOfBounds();
    }
    pBuffer += sprintf(pBuffer, "\01%c", mode);
  }

  // punctuation level
  if (settings.punc_level >= 0 && settings.punc_level != currentSettings.punc_level) {
    if (settings.punc_level > 7)
      throw EOutOfBounds();
    pBuffer += sprintf(pBuffer, "\01%i%c", (int) settings.punc_level, 'B');
  }

  // formant frequency
  if (settings.formant_freq >= 0 && settings.formant_freq != currentSettings.formant_freq) {
    if (settings.formant_freq > 9)
      throw EOutOfBounds();
    pBuffer += sprintf(pBuffer, "\01%i%c", (int) settings.formant_freq, 'F');
  }

  // pitch
  if (settings.pitch >= 0 && settings.pitch != currentSettings.pitch) {
    if (settings.pitch > 9)
      throw EOutOfBounds();
    pBuffer += sprintf(pBuffer, "\01%i%c", (int) settings.pitch, 'P');
  }

  // speed
  if (settings.speed >= 0 && settings.speed != currentSettings.speed) {
    if (settings.speed > 9)
      throw EOutOfBounds();
    pBuffer += sprintf(pBuffer, "\01%i%c", (int) settings.speed, 'S');
  }

  // volume
  if (settings.volume >= 0 && settings.volume != currentSettings.volume) {
    if (settings.volume > 9)
      throw EOutOfBounds();
    pBuffer += sprintf(pBuffer, "\01%i%c", (int) settings.volume, 'V');
  }

  // tone
  if (settings.tone >= 0 && settings.tone != currentSettings.tone) {
    if (settings.tone > 2)
      throw EOutOfBounds();
    pBuffer += sprintf(pBuffer, "\01%i%c", (int) settings.tone, 'V');
  }

  // expression
  if (settings.expression >= 0 && settings.expression != currentSettings.expression) {
    if (settings.expression > 9)
      throw EOutOfBounds();
    pBuffer += sprintf(pBuffer, "\01%i%c", (int) settings.expression, 'E');
  }

  // articulation
  if (settings.articulation >= 0 && settings.articulation != currentSettings.articulation) {
    if (settings.articulation > 9)
      throw EOutOfBounds();
    pBuffer += sprintf(pBuffer, "\01%i%c", (int) settings.articulation, 'A');
  }

  // reverb
  if (settings.reverb >= 0 && settings.reverb != currentSettings.reverb) {
    if (settings.reverb > 9)
      throw EOutOfBounds();
    pBuffer += sprintf(pBuffer, "\01%i%c", (int) settings.reverb, 'R');
  }

  if (pBuffer != buffer)
    connection.writeString(buffer);
  connection.writeString(text);
}

void 
SpeechImpl::speak(const char * text) throw(EDevIO)
{
  insertText(getSettings(),text);
}

void 
SpeechImpl::insertCommand(char command, char value) throw(EDevIO)
{
  char buffer[8];

  if (value >= 0)
    sprintf(buffer,"\01%i%c", (int)value, command);
  else
    sprintf(buffer,"\01%c", command);
  connection.writeString(buffer);
}

Miro::DtlkSpeechSettingsIDL 
SpeechImpl::getSettings() throw()
{
  Miro::DtlkSpeechSettingsIDL tmp;
  dtlk_settings settings;

  connection.getSettings(&settings);

  tmp.mode         = settings.mode;
  tmp.punc_level   = settings.punc_level;
  tmp.formant_freq = settings.formant_freq;
  tmp.pitch        = settings.pitch;
  tmp.speed        = settings.speed;
  tmp.volume       = settings.volume;
  tmp.tone         = settings.tone;
  tmp.expression   = settings.expression;
  tmp.articulation = settings.articulation;
  tmp.reverb       = settings.reverb;

  return tmp;
}

void
SpeechImpl::skip() throw(EDevIO)
{
  connection.writeChar('\25');
}
void
SpeechImpl::clear() throw(EDevIO)
{
  connection.writeChar('\24');
}

void 
SpeechImpl::reset() throw(EDevIO)
{
  connection.writeString("\01@");
}

void 
SpeechImpl::setVoice(char voice) throw(EDevIO, EOutOfBounds)
{
  if (voice < 0 || voice > 7)
    throw EOutOfBounds();

  char buffer[6];
  sprintf(buffer, "\01%iO", (int)voice);
  connection.writeString(buffer);
}

