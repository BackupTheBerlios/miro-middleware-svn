
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldSerial.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#include "ldSerial.h"
#include "Parameters.h"

#include <string>
#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

namespace ldoem_
{

  //---------------------------------------------------------------------------
  // default empty string for the Serial class
  const char   * const Serial::emptyString_ = "";

  // --------------------------------------------------
  // Serial::Parameters
  // --------------------------------------------------

  //---------------------------------------------------------------------------
  // default constructor
  Serial::Parameters::Parameters() : InOutObject(), parameters_( *::Laser360::Parameters::instance() )
  {
    setBaudrate( parameters_.baudrate );
    //baudRate_ = Baud57600;
    port_ = Com1;
    characterSize_ = Cs8;
    parity_ = No;
    stopBits_ = One;
    receiveTimeout_ = 50; // 50 ms
  }

  //---------------------------------------------------------------------------
  // copy constructor
  Serial::Parameters::Parameters( const Parameters & other ) : InOutObject(),
     parameters_( *::Laser360::Parameters::instance() )
     {
       copy( other );
  }

  //---------------------------------------------------------------------------
  // destructor
  Serial::Parameters::~Parameters()
  {
  }

  //---------------------------------------------------------------------------
  // get type name
  const char * Serial::Parameters::getTypeName() const
  {
    return "Serial::Parameters";
  }

  //---------------------------------------------------------------------------
  // copy member
  Serial::Parameters & Serial::Parameters::copy( const Parameters & other )
  {
    baudRate_ = other.baudRate_;
    port_ = other.port_;
    characterSize_ = other.characterSize_;
    parity_ = other.parity_;
    stopBits_ = other.stopBits_;
    receiveTimeout_ = other.receiveTimeout_;
    return * this;
  }

  //---------------------------------------------------------------------------
  // alias for copy member
  Serial::Parameters & Serial::Parameters::operator = ( const Parameters & other )
  {
    return copy( other );
  }

  //---------------------------------------------------------------------------
  // clone member
  Serial::Parameters * Serial::Parameters::clone() const
  {
    return new Parameters( * this );
  }

