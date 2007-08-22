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
#include "ldScannerControl.h"
#include "ldCrcTable.h"

#include "Parameters.h"
#include "miro/TimeHelper.h"
#include "miro/RangeSensorImpl.h"
#include "miro/Exception.h"
#include "miro/Log.h"

using namespace Miro;

namespace ldoem_
{

  /// vector for command string items
  typedef std::vector < uint16 > vectorItem;
  typedef std::vector < float > vectorDistance;

  //---------------------------------------------------------------------------
  //  initializing of static member variables
  SerialCom * ScannerControl::serial_com_;
  vectorItem ScannerControl::vector_item_;
  vectorDistance ScannerControl::distance_;
  vectorDistance ScannerControl::step_;

  bool ScannerControl::display_on_           = false;
  bool ScannerControl::first_profile_        = false;
  bool ScannerControl::toggle_               = false;
  const uint16 ScannerControl::VECTOR_SIZE   = 5;
  const uint16 ScannerControl::SECTOR_SIZE   = 8;
  const uint16 ScannerControl::ANGLE_STEP    = 16;
  const uint16 ScannerControl::HEADER        = 5;
  const uint16 ScannerControl::CRC           = 1;
  const uint16 ScannerControl::GLOBAL_CONFIG = 0x0010;
  uint16 ScannerControl::sequence_           = 0;
  uint16 ScannerControl::config_count_       = 0;
  uint16 ScannerControl::name_of_sensor      = 0x0001; // name of the sensor
  uint16 ScannerControl::reset_level_        = 0x0002; // halt application and enter idle state
  uint16 ScannerControl::interface_item_     = 0x0001; // rs232/rs422
  uint16 ScannerControl::baudrate_           = 0x0006; // 115200 baud
  uint16 ScannerControl::parity_             = 0x0000; // non parity
  uint16 ScannerControl::stop_bit_           = 0x0001; // one stop bit
  uint16 ScannerControl::character_          = 0x0008; // 8 bits per character
  uint16 ScannerControl::sector_number_[8]   =
    {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007 // sector number 0..7
  };
  // Measurement function for the sector
  // 0: not onotialised
  // 1: no measurement
  // 2: reserved
  // 3: normal measurement
  uint16 ScannerControl::sector_function_[8] =
    {
    0x0003, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 // sector function
  };
  // End angle of the sector
  uint16 ScannerControl::sector_stop_[8]     =
    {
    0x1670, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
  };
  uint16 ScannerControl::flashflag_            = 0x0001; // configuration written to flash memory
  uint16 ScannerControl::angle_                = 0x0010; // angle corresponds 1°
  uint16 ScannerControl::frequency_            = 0x0005; // frequency corresponds to the configuration parameter
  uint16 ScannerControl::port_value_           = 0x0001; // LED 0 (yellow)
  uint16 ScannerControl::profile_number_       = 0x0000; // profil number
  uint16 ScannerControl::profile_count_        = 0x0001; // profil count
  uint16 ScannerControl::step_count_           = 360;    // step count
  uint16 ScannerControl::actual_profile_count_ = 0;      // actual profile count
  // 0x0700 profil format - ( measured distances, direction of measured distances, echo amplitudes )
  uint16 ScannerControl::profile_format_     = 0x0700;
  uint16 ScannerControl::state_;

  //---------------------------------------------------------------------------
  ScannerControl::ScannerControl( RangeSensorImpl & _laserI ) : laserI_( _laserI ),
     parameters_( *::Laser360::Parameters::instance() )
     {
       vector_item_ = vectorItem( VECTOR_SIZE );
       config_count_ = 0;
       initial();

       // load scanner parameter configuration
       frequency_      = parameters_.frequency;
       // Angle 1° corresponds 16,  mimimal angle with RS232/422 interface is 0.5°
       angle_          = static_cast < uint16 > (16.0 * parameters_.scanResolution);

       profile_number_ = parameters_.profilnumber;
       profile_count_  = parameters_.profilcount;
       step_count_     = static_cast < uint16 > (parameters_.fov / parameters_.scanResolution);

       sector_function_[0] = parameters_.sectorfunction0;
       sector_stop_[0]     = parameters_.sectorstop0;

  }

  //---------------------------------------------------------------------------
  ScannerControl::~ScannerControl()
  {
    delete serial_com_;
  }

