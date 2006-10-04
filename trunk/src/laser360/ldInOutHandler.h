
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldInOutHandler.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LD_IN_OUT_HANDLER_H_
  #define _LD_IN_OUT_HANDLER_H_

  #include "ldObject.h"
  #include <string>
  #include <iostream>

namespace ldoem_
{
  //---------------------------------------------------------------------------
  /// Input/output handlers.
  ///
  /// The InOutHandler classes offer an abstract interface for the functor
  /// parameters and other ldoem_::InOutObject classes to read() and write() them in different formats.
  ///
  /// All InOutHandlers are used as wrappers for std::istreams or std::ostreams
  /// objects. Therefore, the abstract interface provides the virtual methods
  /// use() to indicate which stream has to be used.
  ///
  /// @ingroup gStorable
  class InOutHandler : public Object
  {
  public:

    //---------------------------------------------------------------------------
    /// default constructor
    InOutHandler();

    //---------------------------------------------------------------------------
    /// copy constructor
    InOutHandler( const InOutHandler & other );

    //---------------------------------------------------------------------------
    /// destructor
    virtual ~InOutHandler();

    //---------------------------------------------------------------------------
    /// returns the name of this type ("InOutHandler")
    virtual const char * getTypeName() const;

    //---------------------------------------------------------------------------
    /// copy data of "other" functor.
    /// @param other the functor to be copied
    /// @return a reference to this functor object
    InOutHandler & copy( const InOutHandler & other );

    //---------------------------------------------------------------------------
    /// @returns a pointer to a clone of this functor.
    virtual InOutHandler * clone() const = 0;

    //---------------------------------------------------------------------------
    /// indicate the output stream to be used
    /// Calling this method you will reinitialize the state of the
    /// parser (see also clear()).
    virtual void use( std::ostream & aStream ) = 0;

    //---------------------------------------------------------------------------
    /// indicate the input stream to be used
    /// Calling this method you will reinitialize the state of the
    /// parser (see also clear()).
    virtual void use( std::istream & aStream ) = 0;

    //---------------------------------------------------------------------------
    /// write the begin token or tokens
    virtual bool writeBegin();

    //---------------------------------------------------------------------------
    /// write the end token or tokens
    virtual bool writeEnd();

    //---------------------------------------------------------------------------
    /// read the begin token or tokens
    virtual bool readBegin();

    //---------------------------------------------------------------------------
    /// read the end token or tokens
    virtual bool readEnd();

    //---------------------------------------------------------------------------
    /// write member for the standard types
    virtual bool write( const std::string & data ) = 0;
    virtual bool write( const char * data ) = 0;
    virtual bool write( const double & data ) = 0;
    virtual bool write( const float & data ) = 0;
    virtual bool write( const int & data ) = 0;
    virtual bool write( const unsigned int & data ) = 0;
    virtual bool write( const char & data ) = 0;
    virtual bool write( const signed char & data ) = 0;
    virtual bool write( const unsigned char & data ) = 0;
    virtual bool write( const bool & data ) = 0;
    virtual bool write( const long & data ) = 0;
    virtual bool write( const unsigned long & data ) = 0;
    virtual bool write( const short & data ) = 0;
    virtual bool write( const unsigned short & data ) = 0;

    //---------------------------------------------------------------------------
    /// write member for symbol-data pairs
    virtual bool write( const std::string & name, const std::string & data );
    virtual bool write( const std::string & name, const char * data );
    virtual bool write( const std::string & name, const double & data );
    virtual bool write( const std::string & name, const float & data );
    virtual bool write( const std::string & name, const int & data );
    virtual bool write( const std::string & name, const unsigned int & data );
    virtual bool write( const std::string & name, const char & data );
    virtual bool write( const std::string & name, const signed char & data );
    virtual bool write( const std::string & name, const unsigned char & data );
    virtual bool write( const std::string & name, const bool & data );
    virtual bool write( const std::string & name, const long & data );
    virtual bool write( const std::string & name, const unsigned long & data );
    virtual bool write( const std::string & name, const short & data );
    virtual bool write( const std::string & name, const unsigned short & data );

    //---------------------------------------------------------------------------
    /// @name read members


    //---------------------------------------------------------------------------
    /// read members for the standard types
    virtual bool read( std::string & data ) = 0;
    virtual bool read( double & data ) = 0;
    virtual bool read( float & data ) = 0;
    virtual bool read( int & data ) = 0;
    virtual bool read( unsigned int & data ) = 0;
    virtual bool read( char & data ) = 0;
    virtual bool read( signed char & data ) = 0;
    virtual bool read( unsigned char & data ) = 0;
    virtual bool read( bool & data ) = 0;
    virtual bool read( long & data ) = 0;
    virtual bool read( unsigned long & data ) = 0;
    virtual bool read( short & data ) = 0;
    virtual bool read( unsigned short & data ) = 0;

