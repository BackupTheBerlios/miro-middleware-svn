// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef OdometryTracking_h
#define OdometryTracking_h

#include "MotionStatusC.h"
#include "StructuredPushConsumer.h"

#include <list>
#include <string>

namespace Miro
{
  class OdometryTracking : public StructuredPushConsumer
  {
    typedef StructuredPushConsumer Super;
    
  public:
    OdometryTracking(CosNotifyChannelAdmin::EventChannel_ptr _ec,
		     const std::string& domainName);

    /** inherited IDL interface: StructuredPushSupplier methods */
    virtual void push_structured_event(const CosNotification::StructuredEvent & notification
				       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosEventComm::Disconnected);

    MotionStatusIDL getPosition(const ACE_Time_Value& stamp) const;
    PositionIDL getRawPosition(const ACE_Time_Value& stamp) const;

  protected:
    static const unsigned long maxDequeSize = 128;

    typedef std::list<Miro::MotionStatusIDL> OdometryDeque;
    typedef std::list<Miro::RawPositionIDL> RawDeque;

    mutable Miro::Mutex odoMutex;
    mutable Miro::Mutex rawMutex;
    OdometryDeque odoDeque;
    bool odoTruncate_;
    RawDeque rawDeque;
    bool rawTruncate_;
  };
};

#endif
