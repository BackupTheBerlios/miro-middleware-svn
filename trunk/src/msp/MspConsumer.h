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
#ifndef MspConsumer_h
#define MspConsumer_h

#include "miro/Synch.h"
#include "miro/DevConsumer.h"

#include "MspDevice.h"

#include <map>

// forward declerations
namespace Miro
{
  class RangeSensorImpl;
};

namespace Msp
{
  // forward decleration
  class Connection;
  class Message;
  class Parameters;

  class Consumer : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;
    typedef Msp::Consumer Self;
    
  public:
    Consumer(Connection& _msp, 
	     Miro::RangeSensorImpl * _pSonar,
	     Miro::RangeSensorImpl * _pInfrared,
	     Miro::RangeSensorImpl * _pTactile);

    virtual ~Consumer();
    
  protected:
    // Msp message interface
    // the asynchronously called handler
    virtual void handleMessage         (const Miro::DevMessage * msg);
    
    // RWI messages
    virtual void handleVersionReply    (const Message& msg);
    virtual void handleDebugString     (const Message& msg);
    virtual void handleDebugBinary     (const Message& msg);

    // Msp messages
    virtual void handleTactileReply    (const Message& msg);
    virtual void handleIrReply         (const Message& msg);
    virtual void handleIrParmsReply    (const Message&) {}
    virtual void handleSonarReply      (const Message& msg);
    virtual void handleSonarParmsReply (const Message&) {}
    virtual void handleSonarTableReply (const Message&) {}

    Connection&   msp;

    Miro::RangeSensorImpl * pSonar_;
    Miro::RangeSensorImpl * pInfrared_;
    Miro::RangeSensorImpl * pTactile_;

  public:
    bool tactileScan[TACTILE_ROWS][TACTILE_COLS];

    static std::map<unsigned long, int> sonarAddr2Num;
    static std::map<unsigned long, int> mspAddr2Num;
  };
};

#endif