    //---------------------------------------------------------------------------
    /// read members for symbol-data pairs
    virtual bool read( const std::string & name, std::string & data );
    virtual bool read( const std::string & name, double & data );
    virtual bool read( const std::string & name, float & data );
    virtual bool read( const std::string & name, int & data );
    virtual bool read( const std::string & name, unsigned int & data );
    virtual bool read( const std::string & name, char & data );
    virtual bool read( const std::string & name, signed char & data );
    virtual bool read( const std::string & name, unsigned char & data );
    virtual bool read( const std::string & name, bool & data );
    virtual bool read( const std::string & name, long & data );
    virtual bool read( const std::string & name, unsigned long & data );
    virtual bool read( const std::string & name, short & data );
    virtual bool read( const std::string & name, unsigned short & data );

    //---------------------------------------------------------------------------
    /// write a std::string as a symbol token.
    /// A symbol must not contain special characters or spaces.  Only letters
    /// and numbers are allowed, and the first character must be a letter.
    /// @param data The symbol to be written
    virtual bool writeSymbol( const std::string & data ) = 0;

    //---------------------------------------------------------------------------
    /// write comment writes the input data without any preprocessing,
    /// just ensuring that the comment format is given
    virtual bool writeComment( const std::string & data ) = 0;

    //---------------------------------------------------------------------------
    /// write comment writes the input data without any preprocessing,
    /// just ensuring that the comment format is given
    virtual bool writeComment( const char * data ) = 0;

    //---------------------------------------------------------------------------
    /// read a std::string as a symbol token
    /// A symbol must not contain special characters or spaces.  Only letters
    /// and numbers are allowed, and the first character must be a letter
    /// @param data the symbol readed will be left here
    virtual bool readSymbol( std::string & data ) = 0;

    //---------------------------------------------------------------------------
    /// try to read the given symbol from the handler.
    /// If present, returns true and the token is removed from the
    /// handler, if not present returns false and leaves the handle as
    /// it was before calling this member...
    /// A symbol must not contain special characters or spaces.  Only letters
    /// and numbers are allowed, and the first character must be a letter.
    /// @param data the symbol to be readed
    virtual bool trySymbol( const std::string & data ) = 0;

    //---------------------------------------------------------------------------
    /// try to read the begin token from the handler.
    /// If present, returns true and the token is removed from the
    /// handler, if not present returns false and leaves the handle as
    /// it was before calling this member...
    virtual bool tryBegin() = 0;

    //---------------------------------------------------------------------------
    /// try to read the end token from the handler.
    /// If present, returns true and the token is removed from the
    /// handler, if not present returns false and leaves the handle as
    /// it was before calling this member...
    virtual bool tryEnd() = 0;

    //---------------------------------------------------------------------------
    /// write spaces (default value 1)
    /// The spaces are tokes that will be ignored by reading
    /// @param s the number of space-tokens to be written
    virtual bool writeSpaces( const int & s = 1 ) = 0;

    //---------------------------------------------------------------------------
    /// write end-of-line token
    /// The EOL are tokes that will be ignored by reading
    virtual bool writeEOL() = 0;

    //---------------------------------------------------------------------------
    /// write key/value separator.
    /// The key/value separator is the token which will separate a symbol from
    /// its value in a symbol-data-pair.
    virtual bool writeKeyValueSeparator() = 0;

    //---------------------------------------------------------------------------
    /// write inter-data separator.
    /// The data-separator token separates data elements in a data list or array
    virtual bool writeDataSeparator() = 0;

    //---------------------------------------------------------------------------
    /// read key/value separator token.
    virtual bool readKeyValueSeparator() = 0;

    //---------------------------------------------------------------------------
    /// read inter-data separator token.
    virtual bool readDataSeparator() = 0;

    //---------------------------------------------------------------------------
    /// return the actual nesting level vor begins and ends
    /// Example
    virtual const int & getLevel() const;

    //---------------------------------------------------------------------------
    /// restore all the information in the handler taken in the actual level.
    virtual bool restoreLevel() = 0;

    //---------------------------------------------------------------------------
    /// usually this member is needed by reinitialization routines to set
    /// the begin/end nesting level to zero.
    virtual void resetLevel( const int & theLevel = 0 );

    //---------------------------------------------------------------------------
    /// @name Error Report
    /// return the last message set with setStatusString().  This will
    /// never return 0.  If no status-string has been set yet an empty string
    /// (pointer to a string with only the char(0)) will be returned.
    virtual const char * getStatusString() const;

    //---------------------------------------------------------------------------
    /// set a status string.
    /// @param msg the const string to be reported next time by
    /// getStatusString(). The given string will be copied.
    /// This message will be usually set within the apply methods to indicate
    /// an error cause.
    /// Note that the change of the status string is not considered as
    /// a change in the functor status.
    virtual void setStatusString( const char * msg ) const;

    //---------------------------------------------------------------------------
    /// set a status string.
    /// @param msg the const string to be reported next time by
    /// getStatusString(). The given string will be copied.
    /// This message will be usually set within the apply methods to indicate
    /// an error cause.
    /// Note that the change of the status string is not considered as
    /// a change in the functor status.
    virtual void setStatusString( const std::string & msg ) const;

