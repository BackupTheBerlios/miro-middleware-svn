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
#ifndef SpeechConnection_h
#define SpeechConnection_h

#include "miro/Exception.h"
#include "miro/ExceptionC.h"

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




