// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroException_hh
#define miroException_hh

#include <exception>
#include <string>
#include <iostream>

namespace Miro
{
  /**
   * The root of the server side exception hierarchy.
   *
   * This is the root class of the exceptions used within Miro Servers.
   * Exceptions that raise on the client side are CORBA exception, which are
   * defined in miroException.idl. As a pure client programmer you can 
   * safely ignore this class and its derivates.
   */
  class Exception : public std::exception 
  {
    typedef exception Super;
  public:
    /**
     * Default constructor.
     */
    Exception() throw();
    /**
     * Initializing constructor
     *
     * You can pass this constructor a string, which describes the cause exception.
     */
    Exception(const std::string& _what) throw(); 

    ~Exception() throw();

    /**
     * Returns a string describing the exception reason.
     * This string is set when the exception is thrown.
     */
    virtual const char* what() const throw();

  private:
    std::string what_;
  };

  /**
   * Output operator.
   *
   * This operator pipes a Miro::Exception to a specified ostream.
   *
   * @return Reference to the ostream.
   */
  std::ostream& operator << (std::ostream& ostr, const Exception& x);

  /**
   * Class to throw C errors as exceptions.
   *
   */
  class CException : public Exception 
  {
    typedef Exception Super;
  public:
    /**
     * Initializing constructor.
     *
     * Pass the value of errno and a string describing the cause for this exception
     * to be thrown to this constructor.
     */
    CException(int _error, const std::string& _what) throw();
    ~CException() throw();
    /**
     * Returns a number describing the exception reason.
     * This number is set when the exception is thrown.
     */
    virtual int error_num() const throw();
  private:
    int error_;
  };

  /**
   * Output operator.
   *
   * This operator pipes a Miro::CException to a specified ostream.
   *
   * @return Reference to the ostream.
   */
  std::ostream& operator << (std::ostream& ostr, const CException& x);

  /**
   * Class to throw errors of the ACE library as exceptions.
   */
  class ACE_Exception : public CException 
  {
    typedef CException Super;
  public:
    /**
     * Initializing constructor.
     *
     * Pass the value of errno and a string describing the cause for this exception
     * to be thrown to this constructor.
     */
    ACE_Exception(int _errno, const std::string& _what) throw();
    ~ACE_Exception() throw();
  };
};
#endif




