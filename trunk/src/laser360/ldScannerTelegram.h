
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldScannerTelegram.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef  _LD_SCANNER_TELEGRAM_H_
  #define _LD_SCANNER_TELEGRAM_H_

  #include "ldBaseTelegram.h"
  #include "ldFactory.h"
  #include "ldTelegramCodes.h"

using namespace std;

namespace ldoem_
{

  //---------------------------------------------------------------------------
  ///
  /// GetIndification
  ///
  /// Information about the LD type, firmware and application version
  class GetIndification : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    GetIndification( bool accept = true ) : BaseTelegram( accept )
    {
      ident_item_ = 0x0001; // name of the sensor
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~GetIndification()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @param item
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 identitem = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      ident_item_ = ident_item[0];
    }

    //---------------------------------------------------------------------------
    // public data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// ident_item
    ///
    /// 0x0000 part naumber of the sensor (LD)
    /// 0x0001 name of the sensor (LD)
    /// 0x0002 version of the sensor (LD)
    /// 0x0003 serial number of th LD unit
    /// 0x0004 serial number of the EDM unit
    /// 0x0010 part number of the firmware
    /// 0x0011 name of the firmware
    /// 0x0012 version of the firmware
    /// 0x0020 part number of the apllication software
    /// 0x0021 name of the application software
    /// 0x0022 version of the apllication software
    uint16 ident_item_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;

  };



  //---------------------------------------------------------------------------
  ///
  /// GetStatus
  ///
  /// Status query
  class GetStatus : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    GetStatus( bool accept = true ) : BaseTelegram( accept )
    {
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~GetStatus()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      vectorItem initial = ident_item;
    }

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0003;
  };



  //---------------------------------------------------------------------------
  ///
  /// GetSignal
  ///
  /// Reads the value of the switch and LED port
  class GetSignal : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    GetSignal( bool accept = true ) : BaseTelegram( accept )
    {
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~GetSignal()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      vectorItem initial = ident_item;
    }

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0003;
  };



  //---------------------------------------------------------------------------
  ///
  /// SetSignal
  ///
  /// Sets the switches and LEDs
  class SetSignal : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    SetSignal( bool accept = true ) : BaseTelegram( accept )
    {
      port_value_ = 0x0001; // LED 0 (yellow)
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~SetSignal()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      port_value_ = ident_item[0];
    }

    //---------------------------------------------------------------------------
    // public data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// port_value_
    ///
    /// lower byte : port bits
    /// upper byte : do not care
    ///
    /// definiation of the 8 port bits
    /// Bit 0 LED 0 (yellow)
    /// Bit 1 LED 1 (yellow)
    /// Bit 2 LED 2 (green)
    /// Bit 3 LED 3 (red)
    /// Bit 4 Switch 0
    /// Bit 5 Switch 1
    /// Bit 6 Switch 2
    /// Bit 7 Switch 3
    uint16 port_value_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;
  };



  //---------------------------------------------------------------------------
  ///
  /// SetConfig
  ///
  /// Sets the serial interface configuration
  class SetConfig : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    SetConfig( bool accept = true ) : BaseTelegram( accept )
    {
      config_item_ = 0x0001; // rs232/rs422
      baudrate_ = 0x0006; // 115200 bd
      parity_ = 0x0000; // non parity
      stop_bit_ = 0x0001; // one stop bit
      character_ = 0x0008; // 8 bits per character
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~SetConfig()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      config_item_ = ident_item[0];
      baudrate_ = ident_item[1];
      parity_ = ident_item[2];
      stop_bit_ = ident_item[3];
      character_ = ident_item[4];
    }

    //---------------------------------------------------------------------------
    // public data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// config_item_
    ///
    /// 0x0000 historical arcnet configuration key
    /// 0x0001 rs232/rs422 configuration key
    /// 0x0002 can configuration key
    /// 0x0003 reserved
    /// 0x0004 arcnet configuration key
    /// 0x0010 global configuration key
    uint16 config_item_;

    //---------------------------------------------------------------------------
    /// baudrate_
    ///
    /// 0x0001   4800 bd
    /// 0x0002   9600 bd
    /// 0x0003  19200 bd
    /// 0x0004  38400 bd
    /// 0x0005  57600 bd
    /// 0x0006 115200 bd
    uint16 baudrate_;

    //---------------------------------------------------------------------------
    /// parity_
    ///
    /// 0x0000 no parity
    /// 0x0001 even parity
    /// 0x0002 odd parity
    uint16 parity_;

    //---------------------------------------------------------------------------
    /// stop_bit_
    ///
    /// 0x0001 one stop bit
    /// 0x0002 two stop bit
    uint16 stop_bit_;

    //---------------------------------------------------------------------------
    /// character_
    ///
    /// number of bits per character (1..8 bit)
    uint16 character_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0008;

  };



  //---------------------------------------------------------------------------
  ///
  /// GetConfig
  ///
  /// Reads the configuration settings
  class GetConfig : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    GetConfig( bool accept = true ) : BaseTelegram( accept )
    {
      config_item_ = 0x0001; // rs232/rs422
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~GetConfig()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      vectorItem initial = ident_item;
    }

    //---------------------------------------------------------------------------
    // prublic data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// config_item_
    ///
    /// 0x0000 historical arcnet configuration key
    /// 0x0001 rs232/rs422 configuration key
    /// 0x0002 can configuration key
    /// 0x0003 reserved
    /// 0x0004 arcnet configuration key
    /// 0x0010 global configuration key
    uint16 config_item_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0006;

  };



  //---------------------------------------------------------------------------
  ///
  /// SetTimeAbsolute
  ///
  /// Sets the internal clock to the time stamp value
  class SetTimeAbsolute : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    SetTimeAbsolute( bool accept = true ) : BaseTelegram( accept )
    {
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~SetTimeAbsolute()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      time_stamp_ = ident_item[1];
    }

    //---------------------------------------------------------------------------
    // prublic data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// time_stamp_
    ///
    /// New value of the internal clock [ms]
    uint16 time_stamp_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;

  };



  //---------------------------------------------------------------------------
  ///
  /// SetTimeRelative
  ///
  /// Corrects the internal clock by a defined value
  class SetTimeRelative : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    SetTimeRelative( bool accept = true ) : BaseTelegram( accept )
    {
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~SetTimeRelative()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      time_diff_ = ident_item[0];
    }

    //---------------------------------------------------------------------------
    // prublic data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// time_diff_
    ///
    /// Offset value [ms], which corrects the internal clock
    uint16 time_diff_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;

  };



  //---------------------------------------------------------------------------
  ///
  /// GetSyncClock
  ///
  /// Reads the internal time of LD
  class GetSyncClock : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    GetSyncClock( bool accept = true ) : BaseTelegram( accept )
    {
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~GetSyncClock()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      vectorItem initial = ident_item;
    }

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0003;

  };



  //---------------------------------------------------------------------------
  ///
  /// SetFunction
  ///
  /// Assigns a measurement function to an angle range
  class SetFunction : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    SetFunction( bool accept = true ) : BaseTelegram( accept )
    {
      sector_number_ = 0x0001; // number of the sector
      sector_function_ = 0x0003; // normal measurement
      sector_stop_ = 0x0000; // end angel
      flashflag_ = 0x0000; // configuration stays temporary
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~SetFunction()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      sector_number_ = ident_item[0];
      sector_function_ = ident_item[1];
      sector_stop_ = ident_item[2];
      flashflag_ = ident_item[3];
    }

    //---------------------------------------------------------------------------
    // public data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// sector_number_
    ///
    /// number of the measurement sector.
    /// valid values: 0..7.
    uint16 sector_number_;

    //---------------------------------------------------------------------------
    /// sector_function_
    ///
    /// measurement function for the sector.
    /// 0: no initialised
    /// 1: no measurement
    /// 2: reserved
    /// 3: normal measurement
    uint16 sector_function_;

    //---------------------------------------------------------------------------
    /// sector_stop_
    ///
    /// end angle of the sector.
    /// NOTE: This Angle is given in 1/16 degrees. It must be an
    /// integer multiple of the angle step, i.e. of the angle between
    /// two laser pulses otherwise this sector is unreachable for the measurement kernel.
    uint16 sector_stop_;

    //---------------------------------------------------------------------------
    /// flashflag_
    ///
    /// number of the measurement sector.
    /// 1: The sector configuration is written to flash memory
    /// Else: The sector configuration stays temporary and is lost after a reset.
    uint16 flashflag_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0007;

  };



  //---------------------------------------------------------------------------
  ///
  /// GetFunction
  ///
  /// Returns the configuration of the declared setcor
  class GetFunction : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    GetFunction( bool accept = true ) : BaseTelegram( accept )
    {
      sector_number_ = 0x0001;
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~GetFunction()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      vectorItem initial = ident_item;
    }

    //---------------------------------------------------------------------------
    // public data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// sector_number_
    ///
    /// number of the measurement sector.
    /// valid values: 0..7.
    uint16 sector_number_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;

  };



  //---------------------------------------------------------------------------
  ///
  /// GetProfile
  ///
  /// Requests n profiles of a defined format
  class GetProfile : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    GetProfile( bool accept = true ) : BaseTelegram( accept )
    {
      profile_number_ = 0x0001; // profil number
      profile_format_ = 0x0080; // profil format
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~GetProfile()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      profile_number_ = ident_item[0];
      profile_format_ = ident_item[1];
    }

    //---------------------------------------------------------------------------
    // public data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// profile_number_
    ///
    /// number of profiles, if it is equals 0 the LD sends profiles PROFILENUM WORD
    /// continuously, until the user sends the CANCEL_PROFILE command
    uint16 profile_number_;

    //---------------------------------------------------------------------------
    /// profile_number_
    ///
    /// 16 bit-array
    uint16 profile_format_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0005;

  };



  //---------------------------------------------------------------------------
  ///
  /// CancelProfile
  ///
  /// Stops the profile output
  class CancelProfile : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    CancelProfile( bool accept = true ) : BaseTelegram( accept )
    {
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~CancelProfile()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      vectorItem initial = ident_item;
    }

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;

  };



  //---------------------------------------------------------------------------
  ///
  /// DoReset
  ///
  /// The LD enters a reset sequence
  class DoReset : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    DoReset( bool accept = true ) : BaseTelegram( accept )
    {
      reset_level_ = 0x0002; // halt application and enter idle state
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~DoReset()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      reset_level_ = ident_item[0];
    }

    //---------------------------------------------------------------------------
    // public data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// reset_level_
    ///
    /// 0x0000 reset (CPU reinitialized)
    /// 0x0001 restart (CPU not reinitialized)
    /// 0x0002 halt application and enter idle state
    /// 0x0010 reload APD voltage setting
    /// others reserved
    uint16 reset_level_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;
  };



  //---------------------------------------------------------------------------
  ///
  /// TranseIdle
  ///
  /// Sets the LD into the IDLE mode:
  /// the motor of the rotating prism stops and the laser is switched off
  class TranseIdle : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    TranseIdle( bool accept = true ) : BaseTelegram( accept )
    {
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~TranseIdle()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      vectorItem initial = ident_item;
    }

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;
  };



  //---------------------------------------------------------------------------
  ///
  /// TranseRotate
  ///
  /// Sets the LD into the ROTATE mode: the motor starts, when it is off, and
  /// rotates with a speed, defined by freqeuncy_. The laser is switched off.
  class TranseRotate : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    TranseRotate( bool accept = true ) : BaseTelegram( accept )
    {
      frequency_ = 0x0005; // Scanning frequency corresponds to the configuration parameter
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~TranseRotate()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );


    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      frequency_ = ident_item[0];
    }


    //---------------------------------------------------------------------------
    // public data members
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// frequency_
    ///
    /// 0x0000 Scanning frequency corresponds to the configuration parameter
    /// 0x0001 reserved
    /// 0x0002 reserved
    /// 0x0003 reserved
    /// 0x0004 reserved
    /// 0x0005 - 0x0014 Scanning frequency in Hz
    uint16 frequency_;

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;
  };



  //---------------------------------------------------------------------------
  ///
  /// TranseMeasure
  ///
  /// Sets the LD into the MEASURE mode: the laser starts with the next revolution;
  /// a request for a profile can be started
  class TranseMeasure : public BaseTelegram
  {
  public:

    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param accept Accept telegrams by default ( default = true )
    TranseMeasure( bool accept = true ) : BaseTelegram( accept )
    {
    }

    //---------------------------------------------------------------------------
    /// virtual destructor
    virtual ~TranseMeasure()
    {
    };

    //---------------------------------------------------------------------------
    /// execute
    ///
    /// Executes the Telegram spezific operation
    /// @param message Message from Network or empty string if only generate message to server.
    /// @return generated Message string (Send)
    /// @exception is thrown if an error occurs during parsing.
    string execute( string message = "", uint16 item = 0x0000 ) throw( TelegramException );

    //---------------------------------------------------------------------------
    /// setTgmItem
    ///
    /// set telegramm message items for scanner commands
    void setTgmItem( vectorItem & ident_item )
    {
      vectorItem initial = ident_item;
    }

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// message length
    static const uint16 message_len_ = 0x0004;
  };



}; // end namespace ldoem_

#endif // _LD_SCANNER_TELEGRAM_H_
