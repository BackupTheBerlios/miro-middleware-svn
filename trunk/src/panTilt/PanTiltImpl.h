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
#ifndef PanTiltImpl_h
#define PanTiltImpl_h

#include "idl/DirectedPerceptionPanTiltS.h"

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
  class PanTiltImpl :  public virtual POA_Miro::DirectedPerceptionPanTilt
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
    PanTiltImpl(Connection & _connection, Data & _data);

    /**
     * Destructor
     */
    virtual ~PanTiltImpl();

    // -----------------------------------------------------------------------
    // Pan interface
    virtual void setPan(double panvalue) throw();
    virtual double getPan() throw();

    // -----------------------------------------------------------------------
    // Tilt interface
    virtual void setTilt(double tiltvalue) throw();
    virtual double getTilt() throw();

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
     * returns the actual speed and acceleration settings of the pantilt 
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




