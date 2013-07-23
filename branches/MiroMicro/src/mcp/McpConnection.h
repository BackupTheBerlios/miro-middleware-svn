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
#ifndef McpConnection_h
#define McpConnection_h

#include "miro/Synch.h"
#include "miro/TtyConnection.h"
#include "miro/Exception.h"
#include "idl/ExceptionC.h"

#include "McpDevice.h"
#include "McpMessage.h"

// forward declerations
class ACE_Reactor;

namespace Mcp
{
  // forward declerations
  class EventHandler;
  class Parameters;

  class Connection : public Miro::TtyConnection
  {
    typedef Miro::TtyConnection Super;

  public:
    Connection(ACE_Reactor* reactor_,
	       EventHandler* eventHandler_,
	       const Parameters& parameters_) 
      throw(Miro::Exception, Miro::EDevIO);
    virtual ~Connection();
  
    // Server Behaviour
    void setStatusReportPeriod(unsigned int period);
    unsigned int getStatusReportPeriod() { return reportPeriod; }
    void setStatusReportFormat(unsigned int format);
    unsigned int getStatusReportFormat() { return reportFormat; }
    
    // possible commands to send to all mcp devices
    void synch() const throw(Miro::EDevIO);
    void kill() throw(Miro::EDevIO);
    void delay(unsigned long delay) throw(Miro::EDevIO);

    void leaveDirectMode() throw(Miro::EDevIO);
    void errorDelay(unsigned long delay) throw(Miro::EDevIO);
    void errorAcknowledge(unsigned long err_ack) throw(Miro::EDevIO);
    void joystickDisable(unsigned long disable) throw(Miro::EDevIO);
    void statusReportRequest(unsigned long report) throw(Miro::EDevIO);
    void setStatusData(unsigned long data) throw(Miro::EDevIO);
    void setStatusPeriod(unsigned long period) throw(Miro::EDevIO);
    void loadHeading(unsigned long heading) throw(Miro::EDevIO);
    void loadPosition(unsigned short x, unsigned short y) throw(Miro::EDevIO);
    void watchdogTimer(unsigned long) throw(Miro::EDevIO);

    void sendCmd(const OutMessage * message) const;
    void sendCommands(const iovec * vector, size_t count);

  protected:
    EventHandler* eventHandler;

    int watchdogTimerId;

  public:
    bool moduleRunning;
    Miro::Mutex comMutex;
    Miro::Condition comCond;
    bool comBlocked;

    ACE_Time_Value lastWrite;
    static ACE_Time_Value writeTimeOut;

   protected:
    unsigned long reportFormat;
    unsigned long reportPeriod;
    
    void setBinaryMode() const throw(Miro::CException);
  };

  //---------------------------------------------------------------------------
  // inlines
  //---------------------------------------------------------------------------

  inline
  void
  Connection::synch() const throw(Miro::EDevIO)
  { 
    sendCmd(&MSG_SYNCH); 	
  }
  inline
  void
  Connection::leaveDirectMode() throw(Miro::EDevIO)
  { 
    sendCmd(&MSG_DIRECT_MODE); 	
  }
  inline
  void
  Connection::errorAcknowledge(unsigned long error_ack)
    throw(Miro::EDevIO)
  {
    OutMessage message(OP_ERROR_ACKNOWLEDGE, error_ack);
    sendCmd(&message);
  }
  inline
  void
  Connection::errorDelay(unsigned long arg) throw(Miro::EDevIO)
  {
    OutMessage message(OP_ERROR_DELAY, arg);
    sendCmd(&message);	
  }
  inline
  void 
  Connection::delay(unsigned long arg) throw(Miro::EDevIO)
  {
    OutMessage message(OP_DELAY, arg);
    sendCmd(&message);
  }
  inline
  void
  Connection::kill() throw(Miro::EDevIO)
  {
    sendCmd(&MSG_KILL);
  }
  inline
  void 
  Connection::joystickDisable(unsigned long arg) throw(Miro::EDevIO)
  { 
    OutMessage message(OP_JOYSTICK_DISABLE, arg);
    sendCmd(&message);	
  }
  inline
  void 
  Connection::setStatusData(unsigned long report) throw(Miro::EDevIO)
  {
    if (report) {
      // always make sure the report send what was requested
      // as first item
      OutMessage message(OP_PSR_DATA, report);
      sendCmd(&message);
    }
  }
  inline
  void 
  Connection::setStatusPeriod(unsigned long period) throw(Miro::EDevIO)
  {
    if (period < 26 && period != 0)
      period = 26;
    OutMessage message(OP_PSR_PERIOD, period);
    sendCmd(&message);
  }
  inline
  void 
  Connection::statusReportRequest(unsigned long report) 
    throw(Miro::EDevIO)
  {
    // always make sure the report send what was requested
    // as first item
    OutMessage message(OP_STATUS_REPORT, reportFormat | report);
    sendCmd(&message);
  }
  inline
  void
  Connection::loadHeading(unsigned long arg) throw(Miro::EDevIO)
  {
    OutMessage message(OP_LOAD_HEADING, arg);
    sendCmd(&message);
  }
  inline
  void
  Connection::loadPosition(unsigned short x, unsigned short y)
    throw(Miro::EDevIO)
  {
    unsigned long arg = (x << 16) | y;
    OutMessage message(OP_LOAD_POSITION, arg);

    sendCmd(&message);
  }

  inline
  void
  Connection::watchdogTimer(unsigned long interval) throw(Miro::EDevIO)
  {
    OutMessage message(OP_WATCH_DOG, interval);
    sendCmd(&message);
  }
}
#endif




