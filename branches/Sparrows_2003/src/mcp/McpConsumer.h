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




