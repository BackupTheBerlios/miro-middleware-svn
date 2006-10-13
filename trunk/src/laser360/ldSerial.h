
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldSerial.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LD_SERIAL_H_
  #define _LD_SERIAL_H_

  #include "ldObject.h"
  #include "ldInOutObject.h"
  #include "ldInOutHandler.h"
  #include "ldUserTypes.h"

  #include <unistd.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <termios.h>
  #include <cstdio>
  #include <cstdlib>
  #include <string>


namespace Laser360
{
  class Parameters;
};

namespace ldoem_
{
  //---------------------------------------------------------------------------
  /// This Serial class provides an unified interface to access Serial
  /// ports in posix operating systems.
  ///
  /// You just need to set the parameters if you do not want the default ones,
  /// and send or receive data with the send() and receive() methods.
  ///
  /// If you use a Linux system, do not forget to make the users who
  /// needs access to the Serial ports to be members of the group
  /// <code>uucp</code> or the corresponding group of the
  /// <code>/dev/ttyS*</code> devices.
  class Serial : public Object
  {
  public:
    //---------------------------------------------------------------------------
    /// the parameters for the class serial
    class Parameters : public InOutObject
    {
    public:
      //---------------------------------------------------------------------------
      /// default constructor
      Parameters();

      //---------------------------------------------------------------------------
      /// copy constructor
      /// @param other the parameters object to be copied
      Parameters( const Parameters & other );

      //---------------------------------------------------------------------------
      /// destructor
      virtual ~Parameters();

      //---------------------------------------------------------------------------
      /// returns name of this type
      const char * getTypeName() const;

      //---------------------------------------------------------------------------
      /// copy the contents of a parameters object
      /// @param other the parameters object to be copied
      /// @return a reference to this parameters object
      Parameters & copy( const Parameters & other );

      //---------------------------------------------------------------------------
      /// copy the contents of a parameters object
      /// @param other the parameters object to be copied
      /// @return a reference to this parameters object
      Parameters & operator = ( const Parameters & other );

      //---------------------------------------------------------------------------
      /// returns a pointer to a clone of the parameters
      virtual Parameters * clone() const;

      //---------------------------------------------------------------------------
      /// write the parameters in the given ioHandler
      /// @param handler the ioHandler to be used
      /// @param complete if true (the default) the enclosing begin/end will
      /// be also written, otherwise only the data block will be written.
      /// @return true if write was successful
      virtual bool write( InOutHandler & handler, const bool complete = true ) const;

      //---------------------------------------------------------------------------
      /// read the parameters in the given ioHandler
      /// @param handler the ioHandler to be used
      /// @param complete if true (the default) the enclosing begin/end will
      /// be also written, otherwise only the data block will be written.
      /// @return true if write was successful
      virtual bool read( InOutHandler & handler, const bool complete = true );

      //---------------------------------------------------------------------------
      /// setBaudrate
      void setBaudrate( uint16 baudrate );

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      //---------------------------------------------------------------------------
      /// Type to specificy the port to be used
      enum ePortType
      {
        Com1 = 0,   // /dev/ttyS0 in Linux
        Com2,       // /dev/ttyS1 in Linux
        Com3,       // /dev/ttyS2 in Linux
        Com4,       // /dev/ttyS3 in Linux
        Com5,       // /dev/ttyS4 in Linux
        Com6,       // /dev/ttyS5 in Linux
        Com7,       // /dev/ttyS6 in Linux
        Com8        // /dev/ttyS7 in Linux
      };

      //---------------------------------------------------------------------------
      ///  Type to specify the baud rate.
      ///
      /// Baud rate is a measure of the number of times per second a signal in a communications channel varies,
      /// or makes a transition between states (states being frequencies, voltagelevels, or phase angles).
      /// One baud is one such change. Thus, a 1200-baud modem's signal changes state 1200 times eachb second.
      /// The number of bits transmitted in a second (baudrate in bits-per-second (bps) )depends on the modulation
      /// technique being used.  A 2400 baud modem can transmit 9400 bps if there are 4 bits/baud.
      enum eBaudRateType
      {
        Baud0,     //       0 baud
        Baud300,   //     300 baud
        Baud600,   //     600 baud
        Baud1200,  //    1200 baud
        Baud1800,  //    1800 baud
        Baud2400,  //    2400 baud
        Baud4800,  //    4800 baud
        Baud9600,  //    9600 baud
        Baud19200, //   19200 baud
        Baud38400, //   38400 baud
        Baud57600, //   57600 baud
        Baud76800, //   76800 baud
        Baud115200 //  115200 baud
      };

      //---------------------------------------------------------------------------
      /// Type to specify the number of bits of a character. The first element Cs4 means for example 4 bits/character.
      enum eCharBitSizeType
      {
        Cs4 = 4, //  4 bits per character
        Cs5,     //  5 bits per character
        Cs6,     //  6 bits per character
        Cs7,     //  7 bits per character
        Cs8      //  8 bits per character
      };

      //---------------------------------------------------------------------------
      /// definiton of the stop bit This is also called the remaining bits.  Stop bits serve to
      /// synchronise the receiving computer to the incoming characters.
      enum eStopBitType
      {
        One,     // one stop bit
        OneFive, // one and a half stop bits
        Two      // two stop bits
      };

      //---------------------------------------------------------------------------
      /// definition of the parity bit the optional parity bit is a simple checksum of the data bits
      /// indicating whether or not the data contains an even or odd number of "1" bits.
      enum eParityType
      {
        No,     // no parity
        Even,   // even parity
        Odd,    // odd parity
        Space,  // space
        Mark    // mark
      };

