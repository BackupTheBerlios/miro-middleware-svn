
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldBaseTelegram.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#include <iomanip>
#include "ldBaseTelegram.h"
#include "ldCrcTable.h"

using namespace std;

namespace ldoem_
{
  //---------------------------------------------------------------------------
  //  initializing of static member variables
  const string BaseTelegram::SID = "00";
  const string BaseTelegram::DID = "10";
  const string BaseTelegram::SEND_PACKAGE_ID = "0000";
  const uint16 BaseTelegram::INITIAL_CRC = 0xffff;
  const uint16 BaseTelegram::CRC_POLYNOM = 0x1021;

  //---------------------------------------------------------------------------
  BaseTelegram::BaseTelegram( bool accept_tgm )
  {
    accept_tgm_ = accept_tgm;
  }

  //---------------------------------------------------------------------------
  void BaseTelegram::acceptTgm( bool accept )
  {
    accept_tgm_ = accept;
  }

  //---------------------------------------------------------------------------
  string BaseTelegram::hexToString( uint16 value, uint8 wide )
  {
    string temp;
    stringstream stream;
    stream << hex << setw( wide ) << setfill( '0' ) << value << ends; temp = stream.str();
    temp.erase( temp.end() - 1 );

    return temp;
  }

  //---------------------------------------------------------------------------
  uint16 BaseTelegram::stringToInt( string s )
  {
    uint16 i;

    std::istringstream istr( s ); // fill with ASCII-character
    istr >> i;
    return ( i );
  }

  //---------------------------------------------------------------------------
  uint16 BaseTelegram::createCrc( string & data )
  {
    uint16 crc = INITIAL_CRC;
    uint16 value = 0;
    uint16 i = 0;
    uint16 numofbytes = data.size() / sizeof( uint32 );

    while ( numofbytes-- )
    {
      value = 0;

      if ( ( data[i] >= '0' ) && ( data[i] <= '9' ) )
        value = ( ( data[i] - 48 ) << 12 );
      else
      {
        if ( ( data[i] == 'a' ) || ( data[i] == 'A' ) )
          value = ( 10 << 12 );
        if ( ( data[i] == 'b' ) || ( data[i] == 'B' ) )
          value = ( 11 << 12 );
        if ( ( data[i] == 'c' ) || ( data[i] == 'C' ) )
          value = ( 12 << 12 );
        if ( ( data[i] == 'd' ) || ( data[i] == 'D' ) )
          value = ( 13 << 12 );
        if ( ( data[i] == 'e' ) || ( data[i] == 'E' ) )
          value = ( 14 << 12 );
        if ( ( data[i] == 'f' ) || ( data[i] == 'F' ) )
          value = ( 15 << 12 );
      }

      if ( ( data[i + 1] >= '0' ) && ( data[i + 1] <= '9' ) )
        value = value | ( ( data[i + 1] - 48 ) << 8 );
      else
      {
        if ( ( data[i + 1] == 'a' ) || ( data[i + 1] == 'A' ) )
          value = value | ( 10 << 8 );
        if ( ( data[i + 1] == 'b' ) || ( data[i + 1] == 'B' ) )
          value = value | ( 11 << 8 );
        if ( ( data[i + 1] == 'c' ) || ( data[i + 1] == 'C' ) )
          value = value | ( 12 << 8 );
        if ( ( data[i + 1] == 'd' ) || ( data[i + 1] == 'D' ) )
          value = value | ( 13 << 8 );
        if ( ( data[i + 1] == 'e' ) || ( data[i + 1] == 'E' ) )
          value = value | ( 14 << 8 );
        if ( ( data[i + 1] == 'f' ) || ( data[i + 1] == 'F' ) )
          value = value | ( 15 << 8 );
      }

      if ( ( data[i + 2] >= '0' ) && ( data[i + 2] <= '9' ) )
        value = value | ( ( data[i + 2] - 48 ) << 4 );
      else
      {
        if ( ( data[i + 2] == 'a' ) || ( data[i + 2] == 'A' ) )
          value = value | ( 10 << 4 );
        if ( ( data[i + 2] == 'b' ) || ( data[i + 2] == 'B' ) )
          value = value | ( 11 << 4 );
        if ( ( data[i + 2] == 'c' ) || ( data[i + 2] == 'C' ) )
          value = value | ( 12 << 4 );
        if ( ( data[i + 2] == 'd' ) || ( data[i + 2] == 'D' ) )
          value = value | ( 13 << 4 );
        if ( ( data[i + 2] == 'e' ) || ( data[i + 2] == 'E' ) )
          value = value | ( 14 << 4 );
        if ( ( data[i + 2] == 'f' ) || ( data[i + 2] == 'F' ) )
          value = value | ( 15 << 4 );

      }

      if ( ( data[i + 3] >= '0' ) && ( data[i + 3] <= '9' ) )
        value = value | ( ( data[i + 3] - 48 ) );
      else
      {
        if ( ( data[i + 3] == 'a' ) || ( data[i + 3] == 'A' ) )
          value = value | ( 10 );
        if ( ( data[i + 3] == 'b' ) || ( data[i + 3] == 'B' ) )
          value = value | ( 11 );
        if ( ( data[i + 3] == 'c' ) || ( data[i + 3] == 'C' ) )
          value = value | ( 12 );
        if ( ( data[i + 3] == 'd' ) || ( data[i + 3] == 'D' ) )
          value = value | ( 13 );
        if ( ( data[i + 3] == 'e' ) || ( data[i + 3] == 'E' ) )
          value = value | ( 14 );
        if ( ( data[i + 3] == 'f' ) || ( data[i + 3] == 'F' ) )
          value = value | ( 15 );
      }

      crc = ( ( crc << 8 ) | ( ( static_cast < uint8 > ( value >> 8 ) ) ) ) ^ crctab[crc >> 8];
      crc = ( ( crc << 8 ) | ( static_cast < uint8 > ( value ) ) ) ^ crctab[crc >> 8];
      i += 4;
    }
    return crc;
  }

  //---------------------------------------------------------------------------
  string BaseTelegram::createTrailer( string & data )
  {
    string message = "";
    stringstream stream;

    stream << STX;
    stream << data;
    stream << ETX;

    return stream.str();

  }

  //---------------------------------------------------------------------------
  string BaseTelegram::header()
  {
    stringstream stream;

    stream << SID << DID;
    return stream.str();
  }

}; // end namespace ldoem_
