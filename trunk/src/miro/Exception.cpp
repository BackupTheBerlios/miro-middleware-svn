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

#include "Exception.h"

namespace Miro
{
  Exception::Exception() throw() : 
    Super(),
    what_()
  {}

  /**
   * You can pass this constructor a string, which describes the
   * cause of the exception.
   */
  Exception::Exception(const std::string& _what)  throw() : 
    Super(),
    what_(_what) 
  {}

  Exception::~Exception() throw()
  {}

  /**
   * @return A pointer to the string describing the exception
   * reason.  This string is set when the exception is thrown.
   */
  const char* Exception::what () const throw() 
  {
    return what_.c_str (); 
  }

  /**
   * This operator pipes a Miro::Exception to a specified ostream.
   *
   * @return Reference to the ostream.
   */
  std::ostream& operator << (std::ostream& ostr, const Exception& x) 
  {
    return ostr << x.what();
  }

  /**
   * Pass the value of errno and a string describing the cause for this exception
   * to be thrown to this constructor.
   */
  CException::CException(int _error, const std::string& _what) throw() :
    Super(_what),
    error_(_error)
  {}

  CException::~CException() throw()
  {}

  /**
   * Returns a number describing the exception reason.
   * This number is set when the exception is thrown.
   */
  int CException::error_num() const throw() 
  { 
    return error_; 
  }

  /**
   * This operator pipes a Miro::CException to a specified ostream.
   *
   * @return Reference to the ostream.
   */
  std::ostream& operator << (std::ostream& ostr, const CException& x) 
  {
    return ostr << x.what() << " - " 
		<< x.error_num() << ": " << std::strerror(x.error_num());
  }

  /**
   * Pass the value of errno and a string describing the cause for this exception
   * to be thrown to this constructor.
   */
  ACE_Exception::ACE_Exception(int _errno, const std::string& _what) throw() :
    Super(_errno, _what) 
  {}

  ACE_Exception::~ACE_Exception() throw()
  {}
};
