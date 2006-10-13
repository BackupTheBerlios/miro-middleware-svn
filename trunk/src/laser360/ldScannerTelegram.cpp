
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldScannerTelegram.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#include "ldScannerTelegram.h"


namespace ldoem_
{

  //---------------------------------------------------------------------------
  string GetIndification::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( GET_IDENTIFICATION, 4 );
    stream << hexToString( ident_item_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();

    command = createTrailer( message );

    return command;
  }

  //---------------------------------------------------------------------------
  string GetStatus::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( GET_STATUS, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string GetSignal::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( GET_SIGNAL, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string SetSignal::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( SET_SIGNAL, 4 );
    stream << hexToString( port_value_, 4 );
    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string GetConfig::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( GET_CONFIG, 4 );
    stream << hexToString( config_item_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string SetTimeAbsolute::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( SET_TIME_ABS, 4 );
    stream << hexToString( time_stamp_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string SetTimeRelative::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( SET_TIME_REL, 4 );
    stream << hexToString( time_diff_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string GetSyncClock::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( GET_SYNC_CLOCK, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string SetConfig::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( SET_CONFIG, 4 );
    stream << hexToString( config_item_, 4 );
    stream << hexToString( baudrate_, 4 );
    stream << hexToString( parity_, 4 );
    stream << hexToString( stop_bit_, 4 );
    stream << hexToString( character_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string SetFunction::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( SET_FUNCTION, 4 );
    stream << hexToString( sector_number_, 4 );
    stream << hexToString( sector_function_, 4 );
    stream << hexToString( sector_stop_, 4 );
    stream << hexToString( flashflag_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string GetFunction::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( GET_FUNCTION, 4 );
    stream << hexToString( sector_number_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }

  //---------------------------------------------------------------------------
  string GetProfile::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( GET_PROFILE, 4 );
    stream << hexToString( profile_number_, 4 );
    stream << hexToString( profile_format_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }

  //---------------------------------------------------------------------------
  string CancelProfile::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( CANCEL_PROFILE, 4 );
    stream << hexToString( item, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string DoReset::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( DO_RESET, 4 );
    stream << hexToString( reset_level_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string TranseIdle::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( TRANSE_IDLE, 4 );
    stream << hexToString( item, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }

  //---------------------------------------------------------------------------
  string TranseRotate::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( TRANSE_ROTATE, 4 );
    stream << hexToString( frequency_, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }


  //---------------------------------------------------------------------------
  string TranseMeasure::execute( string message, uint16 item ) throw( TelegramException )
  {
    message = "";

    string command = "";
    stringstream stream;
    uint16 crc = item;

    // parse command
    stream << header();
    stream << hexToString( message_len_, 4 );
    stream << BaseTelegram::SEND_PACKAGE_ID;
    stream << hexToString( TRANSE_MEASURE, 4 );
    stream << hexToString( item, 4 );

    message = stream.str();
    // calculate crc
    crc = 0;
    crc = createCrc( message );
    stream << hexToString( crc, 4 );

    message = "";
    message = stream.str();
    // set start and end trailer for command
    command = createTrailer( message );

    return command;
  }

}; // end namespace
