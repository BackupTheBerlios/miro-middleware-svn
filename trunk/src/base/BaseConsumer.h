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
#ifndef BaseConsumer_h
#define BaseConsumer_h

#include "miro/Synch.h"

#include "mcp/McpConsumer.h"

#include "BaseStatus.h"

// forward decleration
namespace Miro
{
  class OdometryImpl;
};

namespace Base
{
  // forward decleration
  class Connection;

  // Soll die asynchronen Antworten der HardWare kapseln.
  // Stellt Referenzen auf Condition Variables zur Verfügung
  // über die auf neue Daten gewartet werden kann.

  class Consumer : public Mcp::Consumer
  {
    typedef Mcp::Consumer Super;

  public:
    Consumer(Connection& _connection, 
	     Miro::OdometryImpl* _odometry);
    virtual ~Consumer();

  protected:
    // Mcp message interface
    // the asynchronously called handler
    virtual void handleMessage(const Miro::DevMessage* message);

    // Battery interface
    virtual void handleBatteryLow()      {}
    virtual void handleBatteryHigh()     {}
    virtual void handleBatteryCurrent(unsigned long) {}
    virtual void handleBatteryVoltage(unsigned long value);

  protected:
    // error condition handlers
    virtual void handleTranslateError()  {}
    virtual void handleRotateError()     {}

    // handlers for commands that return values, 
    // the handlers sync the return value with the caller
    virtual void handleRotateCurrent(unsigned long value);
    virtual void handleRotateWhere(unsigned long) {}
    virtual void handleRotateHalt();

    virtual void handleTranslateCurrent(unsigned long value);
    virtual void handleTranslateWhere(unsigned long) {}
    virtual void handleTranslateHalt();
    
    // protected:
  public:
    Connection& connection;
    Status      status;

    Miro::Mutex     rotateHaltMutex;
    Miro::Condition rotateHaltCond;

    Miro::Mutex     translateHaltMutex;
    Miro::Condition translateHaltCond;

    Miro::Mutex     rotateCurrentMutex;
    Miro::Condition rotateCurrentCond;
    unsigned long   rotateCurrent;

    Miro::Mutex     translateCurrentMutex;
    Miro::Condition translateCurrentCond;
    unsigned long   translateCurrent;

    Miro::Mutex     batteryVoltageMutex;
    Miro::Condition batteryVoltageCond;
    double          batteryVoltage;

  protected:
    bool moduleRunning;

    int indexState;
    int indexing;

  private:
    void handleStatusReport(const Mcp::Message& message);
    void dispatchValue(const Mcp::Message& message);
    void handleError(const Mcp::Message& message); 

    // The types of errors the base can generate
    static const unsigned long TERR = 0x01;
    static const unsigned long TC   = 0x02;
    static const unsigned long RERR = 0x04;
    static const unsigned long RC   = 0x08;
    static const unsigned long BHI  = 0x10;
    static const unsigned long BLO  = 0x20;
  };
};
#endif