  //---------------------------------------------------------------------------
  /// write the Parameters in the given InOutHandler
  /// @param handler the InOutHandler to be used
  /// @param complete if true (the default) the enclosing begin/end will
  /// be also written, otherwise only the data block will be written.
  /// @return true if write was succeful
  bool Serial::Parameters::write( InOutHandler & handler, const bool complete ) const
  {
    bool b = true;
    if ( complete )
    {
      b = handler.writeBegin();
    }

    if ( b )
    {
      switch ( baudRate_ )
      {
        case Baud0:
          ldoem_::write( handler, "baudRate", "Baud0" );
        break;
        case Baud300:
          ldoem_::write( handler, "baudRate", "Baud300" );
        break;
        case Baud600:
          ldoem_::write( handler, "baudRate", "Baud600" );
        break;
        case Baud1200:
          ldoem_::write( handler, "baudRate", "Baud1200" );
        break;
        case Baud1800:
          ldoem_::write( handler, "baudRate", "Baud1800" );
        break;
        case Baud2400:
          ldoem_::write( handler, "baudRate", "Baud2400" );
        break;
        case Baud4800:
          ldoem_::write( handler, "baudRate", "Baud4800" );
        break;
        case Baud9600:
          ldoem_::write( handler, "baudRate", "Baud9600" );
        break;
        case Baud19200:
          ldoem_::write( handler, "baudRate", "Baud19200" );
        break;
        case Baud38400:
          ldoem_::write( handler, "baudRate", "Baud38400" );
        break;
        case Baud57600:
          ldoem_::write( handler, "baudRate", "Baud57600" );
        break;
        case Baud76800:
          ldoem_::write( handler, "baudRate", "Baud76800" );
        break;
        case Baud115200:
          ldoem_::write( handler, "baudRate", "Baud115200" );
        break;
      }

      switch ( port_ )
      {
        case Com1:
          ldoem_::write( handler, "port", "Com1" );
        break;
        case Com2:
          ldoem_::write( handler, "port", "Com2" );
        break;
        case Com3:
          ldoem_::write( handler, "port", "Com3" );
        break;
        case Com4:
          ldoem_::write( handler, "port", "Com4" );
        break;
        case Com5:
          ldoem_::write( handler, "port", "Com5" );
        break;
        case Com6:
          ldoem_::write( handler, "port", "Com6" );
        break;
        case Com7:
          ldoem_::write( handler, "port", "Com7" );
        break;
        case Com8:
          ldoem_::write( handler, "port", "Com8" );
        break;
        default:
          ldoem_::write( handler, "port", "Com1" );
      }

      switch ( characterSize_ )
      {
        case Cs4:
          ldoem_::write( handler, "characterSize", "Cs4" );
        break;
        case Cs5:
          ldoem_::write( handler, "characterSize", "Cs5" );
        break;
        case Cs6:
          ldoem_::write( handler, "characterSize", "Cs6" );
        break;
        case Cs7:
          ldoem_::write( handler, "characterSize", "Cs7" );
        break;
        default:
          ldoem_::write( handler, "characterSize", "Cs8" );
        break;
      }

      switch ( parity_ )
      {
        case No:
          ldoem_::write( handler, "parity", "No" );
        break;
        case Even:
          ldoem_::write( handler, "parity", "Even" );
        break;
        case Odd:
          ldoem_::write( handler, "parity", "Odd" );
        break;
        case Space:
          ldoem_::write( handler, "parity", "Space" );
        break;
        case Mark:
          ldoem_::write( handler, "parity", "Mark" );
        break;
        default:
          ldoem_::write( handler, "parity", "No" );
        break;
      }

      switch ( stopBits_ )
      {
        case One:
          ldoem_::write( handler, "stopBits", "One" );
        break;
        case OneFive:
          ldoem_::write( handler, "stopBits", "OneFive" );
        break;
        case Two:
          ldoem_::write( handler, "stopBits", "Two" );
        break;
        default:
          ldoem_::write( handler, "stopBits", "One" );
        break;
      }

      ldoem_::write( handler, "receiveTimeout", receiveTimeout_ );
    }

    if ( complete )
    {
      b = b && handler.writeEnd();
    }

    return b;
  }