      //---------------------------------------------------------------------------
      /// Baud rate. Default value: Baud115200
      eBaudRateType baudRate_;

      //---------------------------------------------------------------------------
      /// Which Serial port. Default value: Com1
      ePortType port_;

      //---------------------------------------------------------------------------
      /// Character size. Default value: Cs8
      eCharBitSizeType characterSize_;

      //---------------------------------------------------------------------------
      /// Parity. Default value: No
      eParityType parity_;

      //---------------------------------------------------------------------------
      /// Stop bits Default: One
      eStopBitType stopBits_;

      //---------------------------------------------------------------------------
      /// Receive timeout in milliseconds.
      ///
      /// If you specify zero, then no timeout will exist.  The receive method
      /// will check if there is something in the buffer.  If so, then it
      /// will normally return (reading the first character in the serial FIFO).
      /// If there is nothing in the buffer, the receive() method will return inmediatly a "false" value.
      ///
      /// You should consider that for Linux systems the expected timeout
      /// precision is in deciseconds.  The given value will be rounded
      /// to the best decisecond precision.  For example 1 ms will be rounded to 0 ms, 80ms will be rounded to 0.1s.
      ///
      /// Default value: 50 (i.e. 5 ms)
      int receiveTimeout_;


    protected:

      //---------------------------------------------------------------------------
      // protected members
      //---------------------------------------------------------------------------

      //---------------------------------------------------------------------------
      /// xml parameter handle
      const ::Laser360::Parameters & parameters_;

    };

    //---------------------------------------------------------------------------
    /// default constructor
    Serial();

    //---------------------------------------------------------------------------
    /// construct a serial instance with the given parameters
    Serial( const Parameters & SerialPar );

    //---------------------------------------------------------------------------
    /// copy constructor
    /// @param other the object to be copied
    Serial( const Serial & other );



    //---------------------------------------------------------------------------
    /// destructor
    virtual ~Serial();

    //---------------------------------------------------------------------------
    /// returns the name of this type ("Serial")
    virtual const char * getTypeName() const;

    //---------------------------------------------------------------------------
    /// copy data of "other" functor.
    /// @param other the functor to be copied
    /// @return a reference to this functor object
    Serial & copy( const Serial & other );

    //---------------------------------------------------------------------------
    /// returns used parameters
    const Parameters & getParameters() const;

    //---------------------------------------------------------------------------
    /// set the parameters for the Serial object
    bool setParameters( const Parameters & theParameters );

    //---------------------------------------------------------------------------
    /// open the port using the default parameters
    bool openPort();

    //---------------------------------------------------------------------------
    /// sends a character c through the port specified in the parameters.
    /// @return true if the character could be send successfully, or false otherwise.
    bool send( const char c );

    //---------------------------------------------------------------------------
    /// sends an unsigned character c through the port specified in the parameters.
    /// @return true if the character could be send successfully, or false otherwise.
    bool send( const unsigned char c );

    //---------------------------------------------------------------------------
    /// sends a string through the port specified in the parameters.
    /// @return true if all the string characters are send successfully, or false otherwise.
    bool send( const std::string & theString );

    //---------------------------------------------------------------------------
    /// wait for a character to be received in the port specified in the parameters.
    /// @return true if the character has been received, or false if a
    /// problem occured.  In this case you can read the status string
    /// to check the source of the problem.  Note that false will also be returned if a timeout occurs.
    bool receive( char & c );

    //---------------------------------------------------------------------------
    /// wait for an unsigned character to be received in the port specified in the parameters.
    /// @return true if the character has been received, or false if a
    /// problem occured.  In this case you can read the status string
    /// to check the source of the problem.  Note that false will also be returned if a timeout occurs.
    bool receive( unsigned char & c );

    //---------------------------------------------------------------------------
    /// read all characters until the next new line or zero character, and store them in the given string.
    bool receive( std::string & theString );

    //---------------------------------------------------------------------------
    /// return the last message set with setStatusString().  This will
    /// never return 0.  If no status-string has been set yet an empty string
    /// (pointer to a string with only the char(0)) will be returned.
    const char * getStatusString() const;

    //---------------------------------------------------------------------------
    /// set a status string.
    /// @param msg the const string to be reported next time by getStatusString().
    /// This message will be usually set within the send/receive methods to indicate an error cause.
    /// Note that the change of the status string is not considered as
    /// a change in the class status (this method is const).
    void setStatusString( const char * msg ) const;

    //---------------------------------------------------------------------------
    /// round any float type and return an integer

    template < class T >
    inline int iround( T x )
    {
      if ( x >= 0 )
      {
        return static_cast < int > ( x + 0.5 );
      }
      return static_cast < int > ( x - 0.5 );
    }


  private:

    //---------------------------------------------------------------------------
    /// file descritpor for the port
    /// get the value -1 in case of error, while opening the port
    int descriptor_;

    //---------------------------------------------------------------------------
    /// contains true if the port is opened, else false
    bool isPortOpened_;

    //---------------------------------------------------------------------------
    /// close the port
    void closePort();

    //---------------------------------------------------------------------------
    /// the empty string returned if the statusString is empty
    static const char * const emptyString_ ;

    //---------------------------------------------------------------------------
    /// the status string written with setStatusString
    mutable char * statusString_;

  protected:

    //---------------------------------------------------------------------------
    /// the parameters in use
    Parameters param_;

    //---------------------------------------------------------------------------
    /// xml parameter handle
    const ::Laser360::Parameters & parameters_;

  };
}
#endif //_LD_SERIAL_H_
