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
#ifndef SparrowPanTiltImpl_h
#define SparrowPanTiltImpl_h

#include "miro/PanTiltImpl.h"
#include "idl/SparrowPanTiltS.h"
#include "miro/Synch.h"
#include "miro/Angle.h"

#include "sparrow/Parameters.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;
}

namespace Sparrow
{
  // forward declaration
  class BaseConnection;
  class Connection2003;
  class Parameters;

  class  PanTiltImpl : public virtual POA_Miro::SparrowPanTilt,
		       public Miro::PanImpl
  {
  public:
    //! Constructor
    PanTiltImpl(BaseConnection * _connection,
		Miro::StructuredPushSupplier * _pSupplier = NULL);

    //! Destructor
    virtual ~PanTiltImpl();

    virtual void setPan(CORBA::Float value)
      throw (Miro::EDevIO, Miro::EOutOfBounds);
    virtual void setPanExt(CORBA::Float value, CORBA::Float _speed)
      throw (Miro::EDevIO, Miro::EOutOfBounds);  
    virtual void setPanSpeed(CORBA::Float speed)
      throw (Miro::EDevIO, Miro::EOutOfBounds);
    virtual CORBA::Float getPan() throw (Miro::EDevIO);

    virtual CORBA::Boolean panning(const Miro::TimeIDL& stamp) throw();
    virtual Miro::PanPositionIDL currentPan(const Miro::TimeIDL& stamp) throw();

  protected:
    bool prvPanning(const ACE_Time_Value& _t);
    Miro::PanPositionIDL currentPosition(const ACE_Time_Value& _t);

    Connection2003 * connection_;
    PanParameters const&  params_;

    Miro::StructuredPushSupplier * pSupplier_;
    CosNotification::StructuredEvent notifyEvent;

    double lastPosition;
    double nextPosition;
    ACE_Time_Value timeLastSet;

    ACE_Time_Value totalLatency;
  };

  inline
  bool
  PanTiltImpl::prvPanning(const ACE_Time_Value& _t)
  {
    if (timeLastSet < _t) {
      ACE_Time_Value t1 = _t; // elapsed time
      t1 -= timeLastSet;
      ACE_Time_Value t2; // time needed to do pan
      t2.set(fabs(nextPosition - lastPosition) / params_.panRadPerSec);
      t2 += totalLatency;

      return (params_.panLatency < t1 && t1 < t2);
    }
    else {
      return false;
    }
  }
}

#endif // SparrowPanTiltImpl_h
