// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroException_h
#define miroException_h

#include <exception>
#include <string>
#include <iostream>

#define MIRO_EXCEPTION_TYPE(N) \
  class N : public ::Miro::Exception \
  { \
    typedef ::Miro::Exception Super; \
  public: \
    N() throw(): Super() {}  \
    N(std::string const& _what) throw() : Super(_what) {} \
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
    ~Exception() throw();

    //! Standart information hook.
    virtual char const * what() const throw();

  private:
    std::string const what_;
  };

  //! Output operator.
  std::ostream& operator << (std::ostream& ostr, const Exception& x);

  //! Class to throw C errors as exceptions.
  class CException : public Exception 
  {
    typedef Exception Super;
  public:
    //! Initializing constructor.
    CException(int _error, const std::string& _what) throw();
    ~CException() throw();
    //! C error number.
    virtual int error_num() const throw();

  private:
    int error_;
  };

  //! Output operator.
  std::ostream& operator << (std::ostream& ostr, const CException& x);

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
    ~ACE_Exception() throw();
  };
}
#endif




