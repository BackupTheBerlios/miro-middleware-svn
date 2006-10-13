
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldInOutObject.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LD_IN_OUT_OBJECT_H_
  #define _LD_IN_OUT_OBJECT_H_

  #include "ldObject.h"
  #include "ldInOutHandler.h"

namespace ldoem_
{

  //---------------------------------------------------------------------------
  /// basic input/output class for data objects
  /// @ingroup gStorable
  class InOutObject : public Object
  {
  public:

    //---------------------------------------------------------------------------
    /// default constructor
    InOutObject();

    //--------------------------------------------------------------------------
    /// copy constructor
    /// @param other the object to be copied
    InOutObject( const InOutObject & other );

    //---------------------------------------------------------------------------
    /// destructor
    virtual ~InOutObject();

    //---------------------------------------------------------------------------
    /// copy data of "other" InOutObject.
    /// Please note that the status string will _NOT_ be copied!
    InOutObject & copy( const InOutObject & other );

    //---------------------------------------------------------------------------
    /// assigment operator (alias for copy(other)).
    /// @param other the InOutObject to be copied
    /// @return a reference to the actual InOutObject
    InOutObject & operator = ( const InOutObject & other );

    //---------------------------------------------------------------------------
    /// returns name of this type
    const char * getTypeName() const;

    //---------------------------------------------------------------------------
    /// write the parameters in the given ioHandler
    /// @param handler the ioHandler to be used
    /// @param complete if true (the default) the enclosing begin/end will
    /// be also written, otherwise only the data block will be written.
    /// @return true if write was successful
    virtual bool write( InOutHandler & handler, const bool complete = true ) const = 0;
   // {
     //  return true;
    //};

    //---------------------------------------------------------------------------
    /// read the parameters from the given ioHandler
    /// @param handler the ioHandler to be used
    /// @param complete if true (the default) the enclosing begin/end will
    ///  be also written, otherwise only the data block will be written.
    /// @return true if write was successful
    virtual bool read( InOutHandler & handler, const bool complete = true ) = 0;
  //  {
    //   return true;
    //};

  };

  //---------------------------------------------------------------------------
  /// write
  /// @ingroup gStorable
  bool write( InOutHandler & handler, const InOutObject & p, const bool complete = true );

  //---------------------------------------------------------------------------
  /// read
  /// @ingroup gStorable
  bool read( InOutHandler & handler, InOutObject & p, const bool complete = true );
}
#endif //_LD_IN_OUT_OBJECT_H_
