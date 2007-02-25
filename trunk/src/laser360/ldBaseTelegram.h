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
#ifndef _LD_BASE_TELEGRAM_H_
  #define _LD_BASE_TELEGRAM_H_

  #include <string>
  #include <sstream>
  #include <iostream>
  #include <vector>
  #include "ldTelegramCodes.h"
  #include "ldUserTypes.h"
  #include "ldTelegramException.h"

using namespace std;

namespace ldoem_
{
  //----------------------------------------------------------------------------
  /// base telegram for factory
  class BaseTelegram
  {
  public:

    /// source address
    static const string SID;
    /// destination address
    static const string DID;
    /// package id host to LD_OEM
    static const string SEND_PACKAGE_ID;
    /// CRC initial value
    static const uint16 INITIAL_CRC;
    /// polynomial for CRC algorithm
    static const uint16 CRC_POLYNOM;
    /// vector for telegramm items
    typedef vector < uint16 > vectorItem;

    //---------------------------------------------------------------------------
    /// constructor
    BaseTelegram( bool accept_tgm = true );

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~BaseTelegram()
    {
    };

    //---------------------------------------------------------------------------
    ///
    /// Set/Reset the accept_tgm_ Bit
    /// @param accept TRUE ... accept Telegram, FALSE ... dont accept Telegram
    void acceptTgm( bool accept );

    //---------------------------------------------------------------------------
    /// hexToString
    ///
    /// @param value to convert into a string
    /// @param wide setw
    /// @return string
    string hexToString( uint16 value, uint8 wide );

    //---------------------------------------------------------------------------
    /// stringToInt
    ///
    /// @param string to convert into integer
    uint16 stringToInt( string s );

    //---------------------------------------------------------------------------
    /// createCrc
    uint16 createCrc( string & data );

    //---------------------------------------------------------------------------
    /// createTrailer
    string createTrailer( string & data );

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Parses the message and generates a return message
    /// Must be Overwritten in derived classes
    virtual string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException ) = 0;


    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegram message items to the scanner commands
    virtual void setTgmItem( vectorItem &set_item ) = 0;

    //---------------------------------------------------------------------------
    /// getTgmItem
    ///
    /// get telegram message items
    // virtual void getTgmItem( uint16 get_item );

    //---------------------------------------------------------------------------
    /// create a string with the header of the telegram.
    static string header();

    //---------------------------------------------------------------------------
    // protected data members
    //---------------------------------------------------------------------------
  protected:

    bool accept_tgm_;

    //---------------------------------------------------------------------------
    // protected methods
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// toNetwork
    ///
    /// converts in integer or whatever to a bytestream.
    /// @param value
    /// @return Type T in a string

    template < typename T >
    string toNetwork( T value )
    {
      string value_string( sizeof( value ), '0' );

      for ( string::reverse_iterator count = value_string.rbegin(); count != value_string.rend(); ++count )
      {
        ( * count ) = static_cast < uint8 > ( value );
        value >>= 8;
      }
      return value;
    }

    //---------------------------------------------------------------------------
    /// toHost
    ///
    /// converts a byte string (Length from type of T) value into an T assumes msb first
    //
    /// @param value_string
    /// @return converted value from string

    template < typename T >
    T toHost( string value_string )
    {
      T value = 0;
      if ( sizeof( T ) > value_string.size() )
        return 0;

      for ( uint16 count = 0; count < sizeof( T ); count++ )
      {
        value *= 256;
        value |= static_cast < uint8 > ( value_string[count] );
      }
      return value;
    }
  };

}; // end namespace SickLdOem_

#endif // _LD_BASE_TELEGRAM_H_
