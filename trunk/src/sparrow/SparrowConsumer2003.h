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

#include "SparrowDevice.h"
#include "AliveCollector.h"
#include "faulMotor/FaulMotorConsumer.h"
#include "sparrow/Parameters.h"

#include "miro/Synch.h"
#include "miro/DevConsumer.h"
#include "miro/OdometryImpl.h"

// forward decleration
namespace Miro
{
  class RangeSensorImpl;
}

namespace Sparrow
{
  // forward declerations
  class Connection2003;

  class Consumer2003 : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;

    // Roland: nach Miro-Konvention beginnen Typen
    // (auch Typedefs) mit einem Grossbuchsten.
    // methoden und membervariablen beginnen mit
    // Kleinbuchstaben
    // Und nur Member-Variablen haben einen _ am Ende
    // ausserdem wuerde ich diese Definitionen
    // als protected deklarieren.
    typedef std::vector<long> sensorv_;
    typedef std::vector<sensorv_> groupv_;
    typedef std::vector<groupv_> irvalues_;
    typedef std::vector<int> timeindex_;

  public:
    Consumer2003(Connection2003 * _connection,
	     Miro::OdometryImpl * _pOdometry,
	     Miro::RangeSensorImpl * _pIR1);
    Consumer2003();
    ~Consumer2003();

    virtual void handleMessage(const Miro::DevMessage * _message);
    virtual void registerInterfaces(Connection2003 * _connection,
	     				Miro::OdometryImpl * _pOdometry,
	     				Miro::RangeSensorImpl * _pIR1,
					FaulMotor::Consumer * _faulConsumer,
					AliveCollector * _aliveCollector);

    static const int INTEGRATION_NUMBER;

  protected:
    long integrateIrValues(unsigned int group, unsigned int sensor, long value);

    Connection2003 * connection;
    Miro::OdometryImpl * pOdometry_;
    Miro::RangeSensorImpl * pIR1_;

    irvalues_ IrValues;
    timeindex_ TimeIndex;

    Miro::ScanDescriptionIDL_var description_;

    Parameters const * params_;

    Miro::MotionStatusIDL status_;
    FaulMotor::Consumer * faulConsumer;

    AliveCollector * pAliveCollector;
  };
}
#endif

