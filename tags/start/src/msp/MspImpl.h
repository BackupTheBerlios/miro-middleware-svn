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
#ifndef MspImpl_h
#define MspImpl_h

#include "miro/InfraredS.h"
#include "miro/SonarS.h"
#include "miro/TactileS.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include "MspConnection.h"
#include "MspStruturedPushSupplier.h"

// forward declerations
class ACE_Reactor;
namespace Miro
{
  class Exception;
  class CException;
}
namespace Base
{
  class Status;
};

namespace Msp
{
  class Consumer;
  class EventHandler;
  class Parameters;

  class Impl
  {
  public:
    // Constructor/Destructor
    Impl(ACE_Reactor * _reactor, 
	 Base::Status& _status,  
	 CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin,
	 const Parameters& _parameters) throw(Miro::Exception);
    virtual ~Impl();
    
    Consumer&    getConsumer()   { return *consumer; }
    Connection&  getConnection() { return connection; }

  protected:
    ACE_Reactor *  reactor;    // for event handling (timers)
    StructuredPushSupplier structuredPushSupplier;
    Consumer *     consumer;   // asynchronous processing of mcp output
    EventHandler * eventHandler;
    Connection     connection; // encapsulating communication to hardware

  private:
  };

  /**
   * Implementation of the IDL interface
   */
  class InfraredImpl : public virtual POA_Miro::Infrared
  {
  public:
    // Constructor/Destructor
    InfraredImpl(Msp::Impl& impl );
    virtual ~InfraredImpl();

    //---------------------------------------------------------------------------
    // implementation of the idl interface
    //---------------------------------------------------------------------------

    Miro::InfraredScanIDL getScan();
    Miro::InfraredScanIDL getWaitScan() throw(Miro::ETimeOut);

  private:
    Consumer&       consumer;
  };

  /**
   * Implementation of the IDL interface
   */
  class SonarImpl : public virtual POA_Miro::Sonar
  {
  public:
    // Constructor/Destructor
    SonarImpl(Msp::Impl& impl );
    virtual ~SonarImpl();

    //-------------------------------------------------------------------------
    // implementation of the idl interface
    //-------------------------------------------------------------------------

    virtual Miro::SonarScanIDL getScan();
    virtual Miro::SonarScanIDL getWaitScan() throw(Miro::ETimeOut);

  private:
    Consumer&       consumer;

    static ACE_Time_Value maxWait;
  };

  /**
   * Implementation of the IDL interface
   */
  class TactileImpl : public virtual POA_Miro::Tactile
  {
  public:
    // Constructor/Destructor
    TactileImpl(Msp::Impl& impl);
    virtual ~TactileImpl();

    //-------------------------------------------------------------------------
    // implementation of the idl interface
    //-------------------------------------------------------------------------

    Miro::TactileScanIDL getScan();
    Miro::TactileScanIDL getWaitScan() throw(Miro::ETimeOut);

  private:
    Consumer&       consumer;
  };
};
#endif