  //---------------------------------------------------------------------------
  bool ScannerControl::checkCrc( string & data )
  {
    std::istringstream iscrc;
    std::string sdata, scrc;

    uint16 crc = BaseTelegram::INITIAL_CRC;
    uint16 value = 0;
    uint16 checkcrc = 0;
    uint16 i = 0;

    // parse message without crc value
    sdata = data.substr( 0, ( data.size() - 4 ) );
    // parce crc value
    scrc = data.substr( ( data.size() - 4 ) , 4 );
    iscrc.str ( scrc );
    iscrc >> std::hex >> checkcrc;

    uint16 numofbytes = sdata.size() / sizeof( uint32 );

    while ( numofbytes-- )
    {
      value = 0;

      if ( ( sdata[i] >= '0' ) && ( sdata[i] <= '9' ) )
        value = ( ( sdata[i] - 48 ) << 12 );
      else
      {
        if ( ( sdata[i] == 'a' ) || ( sdata[i] == 'A' ) )
          value = ( 10 << 12 );
        if ( ( sdata[i] == 'b' ) || ( sdata[i] == 'B' ) )
          value = ( 11 << 12 );
        if ( ( sdata[i] == 'c' ) || ( sdata[i] == 'C' ) )
          value = ( 12 << 12 );
        if ( ( sdata[i] == 'd' ) || ( sdata[i] == 'D' ) )
          value = ( 13 << 12 );
        if ( ( sdata[i] == 'e' ) || ( sdata[i] == 'E' ) )
          value = ( 14 << 12 );
        if ( ( sdata[i] == 'f' ) || ( sdata[i] == 'F' ) )
          value = ( 15 << 12 );
      }

      if ( ( sdata[i + 1] >= '0' ) && ( sdata[i + 1] <= '9' ) )
        value = value | ( ( sdata[i + 1] - 48 ) << 8 );
      else
      {
        if ( ( sdata[i + 1] == 'a' ) || ( sdata[i + 1] == 'A' ) )
          value = value | ( 10 << 8 );
        if ( ( sdata[i + 1] == 'b' ) || ( sdata[i + 1] == 'B' ) )
          value = value | ( 11 << 8 );
        if ( ( sdata[i + 1] == 'c' ) || ( sdata[i + 1] == 'C' ) )
          value = value | ( 12 << 8 );
        if ( ( sdata[i + 1] == 'd' ) || ( sdata[i + 1] == 'D' ) )
          value = value | ( 13 << 8 );
        if ( ( sdata[i + 1] == 'e' ) || ( sdata[i + 1] == 'E' ) )
          value = value | ( 14 << 8 );
        if ( ( sdata[i + 1] == 'f' ) || ( sdata[i + 1] == 'F' ) )
          value = value | ( 15 << 8 );
      }

      if ( ( sdata[i + 2] >= '0' ) && ( sdata[i + 2] <= '9' ) )
        value = value | ( ( sdata[i + 2] - 48 ) << 4 );
      else
      {
        if ( ( sdata[i + 2] == 'a' ) || ( sdata[i + 2] == 'A' ) )
          value = value | ( 10 << 4 );
        if ( ( sdata[i + 2] == 'b' ) || ( sdata[i + 2] == 'B' ) )
          value = value | ( 11 << 4 );
        if ( ( sdata[i + 2] == 'c' ) || ( sdata[i + 2] == 'C' ) )
          value = value | ( 12 << 4 );
        if ( ( sdata[i + 2] == 'd' ) || ( sdata[i + 2] == 'D' ) )
          value = value | ( 13 << 4 );
        if ( ( sdata[i + 2] == 'e' ) || ( sdata[i + 2] == 'E' ) )
          value = value | ( 14 << 4 );
        if ( ( sdata[i + 2] == 'f' ) || ( sdata[i + 2] == 'F' ) )
          value = value | ( 15 << 4 );

      }

      if ( ( sdata[i + 3] >= '0' ) && ( sdata[i + 3] <= '9' ) )
        value = value | ( ( sdata[i + 3] - 48 ) );
      else
      {
        if ( ( sdata[i + 3] == 'a' ) || ( sdata[i + 3] == 'A' ) )
          value = value | ( 10 );
        if ( ( sdata[i + 3] == 'b' ) || ( sdata[i + 3] == 'B' ) )
          value = value | ( 11 );
        if ( ( sdata[i + 3] == 'c' ) || ( sdata[i + 3] == 'C' ) )
          value = value | ( 12 );
        if ( ( sdata[i + 3] == 'd' ) || ( sdata[i + 3] == 'D' ) )
          value = value | ( 13 );
        if ( ( sdata[i + 3] == 'e' ) || ( sdata[i + 3] == 'E' ) )
          value = value | ( 14 );
        if ( ( sdata[i + 3] == 'f' ) || ( sdata[i + 3] == 'F' ) )
          value = value | ( 15 );
      }

      crc = ( ( crc << 8 ) | ( ( static_cast < uint8 > ( value >> 8 ) ) ) ) ^ crctab[crc >> 8];
      crc = ( ( crc << 8 ) | ( static_cast < uint8 > ( value ) ) ) ^ crctab[crc >> 8];
      i += 4;
    }

    if ( crc == checkcrc )
    {
      return ( true );
    }
    else
    {
      return ( false );
    }
  }

