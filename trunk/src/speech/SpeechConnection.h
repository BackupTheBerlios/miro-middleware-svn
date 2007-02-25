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
#ifndef SpeechConnection_h
#define SpeechConnection_h

#include "miro/Exception.h"
#include "idl/ExceptionC.h"

#include <ace/DEV_Connector.h>

// forward declarations
extern "C"
{
  struct dtlk_settings;
};

namespace Miro
{
  class CException;
  class EDevIO;
};

/**
 * Class for Communication with the speech hardware.
 *
 * On instanciation it sets up a connection to the DoubleTalk hardware.
 * Since there exists a hardware driver in current linux kernel for
 * this device, we can talk to it propperly via the file interface.
 *
 * On destruction the connection is automatically closed down again.  
 */
class SpeechConnection
{
public:

  /**
   * Default constructor.
   *
   * Opens up a file descriptor of /dev/dtlk.
   */
  SpeechConnection() throw(Miro::CException);
  /** Cleans up the file handle. */
  virtual ~SpeechConnection();
  
  /**
   * Write the string to the device.
   */
  void writeString(const char* out) throw(Miro::EDevIO);
  void writeChar(char out) throw(Miro::EDevIO);

  /**
   * Get status.
   *
   * Reads status information of the device via an io ctrl.
   */
  void getSettings(dtlk_settings * settings) const throw(Miro::EDevIO);

  void catInfo() const throw(Miro::EDevIO);
  void setBootMessage(const char * message) throw(Miro::EDevIO);
  char getStatus() const throw(Miro::EDevIO);

protected:
  ACE_DEV_Addr deviceName;
  ACE_DEV_IO ioBuffer;
  ACE_DEV_Connector connector;
 
private: 
};
#endif