  //---------------------------------------------------------------------------
  /// read the parameters from the given InOutHandler
  /// @param handler the InOutHandler to be used
  /// @param complete if true (the default) the enclosing begin/end will
  /// be also read, otherwise only the data block will be read.
  /// @return true if write was succeful
  bool Serial::Parameters::read( InOutHandler & handler, const bool complete )
  {
    bool b = true;
    if ( complete )
    {
      b = handler.readBegin();
    }

    if ( b )
    {
      std::string tmp;

      ldoem_::read( handler, "baudRate", tmp );
      if ( tmp == "Baud0" )
      {
        baudRate_ = Baud0;
      }
      else if ( tmp == "Baud300" )
      {
        baudRate_ = Baud300;
      }
      else if ( tmp == "Baud600" )
      {
        baudRate_ = Baud600;
      }
      else if ( tmp == "Baud1200" )
      {
        baudRate_ = Baud1200;
      }
      else if ( tmp == "Baud1800" )
      {
        baudRate_ = Baud1800;
      }
      else if ( tmp == "Baud2400" )
      {
        baudRate_ = Baud2400;
      }
      else if ( tmp == "Baud4800" )
      {
        baudRate_ = Baud4800;
      }
      else if ( tmp == "Baud9600" )
      {
        baudRate_ = Baud9600;
      }
      else if ( tmp == "Baud19200" )
      {
        baudRate_ = Baud19200;
      }
      else if ( tmp == "Baud38400" )
      {
        baudRate_ = Baud38400;
      }
      else if ( tmp == "Baud57600" )
      {
        baudRate_ = Baud57600;
      }
      else if ( tmp == "Baud76800" )
      {
        baudRate_ = Baud76800;
      }
      else if ( tmp == "Baud115200" )
      {
        baudRate_ = Baud115200;
      }
      else
      {
        // default value if unknown tag
        baudRate_ = Baud9600;
      }

      ldoem_::read( handler, "port", tmp );
      if ( tmp == "Com1" )
      {
        port_ = Com1;
      }
      else if ( tmp == "Com2" )
      {
        port_ = Com2;
      }
      else if ( tmp == "Com3" )
      {
        port_ = Com3;
      }
      else if ( tmp == "Com4" )
      {
        port_ = Com4;
      }
      else if ( tmp == "Com5" )
      {
        port_ = Com5;
      }
      else if ( tmp == "Com6" )
      {
        port_ = Com6;
      }
      else if ( tmp == "Com7" )
      {
        port_ = Com7;
      }
      else if ( tmp == "Com8" )
      {
        port_ = Com8;
      }
      else
      {
        // default value
        port_ = Com1;
      }

      ldoem_::read( handler, "characterSize", tmp );

      if ( tmp == "Cs4" )
      {
        characterSize_ = Cs4;
      }
      else if ( tmp == "Cs5" )
      {
        characterSize_ = Cs5;
      }
      else if ( tmp == "Cs6" )
      {
        characterSize_ = Cs6;
      }
      else if ( tmp == "Cs7" )
      {
        characterSize_ = Cs7;
      }
      else if ( tmp == "Cs8" )
      {
        characterSize_ = Cs8;
      }
      else
      {
        // default value
        characterSize_ = Cs8;
      }

      ldoem_::read( handler, "parity", tmp );
      if ( tmp == "No" )
      {
        parity_ = No;
      }
      else if ( tmp == "Even" )
      {
        parity_ = Even;
      }
      else if ( tmp == "Odd" )
      {
        parity_ = Odd;
      }
      else if ( tmp == "Space" )
      {
        parity_ = Space;
      }
      else if ( tmp == "Mark" )
      {
        parity_ = Mark;
      }
      else
      {
        // default value
        parity_ = No;
      }

      ldoem_::read( handler, "stopBits", tmp );
      if ( tmp == "One" )
      {
        stopBits_ = One;
      }
      else if ( tmp == "OneFive" )
      {
        stopBits_ = OneFive;
      }
      else if ( tmp == "Two" )
      {
        stopBits_ = Two;
      }
      else
      {
        // default value
        stopBits_ = One;
      }

      ldoem_::read( handler, "receiveTimeout", receiveTimeout_ );
    }

    if ( complete )
    {
      b = b && handler.readEnd();
    }

    return b;
  }

  //---------------------------------------------------------------------------
  void Serial::Parameters::setBaudrate( uint16 baudrate )
  {

    // now set the parameters chosen by the user:
    switch ( baudrate )
    {
      case 1 :
        baudRate_ = Baud4800;
      break;

      case 2 :
        baudRate_ = Baud9600;
      break;

      case 3 :
        baudRate_ = Baud19200;
      break;

      case 4 :
        baudRate_ = Baud38400;
      break;

      case 5 :
        baudRate_ = Baud57600;
      break;

      case 6 :
        baudRate_ = Baud115200;
      break;

      default:
        baudRate_ = Baud115200;
    }

  }


  // --------------------------------------------------
  // Serial
  // --------------------------------------------------

  //---------------------------------------------------------------------------
  // default constructor
  Serial::Serial() : Object(), statusString_( 0 ), parameters_( *::Laser360::Parameters::instance() )
  {

    isPortOpened_ = false;

    // create an instance of the parameters with the default values
    Parameters defaultParameters;
    // set the default parameters
    setParameters( defaultParameters );
  }

  //---------------------------------------------------------------------------
  // default constructor
  Serial::Serial( const Serial::Parameters & SerialPar ) : Object(), statusString_( 0 ),
     parameters_( *::Laser360::Parameters::instance() )
     {

       isPortOpened_ = false;

       // set the default parameters
       setParameters( SerialPar );
  }


  //---------------------------------------------------------------------------
  // copy constructor
  Serial::Serial( const Serial & other ) : Object(), statusString_( 0 ),
     parameters_( *::Laser360::Parameters::instance() )
     {
       copy( other );
  }