    //---------------------------------------------------------------------------
    /// append a message to the current status string. Take care to
    /// reset the status string by calling setStatusString() for each
    /// call of an apply() or similar method. appendStatusString()
    /// should only be used after setStatusString() has been called.
    /// @param msg message to be appended to the current status string.
    /// Note that the change of the status string is not considered as
    /// a change in the functor status.
    virtual void appendStatusString( const char * msg ) const;

    //---------------------------------------------------------------------------
    /// append a message to the current status string. Take care to
    /// reset the status string by calling setStatusString() for each
    /// call of an apply() or similar method. appendStatusString()
    /// should only be used after setStatusString() has been called.
    /// @param msg message to be appended to the current status string.
    /// Note that the change of the status string is not considered as
    /// a change in the functor status.
    virtual void appendStatusString( const std::string & msg ) const;

    //---------------------------------------------------------------------------
    /// append an integer value to the current status string. Take care
    /// to reset the status string by calling setStatusString() for
    /// each call of an apply() or similar method. appendStatusString()
    /// should only be used after setStatusString() has been called.
    /// @param msg integer value to be appended to the current status string.
    /// Note that the change of the status string is not considered as a change in the functor status.
    virtual void appendStatusString( const int & msg ) const;

    //---------------------------------------------------------------------------
    /// append a double value to the current status string. Take care
    /// to reset the status string by calling setStatusString() for
    /// each call of an apply() or similar method. appendStatusString()
    /// should only be used after setStatusString() has been called.
    /// @param msg double value to be appended to the current status string.
    /// Note that the change of the status string is not considered as a change in the functor status.
    virtual void appendStatusString( const double & msg ) const;

    //---------------------------------------------------------------------------
    /// append contextual information
    /// This function should help the users to find errors in their files.
    /// It just inserts some contextual information into the status string to help localizing wrong data.
    /// It is useful for streams that can be edited by hand, because the users will make errors!
    /// The default behaviour does nothing.
    virtual void appendContextStatus() const;


  protected:
    //---------------------------------------------------------------------------
    /// begin() nesting level
    int level;

  private:
    //---------------------------------------------------------------------------
    /// the status string written with setStatusString
    mutable char * statusString;

    //---------------------------------------------------------------------------
    /// the empty string returned if the statusString is empty
    static const char * const emptyString;
  };

  //---------------------------------------------------------------------------
  /// read functions for standard types
  bool read( InOutHandler & handler, std::string & data );
  bool read( InOutHandler & handler, double & data );
  bool read( InOutHandler & handler, float & data );
  bool read( InOutHandler & handler, int & data );
  bool read( InOutHandler & handler, unsigned int & data );
  bool read( InOutHandler & handler, char & data );
  bool read( InOutHandler & handler, signed char & data );
  bool read( InOutHandler & handler, unsigned char & data );
  bool read( InOutHandler & handler, bool & data );
  bool read( InOutHandler & handler, long & data );
  bool read( InOutHandler & handler, unsigned long & data );
  bool read( InOutHandler & handler, short & data );
  bool read( InOutHandler & handler, unsigned short & data );

  //---------------------------------------------------------------------------
  /// write functions for standard types
  bool write( InOutHandler & handler, const std::string & data );
  bool write( InOutHandler & handler, const char * data );
  bool write( InOutHandler & handler, const double & data );
  bool write( InOutHandler & handler, const float & data );
  bool write( InOutHandler & handler, const int & data );
  bool write( InOutHandler & handler, const unsigned int & data );
  bool write( InOutHandler & handler, const char & data );
  bool write( InOutHandler & handler, const signed char & data );
  bool write( InOutHandler & handler, const unsigned char & data );
  bool write( InOutHandler & handler, const bool & data );
  bool write( InOutHandler & handler, const long & data );
  bool write( InOutHandler & handler, const unsigned long & data );
  bool write( InOutHandler & handler, const short & data );
  bool write( InOutHandler & handler, const unsigned short & data );


  //---------------------------------------------------------------------------
  /// handler functions with standard storable interface
  template < class T >
  bool write( InOutHandler & handler, const std::string & name, const T & data, const bool complete = true )
  {

    if ( complete )
    {
      handler.writeBegin();
    }
    handler.writeSymbol( name );
    handler.writeKeyValueSeparator();
    write( handler, data );
    if ( complete )
    {
      handler.writeEnd();
    }
    return handler.writeEOL();
  };

  //---------------------------------------------------------------------------
  /// handler functions with standard storable interface
  template < class T >
  bool read( InOutHandler & handler, const std::string & name, T & data, const bool complete = true )
  {
    int level = handler.getLevel();
    bool result = true;

    if ( complete )
    {
      result = handler.readBegin();
    }

    if ( result && handler.trySymbol( name ) )
    {
      result = result && handler.readKeyValueSeparator();

      result = result && read( handler, data );

      if ( complete )
      {
        result = result && handler.readEnd();
      }

      while ( result && ( handler.getLevel() > level ) )
      {
        result = result && handler.readEnd();
      }

    }
    else
    {
      result = false;
      handler.appendStatusString( "\nSymbol " + name + " not found: " );
      handler.appendContextStatus();
      handler.restoreLevel();
    }

    return result;
  };
}
#endif //_LD_IN_OUT_HANDLER_H_
