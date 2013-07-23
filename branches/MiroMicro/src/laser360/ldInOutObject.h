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