  //---------------------------------------------------------------------------
  // destructor
  Serial::~Serial()
  {
    delete[] statusString_;
    statusString_ = 0;
  }

  //---------------------------------------------------------------------------
  // returns the name of this type
  const char * Serial::getTypeName() const
  {
    return "Serial";
  }

  //---------------------------------------------------------------------------
  // copy member
  Serial & Serial::copy( const Serial & other )
  {
    closePort();
    param_.copy( other.param_ );
    return * this;
  }

  //---------------------------------------------------------------------------
  // return parameters
  const Serial::Parameters & Serial::getParameters() const
  {
    return param_;
  }

  //---------------------------------------------------------------------------
  //set parameters
  bool Serial::setParameters( const Parameters & sParameters )
  {
    param_.copy( sParameters );
    return true;
  }

  //---------------------------------------------------------------------------
  /// return the last message set with setStatusString().  This will
  /// never return 0.  If no status-string has been set yet an empty string
  /// (pointer to a string with only the char(0)) will be returned.
  const char * Serial::getStatusString() const
  {
    if ( isNull( statusString_ ) )
    {
      return emptyString_;
    }
    else
    {
      return statusString_;
    }
  }

  //---------------------------------------------------------------------------
  /// set a status string.
  /// @param msg the const string to be reported next time by * getStatusString()
  /// This message will be usually set within the apply methods to indicate * an error cause. */
  void Serial::setStatusString( const char * msg ) const
  {
    delete[] statusString_;
    statusString_ = 0;

    statusString_ = new char[strlen( msg ) + 1];
    strcpy( statusString_, msg );
  }