  //---------------------------------------------------------------------------
  void ScannerControl::initial()
  {
    try
    {
      serial_com_ = new SerialCom();
    }
    catch ( SerialException e )
    {
      std::cerr << e.getMessage() << std::endl;
      exit( -1 );
    }
    // reset first scan
    first_profile_ = false;
    // reset toggle
    toggle_ = false;
    // set ident state
    setState( IDENT );
  }


  //---------------------------------------------------------------------------
  void ScannerControl::getIdentification( std::string state )
  {
    displayCurrentState( state, display_on_ );
    // set identifacation item
    vector_item_[0] = name_of_sensor;
    serial_com_->setItem( GET_IDENTIFICATION, vector_item_ );
    // send command string to scanner
    serial_com_->sendCommand( GET_IDENTIFICATION, display_on_ );
    // set next state
    setState( RESP_IDENT );
    timeout.wait( 40 );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::doReset( std::string state )
  {
    displayCurrentState( state, display_on_ );
    // set reset item
    vector_item_[0] = reset_level_;
    serial_com_->setItem( DO_RESET, vector_item_ );
    // send command string to scanner
    serial_com_->sendCommand( DO_RESET, display_on_ );
    // set next state
    setState( RESP_RESET );
    timeout.wait( 40 );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::setConfig( std::string state )
  {
    displayCurrentState( state, display_on_ );
    // set config items
    vector_item_[0] = interface_item_;
    vector_item_[1] = baudrate_;
    vector_item_[2] = parity_;
    vector_item_[3] = stop_bit_;
    vector_item_[4] = character_;
    serial_com_->setItem( SET_CONFIG, vector_item_ );
    // send command string to scanner
    serial_com_->sendCommand( SET_CONFIG, display_on_ );
    // set next state
    setState( RESP_CONFIG );
    timeout.wait( 40 );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::setFunction( std::string state )
  {
    displayCurrentState( state, display_on_ );
    if ( config_count_ < SECTOR_SIZE )
    {
      // set function items
      vector_item_[0] = sector_number_[config_count_];
      vector_item_[1] = sector_function_[config_count_];
      vector_item_[2] = ( sector_stop_[config_count_] - 1 ) * ANGLE_STEP;
      vector_item_[3] = flashflag_;
      serial_com_->setItem( SET_FUNCTION, vector_item_ );
      // send command string to scanner
      serial_com_->sendCommand( SET_FUNCTION, display_on_ );
    }
    config_count_ ++;
    // set next state
    setState( RESP_FUNCTION );
    timeout.wait( 40 );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::setAngle( std::string state )
  {
    displayCurrentState( state, display_on_ );

    // check angle
    if ( angle_ < 8 )
      angle_ = 0x0008;

    // set config items
    vector_item_[0] = GLOBAL_CONFIG;
    vector_item_[1] = GLOBAL_CONFIG;
    vector_item_[2] = 0x000A;
    vector_item_[3] = angle_;
    vector_item_[4] = 0x0000;
    serial_com_->setItem( SET_CONFIG, vector_item_ );
    // send command string to scanner
    serial_com_->sendCommand( SET_CONFIG, display_on_ );
    // set next state
    setState( RESP_STEP );
    timeout.wait( 40 );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::transeRotate( std::string state )
  {
    displayCurrentState( state, display_on_ );

    // check frequency
    if ( frequency_ > 15 || frequency_ < 5 )
      frequency_ = 0x000F;

    // set function items
    vector_item_[0] = frequency_;
    serial_com_->setItem( TRANSE_ROTATE, vector_item_ );
    // send command string to scanner
    serial_com_->sendCommand( TRANSE_ROTATE, display_on_ );
    // set next state
    setState( RESP_ROTATE );
    timeout.wait( 40 );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::transeMeasure( std::string state )
  {
    displayCurrentState( state, display_on_ );
    // send command string to scanner
    serial_com_->sendCommand( TRANSE_MEASURE, display_on_ );
    // set next state
    setState( RESP_MEASURE );
    timeout.wait( 40 );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::setSignal( std::string state )
  {
    displayCurrentState( state, display_on_ );
    // set function items
    vector_item_[0] = port_value_;
    serial_com_->setItem( SET_SIGNAL, vector_item_ );
    // send command string to scanner
    serial_com_->sendCommand( SET_SIGNAL, display_on_ );
    // set next state
    setState( RESP_SIGNAL );
    timeout.wait( 40 );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::getProfile( std::string state )
  {
    displayCurrentState( state, display_on_ );
    // set function items
    vector_item_[0] = profile_number_;
    if ( profile_number_ == 0 )
      profile_format_ = 0x0300;

    vector_item_[1] = profile_format_;

    serial_com_->setItem( GET_PROFILE, vector_item_ );
    // send command string to scanner
    serial_com_->sendCommand( GET_PROFILE, display_on_ );

    // set receive function to continuously measurement mode
    if ( profile_number_ == 0 )
    {
      serial_com_->setContinuously();
      // set next state
      setState( CONTINUOUSLY );
    }
    else
    {
      serial_com_->resetContinuously();
      actual_profile_count_++;
      // set next state
      setState( RESP_PROFILE );
    }
    timeout.wait( 40 );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::getContinuouslyProfile( std::string state )
  {
    displayCurrentState( state, display_on_ );

    RangeGroupEventIDL * data = new RangeGroupEventIDL();

    std::istringstream issequence;
    std::string message = "";
    std::string sdata, slength, svalue, ssequence;
    uint16 value, x, length;
    std::vector < float > vdistance, vstep;

    // get message string
    message = serial_com_->getRcvMessage();

    if ( ! checkCrc( message ) )
      return;

    // parse message length
    std::istringstream islength;
    slength = message.substr( 4 , 4 );
    islength.str( slength );
    islength >> std::hex >> length;

    // parse sequence number
    if ( ! first_profile_ )
    {
      ssequence =  message.substr( 12 , 4 );
      issequence.str( ssequence );
      issequence >> std::hex >> sequence_;
    }
    else
    {
      ssequence =  message.substr( 8 , 4 );
      issequence.str( ssequence );
      issequence >> std::hex >> sequence_;
    }

    if ( ! first_profile_ )
    {
      sdata = message.substr( 28, ( ( length - HEADER - CRC ) * 4 ) );
    }
    else
    {
      sdata = message.substr( 12 , ( ( length - 2 ) * 4 ) );
    }

    // substring length
    uint16 count = sdata.size() /  4;

    x = 0;
    for ( uint16 i = 0; i < count; i ++ )
    {
      std::istringstream isvalue;
      svalue = sdata.substr ( x, 4 );
      isvalue.str( svalue );
      isvalue >> std::hex >> value;
      if ( ! first_profile_ || ! toggle_ )
      {
        vdistance.push_back( value / 256.0 );
        toggle_ = true;
        // set first scan
        first_profile_ = true;
      }
      else
      {
        vstep.push_back( value / 16.0 );
        toggle_ = false;
      }
      x += 4;
    }
/*
    for ( uint16 i = 0; i < vdistance.size(); i++ )
    {
      distance_.push_back( vdistance[i] );
    }
*/       
    float distance_value = 0;
    while ( ! vdistance.empty() )
    {
       distance_value = vdistance.back();
       vdistance.pop_back();
       distance_.push_back(distance_value);
    }
   
    ACE_Time_Value now = ACE_OS::gettimeofday();
    timeA2C( now, data->time );
    data->group = 0;
    int vals = step_count_;
    data->range.length( vals );

    if  ( sequence_ == 1 )
    {
      for ( long i = vals - 1; i >= 0; --i )
      {
        if ( distance_[i] < 250 )
        {
          data->range[i] = static_cast<int>( distance_[i] * 1000);
        }
        else
          data->range[i] = 0;
      }
      laserI_.integrateData( data );
      distance_.clear();
      step_.clear();
      first_profile_  = false;
    }
    timeout.wait( 40 );
    actual_profile_count_++;
  }

  //---------------------------------------------------------------------------
  void ScannerControl::exitState( std::string state )
  {
    displayCurrentState( state, display_on_ );
    // set reset item
    vector_item_[0] = 0x0002;
    serial_com_->setItem( DO_RESET, vector_item_ );
    // send command string to scanner
    serial_com_->sendCommand( DO_RESET, display_on_ );
    // set next state
    setState( RESET );
  }

  //---------------------------------------------------------------------------
  void ScannerControl::setState( eStates nextState )
  {
    state_ = nextState;
  }

  //---------------------------------------------------------------------------
  void ScannerControl::displayCurrentState( std::string message, bool display_on )
  {
    if ( display_on ) 
    {
      std::cout << "< Send > ";
      std::cout.width( 15 );
      std::cout << message << " -> ";
    }  
  }

  //---------------------------------------------------------------------------
  void ScannerControl::displayMessage( std::string state,  bool display_on )
  {
    if ( display_on ) 
	{  
      std::string message = "";
      message = serial_com_->getRcvMessage();
      std::cout << "< Rcv  > ";
      std::cout.width( 15 );
      std::cout << state << " <- " << message << std::endl;
	}  
  }

  //---------------------------------------------------------------------------
  void ScannerControl::parseMeasurementHeader()
  {
    std::string message = "";
    std::istringstream iscount;
    std::string scount;
    uint16 count;

    // get message string
    message = serial_com_->getRcvMessage();

    // parse package count
    scount = message.substr ( 12, 4 );
    iscount.str( scount );
    iscount >> std::hex >> count;

    // parse measurement data
    parseMeasurementData( message, count );

    // wait
    timeout.wait( 500 );
    getProfile( "GetProfile" );

  }

  //---------------------------------------------------------------------------
  void ScannerControl::parseMeasurementData( std::string message, uint16 package_count )
  {
    RangeGroupEventIDL * data = new RangeGroupEventIDL();

    std::string sdata, sdistance, sstep, secho, slength;
    std::string s[package_count];
    uint16 distance, step, echo, x, y;
    uint16 length[package_count];
    std::vector < float > vstep, vdistance, vecho;

    uint16 package = package_count;

    // parse package length
    x = 0;
    for ( uint16 i = 1 ; i <= package; ++i )
    {
      std::istringstream islength;
      if ( i == 1 )
      {
        slength = message.substr ( ( x + 4 ) , 4 );
      }
      else
      {
        slength = message.substr ( ( ( x * 4 ) + 4 ) , 4 );
      }
      islength.str( slength );
      islength >> std::hex >> y;
      if ( y <= 
    		  126 )
        length[i - 1] = y;
      else
        return;
      x += y + 2;
    }

    // parse substrings from package
    x = 0;
    y = 0;
    for ( uint16 i = 0; i < package; ++i )
    {
      if ( i == 0 )
      {
        s[i] = message.substr( 28, ( ( length[i] - HEADER - CRC ) * 4 ) );
        x = ( length[i] + 2 ) * 4;
      }
      else
      {
        y = ( length[i] - 2 ) * 4;
        s[i] = message.substr( x + 12 , y );
        x += ( length[i] + 2 ) * 4;
      }
    }

    // merge substrings
    for ( uint16 i = 0; i < package; ++i )
      sdata += s[i];

    // substring length
    uint16 count = sdata.size() / ( 4 * 3 );

    
    // convert distance
    x = 0;
    for ( uint16 i = 0; i < count; i ++ )
    {
      std::istringstream isdistance;
      sdistance = sdata.substr ( x, 4 );
      isdistance.str( sdistance );
      isdistance >> std::hex >> distance;
      vdistance.push_back( distance / 256.0 );
      x += 12;
    }

    // convert direction
    x = 4;
    for ( uint16 i = 0; i < count; i ++ )
    {
      std::istringstream isstep;
      sstep = sdata.substr( x, 4 );
      isstep.str( sstep );
      isstep >> std::hex >> step;
      vstep.push_back( step / 16.0 );
      x += 12;
    }

    // convert echo
    x = 8;
    for ( uint16 i = 0; i < count; i ++ )
    {
      std::istringstream isecho;
      secho = sdata.substr( x, 4 );
      isecho.str( secho );
      isecho >> std::hex >> echo;
      vecho.push_back( echo );
      x += 12;
    }

    float value = 0;
    while ( ! vdistance.empty() )
    {
      value = vdistance.back();
      vdistance.pop_back();
      distance_.push_back(value);
    }
 /*
    for ( uint16 i = 0; i < vstep.size(); i++ )
    {
      std::cout << "< W: " << std::setw( 6 )  << vstep[i] << ' ' << "  D: " << std::setw( 10 ) << (distance_[i] * 1000) << ' '
         << "  E: " << std::setw( 4 )  << vecho[i] << " >" << std::endl;
    } 
*/
    ACE_Time_Value now = ACE_OS::gettimeofday();
    timeA2C( now, data->time );
    data->group = 0;
    int vals = step_count_;
    data->range.length( vals );

    for ( long i = vals - 1; i >= 0; --i )
    {
      if ( distance_[i] < 250 )
      {
        data->range[i] = static_cast<int>(distance_[i] * 1000);
      }
      else
    	  
        data->range[i] = 0;
    }
    laserI_.integrateData( data );
    distance_.clear();
  }

  //---------------------------------------------------------------------------
  int16 ScannerControl::controlLoop()
  {

    try
    {

      for ( ; ; )
      {
        switch ( state_ )
        {
          case IDENT:
            timeout.start( 5000 );
            getIdentification( "Ident" );
          break;

          case RESP_IDENT:
            if ( serial_com_->newMessageRcv() )
            {
              displayMessage( "Ident", display_on_ );
              timeout.start( 5000 );
              doReset( "DoReset" );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;
          

          case RESP_RESET:
            if ( serial_com_->newMessageRcv() )
            {
              displayMessage( "DoReset", display_on_ );
              timeout.start( 5000 );
              setConfig( "SetConfig" );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;

          case RESP_CONFIG:
            if ( serial_com_->newMessageRcv() )
            {
              displayMessage( "SetConfig", display_on_ );
              timeout.start( 5000 );
              setAngle( "SetStep" );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;

          case RESP_STEP:
            if ( serial_com_->newMessageRcv() )
            {
              displayMessage( "SetStep", display_on_ );
              timeout.start( 5000 );
              setFunction( "SetFunction" );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;

          case RESP_FUNCTION:
            if ( serial_com_->newMessageRcv() )
            {
              displayMessage( "SetFunction", display_on_ );
              if  ( config_count_ < SECTOR_SIZE  )
              {
                timeout.start( 5000 );
                setFunction( "SetFunction" );
              }
              else
              {
                timeout.start( 10000 );
                transeRotate( "TranseRotate" );
              }
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;

          case RESP_ROTATE:
            if ( serial_com_->newMessageRcv() )
            {
              timeout.start( 5000 );
              displayMessage( "TranseRotate", display_on_ );
              transeMeasure( "TranseMeasure" );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;

          case RESP_MEASURE:
            if ( serial_com_->newMessageRcv() )
            {
              displayMessage( "TranseMeasure", display_on_ );
              timeout.start( 5000 );
              setSignal( "SetSignal" );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;

          case RESP_SIGNAL:
            if ( serial_com_->newMessageRcv() )
            {
              displayMessage( "SetSignal", display_on_ );
              timeout.start( 5000 );
              getProfile( "GetProfile" );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;

          case RESP_PROFILE:
            if ( serial_com_->newMessageRcv() && ( ( ( actual_profile_count_ <= profile_count_ ) && ( profile_number_ == 1 ) ) || ( profile_number_ == 2 ) ) ) 
            {
              timeout.start( 5000 );
              parseMeasurementHeader();
            }
            else if ( ( actual_profile_count_ > profile_count_ ) && ( profile_number_ == 1 ) )
            {
              timeout.start( 5000 );
              exitState( "Reset" );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;

          case CONTINUOUSLY:
            if ( serial_com_->newMessageRcv() )
            {
              timeout.start( 5000 );
              getContinuouslyProfile( "Continuously" );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;

          case RESET:
            if ( serial_com_->newMessageRcv() )
            {
              displayMessage( "Reset", display_on_ );
              // set next state
              setState( EXIT );
            }
            else if ( timeout.isFinished() )
            {
              std::cout << "Timeout" << std::endl;
              return ( - 1 );
            }
          break;
          case EXIT:
            return ( -1 );
          break;

          default:
            return ( -1 );
        }
      }
    }
    catch ( ... )
    {

    }

    return ( 1 );
  }
};
