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


#include "MspImpl.h"

#include "miro/Synch.h"

#include "MspEventHandler.h"
#include "MspConsumer.h"

#undef DEBUG

#ifdef DEBUG
#include <iostream>

using std::cout;
using std::cerr;
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

// forward declarations
class ACE_Reactor;

/***************************************************************************
*
*  Base Implementation 
*
*  Handles output from base
*
***************************************************************************/
namespace Msp
{
  Impl::Impl(ACE_Reactor * _reactor,
	     Base::Status& _status,
	     CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin,
	     const Parameters& _parameters) throw(Miro::Exception) :
    reactor(_reactor),
    structuredPushSupplier(supplier_admin),
    consumer(new Consumer(connection, _status, structuredPushSupplier, _parameters)),
    eventHandler(new EventHandler(connection, consumer)),
    connection(_reactor, eventHandler, _parameters)
  {
    DBG(cout << "Constructing Impl" << endl);

    connection.sonarStart();
  }

  Impl::~Impl()
  {
    DBG(cout << "Destructing MspImpl" << endl);

    connection.sonarStop();
  }

  InfraredImpl::InfraredImpl(Impl& impl) :
    consumer(impl.getConsumer())
  {
  }

  InfraredImpl::~InfraredImpl()
  {
  }

  Miro::InfraredScanIDL
  InfraredImpl::getScan() 
  {
    Miro::Guard guard(consumer.infraredMutex);
    return consumer.infraredScan;
  }

  Miro::InfraredScanIDL
  InfraredImpl::getWaitScan() throw(Miro::ETimeOut)
  {
    Miro::Guard guard(consumer.infraredMutex);

    if (consumer.infraredCond.wait() == -1)
      throw Miro::ETimeOut();
    return consumer.infraredScan;
  }

  ACE_Time_Value SonarImpl::maxWait;

  SonarImpl::SonarImpl(Impl& impl) :
    consumer(impl.getConsumer())
  {
    // maximum wait time for cond.wait calls
    maxWait = ACE_Time_Value(0, 500000);
  }

  SonarImpl::~SonarImpl()
  {

  }

  Miro::SonarScanIDL SonarImpl::getScan() 
  {
    Miro::Guard guard(consumer.sonarMutex);
    return consumer.sonarScan;
  }

  Miro::SonarScanIDL SonarImpl::getWaitScan() throw(Miro::ETimeOut)
  {
    Miro::Guard guard(consumer.sonarMutex);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait;

    if (consumer.sonarCond.wait(&timeout) == -1)
      throw Miro::ETimeOut();
    return consumer.sonarScan;
  }

  TactileImpl::TactileImpl(Impl& impl) :
    consumer(impl.getConsumer())
  {
  }

  TactileImpl::~TactileImpl()
  {
  }

  Miro::TactileScanIDL
  TactileImpl::getScan() 
  {
    Miro::Guard guard(consumer.tactileMutex);

    return consumer.tactileScan;
  }

  Miro::TactileScanIDL 
  TactileImpl::getWaitScan() throw(Miro::ETimeOut)
  {
    Miro::Guard guard(consumer.tactileMutex);
    //    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    //    timeout += maxWait;

    if (consumer.tactileCond.wait() == -1)
      throw Miro::ETimeOut();
    return consumer.tactileScan;
  }
}