  //---------------------------------------------------------------------------
  // open the serial port
  bool Serial::openPort()
  {

    if ( isPortOpened_ )
    {
      closePort();
    }

    struct termios allParameters;

    // original flags
    //int flags = O_RDWR | O_NOCTTY; // | O_NDELAY

    // flags used in Serial of robotic group
    int flags = O_CREAT | O_RDWR | O_NONBLOCK;


    descriptor_ = open( parameters_.device.c_str(), flags );

/*
    // original port
    switch ( param_.port_ )
    {
      case Parameters::Com1:
        descriptor_ = open( "/dev/ttyUSB0", flags );
      break;
      case Parameters::Com2:
        descriptor_ = open( "/dev/ttyS1", flags );
      break;
      case Parameters::Com3:
        descriptor_ = open( "/dev/ttyS2", flags );
      break;
      case Parameters::Com4:
        descriptor_ = open( "/dev/ttyS3", flags );
      break;
      case Parameters::Com5:
        descriptor_ = open( "/dev/ttyS4", flags );
      break;
      case Parameters::Com6:
        descriptor_ = open( "/dev/ttyS5", flags );
      break;
      case Parameters::Com7:
        descriptor_ = open( "/dev/ttyS6", flags );
      break;
      case Parameters::Com8:
        descriptor_ = open( "/dev/ttyS7", flags );
      break;
      default:
        descriptor_ = open( "/dev/ttyS0", flags );
      break;
    }
*/

    if ( descriptor_ != -1 )
    {

      //get the current parameters for the port
      if ( tcgetattr( descriptor_, & allParameters ) != -1 )
      {

        // first than all, clear everything!
        allParameters.c_iflag &= ~( IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL );
        // Raw output option,similary as the raw input.The characters
        // are transmitted as-is
        allParameters.c_oflag &= ~OPOST;
        allParameters.c_lflag &= ~( ECHO | ECHONL | ICANON | ISIG | IEXTEN );
        allParameters.c_cflag &= ~( CSIZE | PARENB );

        // now set the parameters chosen by the user:
        switch ( param_.baudRate_ )
        {

          case Parameters::Baud0:
            cfsetispeed( & allParameters, B0 );
            cfsetospeed( & allParameters, B0 );
          break;
          case Parameters::Baud300:
            cfsetispeed( & allParameters, B300 );
            cfsetospeed( & allParameters, B300 );
          break;
          case Parameters::Baud600:
            cfsetispeed( & allParameters, B600 );
            cfsetospeed( & allParameters, B600 );
          break;
          case Parameters::Baud1200:
            cfsetispeed( & allParameters, B1200 );
            cfsetospeed( & allParameters, B1200 );
          break;
          case Parameters::Baud1800:
            cfsetispeed( & allParameters, B1800 );
            cfsetospeed( & allParameters, B1800 );
          break;
          case Parameters::Baud2400:
            cfsetispeed( & allParameters, B2400 );
            cfsetospeed( & allParameters, B2400 );
          break;
          case Parameters::Baud4800:
            cfsetispeed( & allParameters, B4800 );
            cfsetospeed( & allParameters, B4800 );
          break;
          case Parameters::Baud9600:
            cfsetispeed( & allParameters, B9600 );
            cfsetospeed( & allParameters, B9600 );
          break;
          case Parameters::Baud19200:
            cfsetispeed( & allParameters, B19200 );
            cfsetospeed( & allParameters, B19200 );
          break;
          case Parameters::Baud38400:
            cfsetispeed( & allParameters, B38400 );
            cfsetospeed( & allParameters, B38400 );
          break;
          case Parameters::Baud57600:
            cfsetispeed( & allParameters, B57600 );
            cfsetospeed( & allParameters, B57600 );
          break;
          case Parameters::Baud76800:
            setStatusString( "Baud76800 not supported" );
            return false;
          break;
          case Parameters::Baud115200:
            cfsetispeed( & allParameters, B115200 );
            cfsetospeed( & allParameters, B115200 );
          break;
          default:
            cfsetispeed( & allParameters, B9600 );
            cfsetospeed( & allParameters, B9600 );
        }

        switch ( param_.parity_ )
        {

          case Parameters::Odd:
            allParameters.c_cflag |= PARENB; // enable parity
            allParameters.c_cflag |= PARODD; // odd parity
            allParameters.c_iflag |= ( INPCK | ISTRIP );
          break;

          case Parameters::Even:
            allParameters.c_cflag |= PARENB; // enable parity
            allParameters.c_cflag &= ~PARODD; // even parity
            allParameters.c_iflag |= ( INPCK | ISTRIP );
          break;

          case Parameters::Space:
          case Parameters::No:
            allParameters.c_cflag &= ~PARENB;
          break;
          default:
            allParameters.c_cflag &= ~PARENB;
        }

        allParameters.c_cflag &= ~CSIZE; // mask the character size bits

        switch ( param_.characterSize_ )
        {
          case Parameters::Cs5:
            allParameters.c_cflag |= CS5;
          break;
          case Parameters::Cs6:
            allParameters.c_cflag |= CS6;
          break;
          case Parameters::Cs7:
            allParameters.c_cflag |= CS7;
          break;
          case Parameters::Cs8:
            allParameters.c_cflag |= CS8;
          break;
          default:
            allParameters.c_cflag |= CS8;
          break;
        }

        if ( param_.stopBits_ == Parameters::Two )
        {
          //if the stop bit is set,2 stop bits are used.Otherwise,only 1 stop bit is used.
          allParameters.c_cflag |= CSTOPB; //set the stop bit.
        }
        else
        {
          allParameters.c_cflag &= ~CSTOPB; //clear the 2 stop bit.
        }

        // These ones should always be set:
        allParameters.c_cflag |= CREAD; // enable receiver
        allParameters.c_cflag |= CLOCAL; // Local line -
        // do not change "owner" of port

        // software flow control disabled (TODO: this could also be somehow parameterized)
        allParameters.c_iflag &= ~( IXON | IXOFF | IXANY );

        // Raw input.
        // Input characters are passed through exactly as they are received
        allParameters.c_lflag &= ~( ICANON | ECHO | ECHOE | ISIG | IEXTEN );

        //
        // Taken from: http://www.easysw.com/~mike/Serial/Serial.html#2_5_2
        //
        // VMIN specifies the minimum number of characters to read. If
        // it is set to 0, then the VTIME value specifies the time to
        // wait for every character read. Note that this does not mean
        // that a read call for N bytes will wait for N characters to
        // come in. Rather, the timeout will apply to the first
        // character and the read call will return the number of
        // characters immediately available (up to the number you
        // request).
        //
        // If VMIN is non-zero, VTIME specifies the time to wait for
        // the first character read. If a character is read within the
        // time given, any read will block (wait) until all VMIN
        // characters are read. That is, once the first character is
        // read, the Serial interface driver expects to receive an
        // entire packet of characters (VMIN bytes total). If no
        // character is read within the time allowed, then the call to
        // read returns 0. This method allows you to tell the Serial
        // driver you need exactly N bytes and any read call will
        // return 0 or N bytes. However, the timeout only applies to
        // the first character read, so if for some reason the driver
        // misses one character inside the N byte packet then the read
        // call could block forever waiting for additional input
        // characters.
        //
        // VTIME specifies the amount of time to wait for incoming
        // characters in tenths of seconds. If VTIME is set to 0 (the
        // default), reads will block (wait) indefinitely unless the
        // NDELAY option is set on the port with open or fcntl.

        // number of characters to be received (zero means wait until anything comes!)
        allParameters.c_cc[VMIN] = 0;
        // time to wait in deciseconds to receive every character,while VMIN=0
        allParameters.c_cc[VTIME] = iround( param_.receiveTimeout_ / 50.0 );

        tcflush( descriptor_, TCIFLUSH );

        if ( tcsetattr( descriptor_, TCSANOW, & allParameters ) == -1 )
        {
          //set all the parameters without waiting for data to complete
          setStatusString( "could not set the parameters" );
          closePort();
          return false;
        }
      }
      else
      {
        setStatusString( "error: unable to get the current parameters" );
        closePort();
        return false;
      }
    }
    else
    {
      setStatusString( strerror( errno ) );
      closePort();
      return false;
    }

    fcntl( descriptor_, F_SETFL, 0 );

    isPortOpened_ = true;
    return isPortOpened_;
  }

