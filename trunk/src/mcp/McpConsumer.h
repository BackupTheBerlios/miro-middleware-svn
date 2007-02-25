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
#ifndef McpConsumer_h
#define McpConsumer_h

// Derived from ACE tutorial chapter 11

#include "miro/DevConsumer.h"

namespace Mcp
{
  // forwar decleration
  class Connection;
  class Message;

  /**
   * Soll die asynchronen Antworten der HardWare kapseln.
   * Dechiffriert den byte strom der base hardware
   * und ruft jeweils methoden fuer die einzelnen
   * aufgaben auf
   */
  class Consumer : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;

  public:
    Consumer(Connection& connection_);
    virtual ~Consumer() {}

  protected:
  /**
   * Mcp message interface
   * the asynchronously called handler
   */
    virtual void handleMessage(const Miro::DevMessage* message);
    
    virtual void handleStatusReport(const Message&) {}
    virtual void handleError(const Message&) {}
    virtual void handleWatchdogTimeout();
  
  protected:
    Connection& connection;
  };
};
#endif




