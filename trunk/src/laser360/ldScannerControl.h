// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005
// Department of Software Technology, Technical University of Graz, Austria
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
#ifndef _LD_SCANNDER_CONTROL_H_
  #define _LD_SCANNDER_CONTROL_H_

  #include "ldBaseTelegram.h"
  #include "ldSerialCom.h"
  #include "ldTelegramCodes.h"

  #include <sstream>
  #include <vector>
  #include <iomanip>

namespace Miro
{
  class RangeSensorImpl;
};

namespace Laser360
{
  class Parameters;
};

namespace ldoem_
{

  // forward declaration


  //---------------------------------------------------------------------------
  /// class to control the scanner
  class ScannerControl
  {

  public:

    /// vector for command string items
    typedef std::vector < uint16 > vectorItem;
    /// vector for distance
    typedef std::vector < float > vectorDistance;
    /// source address
    static const uint16 VECTOR_SIZE;
    /// Number of sectors of the scanner
    static const uint16 SECTOR_SIZE;
    /// Number of global configuration
    static const uint16 GLOBAL_CONFIG;
    /// Angle step
    static const uint16 ANGLE_STEP;
    /// Message header
    static const uint16 HEADER;
    /// CRC
    static const uint16 CRC;

    //---------------------------------------------------------------------------
    /// constructor
    ScannerControl( Miro::RangeSensorImpl & _laserI );

    //---------------------------------------------------------------------------
    /// destructor
    ~ScannerControl();

    //---------------------------------------------------------------------------
    /// loop
    int16 controlLoop();

  protected:

    //---------------------------------------------------------------------------
    // protected methods
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// reference to the LaserImpl class, for signaling data via condition variables
    /// LaserConnection & laser_;
    Miro::RangeSensorImpl & laserI_;

    //---------------------------------------------------------------------------
    /// xml parameter handle
    const ::Laser360::Parameters & parameters_;


  private:

    //---------------------------------------------------------------------------
    // private methods
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// control states
    enum eStates
    {
      IDENT = 1, RESP_IDENT, RESP_RESET, RESP_CONFIG, RESP_STEP, RESP_FUNCTION, RESP_ROTATE, RESP_MEASURE, RESP_SIGNAL,
         RESP_PROFILE, CONTINUOUSLY, RESET, EXIT
    };

    //---------------------------------------------------------------------------
    /// initial
    ///
    /// Initializing scanner control and all its components
    void initial();

    //---------------------------------------------------------------------------
    /// getIdentification
    ///
    /// Information about the LD type, firmware and application version
    /// get name of the sensor
    /// @param name of the current state
    void getIdentification( std::string state );

    //---------------------------------------------------------------------------
    /// doReset
    ///
    /// The LD enters a reset sequence
    /// reset the scanner and set the scanner to idle state
    /// @param name of the current state
    void doReset( std::string state );

    //---------------------------------------------------------------------------
    /// setConfig
    ///
    /// Sets the serial interface configuration
    /// @param name of the current state
    void setConfig( std::string state );

    //---------------------------------------------------------------------------
    /// setFunction
    ///
    /// Assigns a measurement function to an angle range
    /// @param name of the current state
    void setFunction( std::string state );

    //---------------------------------------------------------------------------
    /// setAngleStep
    ///
    /// Sets the angle of measurement distance
    void setAngle( std::string state );

    //---------------------------------------------------------------------------
    /// transeRotate
    ///
    /// Sets the LD into the MEASURE mode: the laser starts with the next revolution;
    /// a request for a profile can be started
    /// @param name of the current state
    void transeRotate( std::string state );

    //---------------------------------------------------------------------------
    /// transeMeasure
    ///
    /// Sets the LD into the MEASURE mode: the laser starts with the next revolution;
    /// a request for a profile can be started
    /// @param name of the current state
    void transeMeasure( std::string state );

    //---------------------------------------------------------------------------
    /// setSignal
    ///
    /// Sets the switches and LEDs
    /// @param name of the current state
    void setSignal( std::string state );

    //---------------------------------------------------------------------------
    /// getProfile
    ///
    /// Sets the LD into the MEASURE mode: the laser starts with the next revolution;
    /// a request for a profile can be started
    /// @param name of the current state
    void getProfile( std::string state );

