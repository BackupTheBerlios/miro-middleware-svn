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
#ifndef SparrowConsumer2003_h
#define SparrowConsumer2003_h

#include "SparrowDevice2003.h"
#include "AliveCollector.h"
#include "faulMotor/FaulMotorConsumer.h"
#include "Parameters.h"

#include "miro/Synch.h"
#include "miro/DevConsumer.h"
#include "miro/OdometryImpl.h"

// forward decleration
namespace Miro
{
  class StructuredPushSupplier;
  class RangeSensorImpl;
};

namespace Sparrow
{
  // forward declerations
  class Connection2003;

  class Consumer2003 : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;

  public:
    Consumer2003(Connection2003 * _connection,
	     Miro::OdometryImpl * _pOdometry,
	     Miro::RangeSensorImpl * _pIR1,
	     Miro::RangeSensorImpl * _pIR2);
    Consumer2003();
    ~Consumer2003();

    virtual void handleMessage(const Miro::DevMessage * _message);
    virtual void registerInterfaces(Connection2003 * _connection,
	     				Miro::OdometryImpl * _pOdometry,
	     				Miro::RangeSensorImpl * _pIR1,
					Miro::RangeSensorImpl * _pIR2, 
					FaulMotor::Consumer * _faulConsumer,
					AliveCollector * _aliveCollector);

    short * getTable1();
    short * getTable2();

  protected:
    Connection2003 * connection;
    Miro::OdometryImpl * pOdometry_;
    Miro::RangeSensorImpl * pIR1_;
    Miro::RangeSensorImpl * pIR2_;

    Parameters const * params_;

    Miro::MotionStatusIDL status_;
    FaulMotor::Consumer * faulConsumer;

    AliveCollector * pAliveCollector;


  public:
    unsigned char   digital[8];
    unsigned short  analog[16];

/*
    Miro::Mutex     irAliveMutex1;
    Miro::Condition irAliveCond1;

    Miro::Mutex     irAliveMutex2;
    Miro::Condition irAliveCond2;
*/


    //Miro::Condition accelCond;

    /*short xPos_;
    short yPos_;
    double phi_;

    short xPrev_;
    short yPrev_;
    double phiPrev_;

    double x_;
    double y_;

    short distanceL;
    short distanceR;*/

  protected:
    /*int index_;
    short table1[ACCEL_TABLE_SIZE];
    short table2[ACCEL_TABLE_SIZE];*/
  };
};
#endif

