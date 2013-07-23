// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id: SickLaserMessage.h,v 1.4 2005/09/23 11:43:11 hutz Exp $
//
//////////////////////////////////////////////////////////////////////////////
#ifndef InertialMessage_h
#define InertialMessage_h

#include "miro/Exception.h"

namespace Inertial
{
  /**
   * represents a message block sent to and received from the SICK
   * laserscanner.
   *
   * a message is structured as follows: (see also sick manual page 7/67)
   * STX (0x2)
   * Addr (1 byte)
   * Length (2 byte, including command byte, but excluding crc)
   * Cmd (1 byte)
   * Data (0..n bytes)
   * Status (1 byte, only in messages from the laserscanner)
   * Crc (2 bytes, computed over the whole message, including STX !)
   */
  class InertialMessage
  {

    float fdata_[18];


  public:
    /**
     * constructs an empty LaserMessage object
     */
    InertialMessage()
    {
      for( int i = 0; i < 18; i++ )
        fdata_[i] = 0.0f;
    }

    /**
     * constructs a LaserMessage object
     */
    InertialMessage( float _data[18] )
    {
      fillMessage( _data );
    }
/*    LaserMessage(char address_, char cmd_, unsigned short length_, const char * data_) {
      fillMessage(address_, cmd_, length_, data_);
    } */


    /**
     * fills buffer with data at appropriate places
     */
    void fillMessage( float *_data )
    {
      for( int i = 0; i < 18; i++ )
      {
        fdata_[i] = _data[i];
      }
    }
  };
}

#endif // LaserMessage_h