  //---------------------------------------------------------------------------
  // sends only one character
  bool Serial::send( const unsigned char c )
  {
    return send( static_cast < char > ( c ) );
  }

  //---------------------------------------------------------------------------
  // sends only one character
  bool Serial::send( const char c )
  {

    if ( !isPortOpened_ )
    {
      if ( !openPort() )
      {
        // some error occured by opening port
        return false;
      }
    }

    int numberOfBytes;
    numberOfBytes =::write( descriptor_, & c, 1 );
    if ( numberOfBytes == 1 )
    {
      return true;
    }
    else
    {
      setStatusString( "error: could not send a character" );
      return false;
    }
  }

  //---------------------------------------------------------------------------
  // sends a string
  bool Serial::send( const std::string & theString )
  {

    if ( !isPortOpened_ )
    {
      if ( !openPort() )
      {
        // some error occured by opening port
        return false;
      }
    }

    int counter = 0;

    while ( ( counter < static_cast < int > ( theString.size() ) ) && ( send( theString[counter] ) == true ) )
    {
      counter++;
    }

    return ( counter == static_cast < int > ( theString.size() ) );
  }

  //---------------------------------------------------------------------------
  // receives only one character
  bool Serial::receive( char & c )
  {

    if ( !isPortOpened_ )
    {
      if ( !openPort() )
      {
        // some error occured by opening port
        return false;
      }
    }

    int numberOfBytes;
    numberOfBytes =::read( descriptor_, & c, 1 );

    if ( numberOfBytes == 1 )
    {
      return true;
    }
    else
    {
      setStatusString( "error: could not receive the character" );
      return false;
    }
  }

  //---------------------------------------------------------------------------
  // receives a string
  bool Serial::receive( std::string & theString )
  {

    bool control = false;
    char ch;

    theString = "";

    do
    {
      if ( control = receive( ch ) )
      {
        theString += ch;
      }
    }
    while ( ( ch != '\n' ) && ( ch != 0 ) && control );

    return control;
  }

  //---------------------------------------------------------------------------
  // close the port
  void Serial::closePort()
  {
    if ( close( descriptor_ ) == -1 )
    {
      setStatusString( "error: the port could not be closed" );
    }
    descriptor_ = -1;
    isPortOpened_ = false;
  }
} // end namspace
