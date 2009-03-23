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
#ifndef _LD_OBJECT_H_
  #define _LD_OBJECT_H_

namespace ldoem_
{
  //---------------------------------------------------------------------------
  /// base class for all objects
  class Object
  {
  public:

    //---------------------------------------------------------------------------
    /// default constructor
    Object();

    //---------------------------------------------------------------------------
    /// destructor
    virtual ~Object();

    //---------------------------------------------------------------------------
    /// returns the name of this class. Each class has to
    /// overload this function and return its name. The returned
    /// string is system-independent; however, template classes
    /// do not consider the template argument.
    virtual const char * getTypeName() const;

  protected:

    //---------------------------------------------------------------------------
    /// this flag indicates if any ldoem-object has been constructed
    static bool initialized_;

    //---------------------------------------------------------------------------
    /// member which initialize everything for the ldoem object
    void initialize();
  };

  inline bool isNull( const void * p )
  {
    return p == 0;
  };
  inline bool notNull( const void * p )
  {
    return p != 0;
  };
}
#endif //_LD_OBJECT_H_
