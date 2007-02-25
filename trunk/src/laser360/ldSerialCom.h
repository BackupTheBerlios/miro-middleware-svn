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
#ifndef _LD_SERIAL_COM_H_
  #define _LD_SERIAL_COM_H_

  #include "ldSerial.h"
  #include "ldUserTypes.h"
  #include "ldTimer.h"
  #include "ldSerialException.h"
  #include <vector>
  #include <string>


namespace ldoem_
{

  class SerialCom
  {

  public:

    /// start trailer
    static const char STX;
    /// end trailer
    static const char ETX;
    /// vector for command string items
    typedef std::vector < uint16 > vectorItem;

    //---------------------------------------------------------------------------
    /// Default Constructor.
    /// Default value for device name is '/dev/ttyUSB0'
    /// @exception serialException Is thrown if the serial port cannot be opened.
    SerialCom() throw( SerialException );

    //---------------------------------------------------------------------------
    /// destructor
    ~SerialCom();

    //---------------------------------------------------------------------------
    /// sendCommand
    ///
    /// Sends a message string to the scanner
    /// @param tgm_code identifer
    /// @param display print command string to standard output
    void sendCommand( uint16 tgm_code, bool display );

    //---------------------------------------------------------------------------
    /// setItem
    ///
    /// Set telegram message items to the scanner commands
    /// @param tgm_code identifer
    /// @param item to set in the message
    void setItem( uint16 tgm_code, vectorItem item );


    //---------------------------------------------------------------------------
    /// newMessageRcv
    ///
    /// @return true if new message received
    bool newMessageRcv();

    //---------------------------------------------------------------------------
    /// setContinuously
    ///
    /// Set receive method to continuously mode (measuremnet mode)
    void setContinuously();

    //---------------------------------------------------------------------------
    /// resetContinuously
    ///
    /// Reset continuously receveive mode (configuration mode)
    void resetContinuously();

    //---------------------------------------------------------------------------
    /// getRcvMessage
    ///
    /// @return the received message string from scanner
    std::string getRcvMessage();

  protected:

    //---------------------------------------------------------------------------
    // protected members
    //---------------------------------------------------------------------------


    static const int32 SLEEP_RCV_LOOP = 5;

    //---------------------------------------------------------------------------
    // protected methods
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// This Method is running in a own thread and is receiving messages from
    /// the serial line and checks the message for ACK or TGM.
    /// @param arg The arguments to pass thru to the thread.
    static void * receiveLoop( void * arg );

    //---------------------------------------------------------------------------
    /// Starts the thread for receiving
    void receiveStart();


  private:

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------

    /// Start Bit for start the loop for receiving
    static bool receive_run_;

    /// Is set when new message is receives
    static bool new_message_received_;

    /// Is set when measurement continuously
    static bool continuously_measurement_;

    static uint16 baudrate_;

    /// Receive string
    static std::string rcv_message_;

    /// Thread for reading from serial port
    pthread_t thread_;

    /// The Serial object that will be used by the stream to communicate with the serial port.
    static Serial serial_port_;

    /// The Serial object parameters that will be used by the stream to communicate with the serial port.
    static Serial::Parameters param_;

  };

}; // end of namespace ldoem_

#endif
