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
#include "ldSerialCom.h"
#include "ldFactory.h"
#include "ldScannerTelegram.h"


namespace ldoem_
{
  //---------------------------------------------------------------------------
  // initializing of static member variables
  const char SerialCom::STX = 0x02;
  const char SerialCom::ETX = 0x03;
  bool SerialCom::receive_run_ = false;
  bool SerialCom::new_message_received_ = false;
  bool SerialCom::continuously_measurement_ = false;
  uint16 SerialCom::baudrate_ = 6;
  Serial SerialCom::serial_port_;
  Serial::Parameters SerialCom::param_;
  string SerialCom::rcv_message_ = "";

  //---------------------------------------------------------------------------
  SerialCom::SerialCom() throw( SerialException )
  {
    // open the serial default port
    if ( serial_port_.openPort() )
    {
      receiveStart();
    }
    else
    {
      stringstream message_stream;
      message_stream << "SerialCom::SerialCom : Could not open serial port !!!";
      throw SerialException( message_stream.str() );
    }
  }

  //---------------------------------------------------------------------------
  SerialCom::~SerialCom()
  {

  }

  //---------------------------------------------------------------------------
  void SerialCom::sendCommand( uint16 tgm_code, bool display )
  {
    string message = "";

    // get command string from factory
    message = TgmFactory::getTGM( tgm_code )->execute();
    // send sting to serial port
    serial_port_.send( message );

    if ( display )
      std::cout << message << std::endl;
  }

  //---------------------------------------------------------------------------
  void SerialCom::setItem( uint16 tgm_code, vectorItem item )
  {
    TgmFactory::getTGM( tgm_code )->setTgmItem( item );
  }

  //---------------------------------------------------------------------------
  bool SerialCom::newMessageRcv()
  {
    if ( new_message_received_ )
    {
      new_message_received_ = false;
      return true;
    }
    else
    {
      return false;
    }
  }

  //---------------------------------------------------------------------------
  void SerialCom::setContinuously()
  {
    continuously_measurement_ = true;
  }

  //---------------------------------------------------------------------------
  void SerialCom::resetContinuously()
  {
    continuously_measurement_ = false;
  }

  //---------------------------------------------------------------------------
  string SerialCom::getRcvMessage()
  {
    return rcv_message_;
  }

  //---------------------------------------------------------------------------
  void * SerialCom::receiveLoop( void * arg )
  {
    Timer timeout;
    char rcv;
    string message = "";

    while ( receive_run_ )
    {
      if ( !serial_port_.receive( rcv ) )
      {
        timeout.wait( SLEEP_RCV_LOOP );
        continue;
      }
      timeout.wait( 10 );

      if ( ! continuously_measurement_ )
      {
        while ( serial_port_.receive( rcv ) )
        {
          if ( rcv != STX && rcv != ETX )
          {
            message += rcv;
          }
          else if ( rcv == ETX )
          {
            continue;
          }
        }
        rcv_message_ = message;
        message = "";
        new_message_received_ = true;
      }
      else
      {
        while ( serial_port_.receive( rcv ) )
        {
          if ( rcv != STX && rcv != ETX )
          {
            message += rcv;
          }
          else if ( rcv == ETX  )
          {
            rcv_message_ = message;
            new_message_received_ = true;
            message = "";
          }
        }
      }
    }
    return 0;
  }

  //--------------------------------------------------------------------------
  void SerialCom::receiveStart()
  {
    int32 retValue;
    receive_run_ = true;
    retValue = pthread_create( & thread_, NULL, receiveLoop, NULL );
    return;
  }
};
