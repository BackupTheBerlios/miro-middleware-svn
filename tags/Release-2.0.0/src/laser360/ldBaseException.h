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
#ifndef _LD_BASE_EXCEPTION_H_
  #define _LD_BASE_EXCEPTION_H_

  #include <string>
  #include <exception>

namespace ldoem_
{
  //----------------------------------------------------------------------------
  /// exception base class
  class BaseException : public std::exception
  {
  public:
    //----------------------------------------------------------------------------
    /// constructor
    ///
    /// @param msg Errortext that you can get afterwards with getMessage()
    BaseException( const std::string msg )
    {
      message_string_ = msg;
    }

    //----------------------------------------------------------------------------
    /// virtual destructor
    virtual ~BaseException() throw()
    {
    };

    //----------------------------------------------------------------------------
    ///
    /// Returns the error-message as C-Style Text - for std::exception compatibility
    /// @return Errormessage
    const char * what() const throw()
    {
      return message_string_.c_str();
    }

    //----------------------------------------------------------------------------
    ///
    /// Get Message as std::string
    /// @return Errormessage
    const std::string getMessage() const
    {
      return message_string_;
    }

    //----------------------------------------------------------------------------
    // protected data members
    //----------------------------------------------------------------------------
  protected:
    std::string message_string_;
  };

}; // end namespace ldoem_
#endif // _LD_BASE_EXCEPTION_H_