    //---------------------------------------------------------------------------
    /// getContinuouslyProfile
    ///
    void getContinuouslyProfile( std::string state );

    //---------------------------------------------------------------------------
    /// resetState
    ///
    // reset the scanner and set the scanner to idle state and finished application
    /// @param name of the current state
    void exitState( std::string state );

    //---------------------------------------------------------------------------
    /// setState
    ///
    /// @param nextState
    void setState( eStates nextState );

    //---------------------------------------------------------------------------
    /// displayCurrentState
    ///
    /// @param message sting of the current state
    void displayCurrentState( std::string message, bool display_on );

    //---------------------------------------------------------------------------
    /// displayMessage
    void displayMessage( std::string state,  bool display_on );

    //---------------------------------------------------------------------------
    /// checkMessage
    void parseMeasurementHeader();

    //---------------------------------------------------------------------------
    /// parseMeasurement
    void parseMeasurementData( std::string message, uint16 package_count );

    //---------------------------------------------------------------------------
    /// checkCrc
    bool checkCrc( std::string & data );

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// Identification ident for getIdentification command string
    static uint16 name_of_sensor;

    //---------------------------------------------------------------------------
    /// Reset level for doReset command string
    static uint16 reset_level_;

    //---------------------------------------------------------------------------
    /// Interface item for setConfig command string
    static uint16 interface_item_;

    //---------------------------------------------------------------------------
    /// Interface item for setConfig command string
    static uint16 baudrate_;

    //---------------------------------------------------------------------------
    /// Interface item for setConfig command string
    static uint16 parity_;

    //---------------------------------------------------------------------------
    /// Interface item for setConfig command string
    static uint16 stop_bit_;

    //---------------------------------------------------------------------------
    /// Interface item for setConfig command string
    static uint16 character_;

    //---------------------------------------------------------------------------
    /// Interface item for setFunction command string
    static uint16 sector_number_[8];

    //---------------------------------------------------------------------------
    /// Interface item for setFunction command string
    static uint16 sector_function_[8];

    //---------------------------------------------------------------------------
    /// Interface item for setFunction command string
    static uint16 sector_stop_[8];

    //---------------------------------------------------------------------------
    /// Interface item for setFunction command string
    static uint16 flashflag_;

    //---------------------------------------------------------------------------
    /// Interface item for setAngle
    static uint16 angle_;

    //---------------------------------------------------------------------------
    /// Interface item for transeRotate command string
    static uint16 frequency_;

    //---------------------------------------------------------------------------
    /// Interface item for setSignal command string
    static uint16 port_value_;

    //---------------------------------------------------------------------------
    /// Interface item for getProfile command string
    static uint16 profile_number_;

    //---------------------------------------------------------------------------
    /// Interface item for getProfile command string
    static uint16 profile_format_;

    //---------------------------------------------------------------------------
    /// Profile count
    static uint16 profile_count_;

    //---------------------------------------------------------------------------
    /// Actual profile count
    static uint16 actual_profile_count_;

    //---------------------------------------------------------------------------
    /// Current state of scanner
    static uint16 state_;

    //---------------------------------------------------------------------------
    /// sector configuration count;
    static uint16 config_count_;

    //---------------------------------------------------------------------------
    /// sector step count;
    static uint16 step_count_;
    
    //---------------------------------------------------------------------------
    /// Display_on_ print command string to standard output
    static bool display_on_;

    //---------------------------------------------------------------------------
    /// Serial communication object
    static SerialCom * serial_com_;

    //--------------------------------------------------------------------------
    /// Vector for command string items
    static vectorItem vector_item_;

    //---------------------------------------------------------------------------
    /// First scan
    static bool first_profile_;

    //---------------------------------------------------------------------------
    /// distance
    static vectorDistance distance_;

    //---------------------------------------------------------------------------
    /// distance
    static vectorDistance step_;

    //---------------------------------------------------------------------------
    /// toggle
    static uint16 sequence_;

    //---------------------------------------------------------------------------
    /// toggle
    static bool toggle_;

    //---------------------------------------------------------------------------
    /// Timer
    Timer timeout;

  };

};
#endif
