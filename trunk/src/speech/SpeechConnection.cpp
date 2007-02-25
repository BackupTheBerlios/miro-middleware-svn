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
#include "SpeechConnection.h"
#include "SpeechDevice.h"

#include "idl/ExceptionC.h"
#include "miro/Exception.h"

// #undef DEBUG

#include <string>
#include <iostream>

#undef DEBUG
#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::hex;
using std::dec;
using std::string;
using namespace Miro;

//
// Constructors / Destructors
//

SpeechConnection::SpeechConnection() throw(CException) :
  // open file descriptor
  deviceName("/dev/dtlk"),
  ioBuffer(),
  connector()
{
  DBG(cout << "Constructing SpeechConnection" << endl);

  if (connector.connect(ioBuffer, deviceName) == -1) {
    cerr << "C Error " << errno << ": " << std::strerror(errno) << endl;
    throw CException(errno, std::strerror(errno));
  }
}
  
SpeechConnection::~SpeechConnection()
{
  DBG(cout << "Destructing SpeechConnection" << endl);

  ioBuffer.close();
} 
  
void 
SpeechConnection::writeString(const char* value) throw(EDevIO) 
{
  if (ioBuffer.send_n(value, strlen(value)) == -1) {
    cerr << "C Error " << errno << ": " << std::strerror(errno) << endl;
    throw EDevIO();
  }
}

void 
SpeechConnection::writeChar(char value) throw(EDevIO) 
{
  if (ioBuffer.send_n(&value, 1) == -1) {
    cerr << "C Error " << errno << ": " << std::strerror(errno) << endl;
    throw EDevIO();
  }
}

void
SpeechConnection::getSettings(dtlk_settings * settings) const throw(EDevIO)
{
  if (ioctl(ioBuffer.get_handle(), DTLK_INTERROGATE, settings) == -1) {
    cerr << "C Error " << errno << ": " << std::strerror(errno) << endl;
    throw EDevIO();
  }
}

void 
SpeechConnection::catInfo() const throw(EDevIO)
{
  dtlk_settings settings;
  getSettings(&settings);

  cout << "double talk settings:" << endl
       << " serial number: 0x" << hex << (int)settings.serial_number << dec << endl
       << " rom version: " << settings.rom_version << endl
       << " mode: " << (int)settings.mode << endl
       << " punc_level: " << (int)settings.punc_level << endl
       << " formant_freq: " << (int)settings.formant_freq << endl
       << " pitch: " << (int)settings.pitch << endl
       << " speed: " << (int)settings.speed << endl
       << " volume: " << (int)settings.volume << endl
       << " tone: " << (int)settings.tone << endl
       << " expression: " << (int)settings.expression << endl
       << " extended dictionary loaded: " << (int)settings.ext_dict_loaded << endl
       << " extended dictionary status: " << (int)settings.ext_dict_status << endl
       << " free ram: " << (int)settings.free_ram << endl
       << " articulation: " << (int)settings.articulation << endl
       << " reverb: " << (int)settings.reverb << endl
       << " has indexing: " << (int)settings.has_indexing << endl;
}

void
SpeechConnection::setBootMessage(const char * message) throw(EDevIO)
{
  writeString("\01W");
  writeString(message);
  writeChar(0);
}

char
SpeechConnection::getStatus() const throw(EDevIO)
{
  char status = 0;

  if (ioctl(ioBuffer.get_handle(), DTLK_STATUS, status) == -1) {
    cerr << "C Error " << errno << ": " << std::strerror(errno) << endl;
    throw EDevIO();
  }
  return status;
}
