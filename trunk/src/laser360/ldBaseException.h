
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldBaseException.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

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
