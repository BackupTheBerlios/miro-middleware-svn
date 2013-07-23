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
#ifndef DpPanTiltImpl_h
#define DpPanTiltImpl_h

#include "idl/DirectedPerceptionPanTiltS.h"

#include "miro/PanTiltImpl.h"

#include "PanTiltData.h"
#include "PanTiltMessage.h"
#include "PanTiltConnection.h"
#include "PanTiltConsumer.h"
#include "PanTiltEventHandler.h"

#include <ace/Event_Handler.h>
#include <ace/Synch.h>

#include <exception>
#include <string>
#include <cmath>

namespace DpPanTilt 
{
  // forward declarations
  class Parameters;
  class EventHandler;

  /**
   * PanTiltImpl
   */
  class PanTiltImpl :  public virtual POA_Miro::DirectedPerceptionPanTilt,
		       public virtual Miro::PanTiltImpl
  {
  public:
    Connection& connection;
    /**
     * this class contains all data which could be received from 
     * the pantilt unit. 
     */
    Data& panTiltData;

  public:
    /**
     * Constructor
     */
    PanTiltImpl(Connection & _connection, 
		Data & _data,
		Miro::PanTiltParameters const& _panTiltParameters
		);

    /**
     * Destructor
     */
    virtual ~PanTiltImpl();

    // -----------------------------------------------------------------------
    // Pan interface
    virtual void setPan(CORBA::Float panvalue) throw();
    virtual CORBA::Float getPan() throw();
    /**
     * returns the pan angle limits
     */
    virtual Miro::PanLimitsIDL getPanLimits() throw();

    // -----------------------------------------------------------------------
    // Tilt interface
    virtual void setTilt(CORBA::Float tiltvalue) throw();
    virtual CORBA::Float getTilt() throw();
    /**
     * returns the tilt angle limits
     */
    virtual Miro::TiltLimitsIDL getTiltLimits() throw();

    // -----------------------------------------------------------------------
    // PanTilt interface
    virtual void setPosition( const Miro::PanTiltPositionIDL &dest) throw();

    /**
     * Get the current panTilt position. 
     */
    virtual Miro::PanTiltPositionIDL getPosition() throw();

    /** 
     * Move the PanTiltUnit to dest, the
     * call returns after the position is reached
     */
    virtual void setWaitPosition( const Miro::PanTiltPositionIDL &dest) throw();

    /**
     * sets the actual speed and acceleration settings
     */
    virtual void setSpdAcc( const Miro::PanTiltSpdAccIDL & dest ) throw();

    /**
     * returns the actual speed and acceleration settings of the pantilt 
     */
    virtual Miro::PanTiltSpdAccIDL  getSpdAcc() throw();

    /**
     * returns the pan and tilt angle limits of the pantilt 
     */
    virtual Miro::PanTiltLimitsIDL getPanTiltLimits() throw();

    /**
     * DEPRECATED.
     * Use getPanTiltLimits() instead
     */
    virtual Miro::PanTiltLimitsIDL  getLimits() throw();

    /**
     * sets the actual speed and acceleration settings
     */
    virtual void setPowers( const Miro::PanTiltPowersIDL & dest ) throw();

    /**
     * returns the actual speed and acceleration settings of the pantilt 
     */
    virtual Miro::PanTiltPowersIDL  getPowers() throw();

  protected:
    /**
     * converts from radians to a pan value in machine units
     */  
    static long rad2pan(double rad);
    /**
     * converts from radians to a tilt value in machine units
     */  
    static long rad2tilt(double rad);
    /**
     * converts from  a pan value in machine units to radians
     */  
    static double pan2rad(long pan);
    /**
     * converts from a tilt value in machine units to radians
     */  
    static double tilt2rad(long tilt);
  };

  inline
  long
  PanTiltImpl::rad2pan(double rad) {
    return static_cast<long>( rint ( (rad * 180.0 / M_PI) / (185.1428 / 3600.0) ) );
  }
  inline
  long
  PanTiltImpl::rad2tilt(double rad) {
    return static_cast<long>( rint ( (rad * 180.0 / M_PI) / (185.1428 / 3600.0) ) );
  }
  inline
  double
  PanTiltImpl::pan2rad(long pan) {
    return ( static_cast<double>( pan ) * (185.1428 / 3600.0) ) / 180.0 * M_PI;
  }
  inline
  double
  PanTiltImpl::tilt2rad(long tilt) {
    return ( static_cast<double>( tilt ) * (185.1428 / 3600.0) ) / 180.0 * M_PI;
  }
};
#endif




