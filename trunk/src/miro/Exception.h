// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
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
#ifndef miro_Exception_h
#define miro_Exception_h

#include <exception>
#include <string>
#include <iosfwd>

#define MIRO_EXCEPTION_TYPE(N) \
  class N : public ::Miro::Exception \
  { \
    typedef ::Miro::Exception Super; \
  public: \
    N() throw(): Super() {}  \
    N(std::string const& _what) throw() : Super(_what) {} \
    virtual ~N() throw() {} \
  }

//! The namespace of the Miro project.
/**
 * To not pollute the global namespace of users of Miro, all classes, structs
 * and helper functions are encapsulated into the namespace Miro.
 *
 * For an introduction into the namespace feature of c++ see:
 * - http://www.cplusplus.com/doc/tutorial/tut5-2.html
 * - http://www.mvps.org/windev/cpp/nspaces.html
 * - Also the hard copy manual constains a short description of
 *   how namespaces are used in Miro.
 * - And Stroustrup's C++ contains a longer description.
 *
 */
namespace Miro
{
  // forward declarations
  class Exception;

  //! Debug output stream operator.
  std::ostream& operator << (std::ostream& ostr, const Exception& x);


  //!The root of the server side exception hierarchy.
  /**
   * This is the root class of the exceptions used within Miro Servers.
   * Exceptions that raise on the client side are CORBA exception, which are
   * defined in Exception.idl. As a pure client programmer you can
   * safely ignore this class and its derivates.
   */
  class Exception : public std::exception
  {
    typedef exception Super;
  public:
    //! Default constructor.
    Exception() throw();
    //! Initializing constructor
    Exception(std::string const& _what) throw();
    //! Virtual dtor.
    virtual ~Exception() throw();

    //! Standart information hook.
    virtual char const * what() const throw();

  protected:
    //! Stream output method.
    void printToStream(std::ostream& _ostr) const;

  private:
    std::string const what_;

    friend std::ostream& operator << (std::ostream& ostr, const Exception& x);
  };

  //! Class to throw C errors as exceptions.
  class CException : public Exception
  {
    typedef Exception Super;
  public:
    //! Initializing constructor.
    CException(int _error, const std::string& _what) throw();
    virtual ~CException() throw();
    //! C error number.
    virtual int error_num() const throw();

  protected:
    //! Stream output method.
    void printToStream(std::ostream& _ostr) const;

  private:
    int error_;
  };

  //!  Class to throw errors of the ACE library as exceptions.
  /**
   * ACE errors are actually just C errors. But that way we can
   * seperate them from ordinary, unwrapped C stuff.
   */
  class ACE_Exception : public CException
  {
    typedef CException Super;
  public:
    //! Initializing constructor.
    ACE_Exception(int _errno, const std::string& _what) throw();
    virtual ~ACE_Exception() throw();
  };
}
#endif // miro_Exception_h
