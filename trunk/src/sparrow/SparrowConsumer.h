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
#ifndef SparrowConsumer_h
#define SparrowConsumer_h

#include "SparrowDevice.h"
#include "sparrow/Parameters.h"

#include "miro/Synch.h"
#include "miro/DevConsumer.h"
#include "miro/OdometryImpl.h"

// forward decleration
namespace Miro
{
  class RangeSensorImpl;
};

namespace Sparrow
{
  // forward declerations
  class Connection;
  class StallImpl;
  class ButtonsImpl;

  /**
   * Class for using the Sparrow robot
   */
  class Consumer : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;
    
  public:
    Consumer(Connection * _connection,
	     Miro::OdometryImpl * _pOdometry,
	     StallImpl * _pStall,
	     ButtonsImpl * _pButtons,
	     Miro::RangeSensorImpl * _pIR);
    Consumer();
    ~Consumer();

    virtual void handleMessage(const Miro::DevMessage * _message);
    virtual void registerInterfaces(Connection * _connection,
                                    Miro::OdometryImpl * _pOdometry,
                                    StallImpl * _pStall,
                                    ButtonsImpl * _pButtons,
                                    Miro::RangeSensorImpl * _pIR);

    short * getTable1();
    short * getTable2();

  protected:
    Connection * connection;
    Miro::OdometryImpl * pOdometry_;
    StallImpl * pStall_;
    ButtonsImpl * pButtons_;
    Miro::RangeSensorImpl * pIR_;

    Parameters const * params_;

    Miro::MotionStatusIDL status_;

  public:
    unsigned char   digital[8];
    unsigned short  analog[16];

    Miro::Mutex     motorAliveMutex;
    Miro::Condition motorAliveCond;
    Miro::Mutex     odoAliveMutex;
    Miro::Condition odoAliveCond;
    Miro::Mutex     portsAliveMutex;
    Miro::Condition portsAliveCond;
    Miro::Mutex     stallAliveMutex;
    Miro::Condition stallAliveCond;
    Miro::Mutex     kickerAliveMutex;
    Miro::Condition kickerAliveCond;
    Miro::Mutex     servoAliveMutex;
    Miro::Condition servoAliveCond;
    Miro::Mutex     dbgAliveMutex;
    Miro::Condition dbgAliveCond;

    Miro::Mutex     distanceLRMutex;
    Miro::Condition distanceLRCond;

    Miro::Mutex     digitalMutex;
    Miro::Condition digitalCond;

    Miro::Mutex     analogMutex;
    Miro::Condition analogCond;

    Miro::Mutex     irAliveMutex;
    Miro::Condition irAliveCond;

    Miro::Mutex     accelMutex;
    Miro::Condition accelCond;

    short xPos_;
    short yPos_;
    double phi_;

    short xPrev_;
    short yPrev_;
    double phiPrev_;

    double x_;
    double y_;

    short distanceL;
    short distanceR;

  protected:
    int index_;
    short table1[ACCEL_TABLE_SIZE];
    short table2[ACCEL_TABLE_SIZE];
  };
};
#endif

