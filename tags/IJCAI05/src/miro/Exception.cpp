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
  /**
   * This operator pipes a Miro::Exception to a specified ostream.
   *
   * @return Reference to the ostream.
   */
  std::ostream& operator << (std::ostream& _ostr, const Exception& x) 
  {
    x.printToStream(_ostr);
    return _ostr;
  }

  /** Noop implementation. */
  Exception::Exception() throw() : 
    Super(),
    what_()
  {}

  /**
   * You can pass this constructor a string, which describes the
   * cause of the exception.
   */
  Exception::Exception(const std::string& _what) throw() : 
    Super(),
    what_(_what) 
  {}

  /** Noop implementation. */
  Exception::~Exception() throw()
  {}

  /**
   * @return A pointer to the string describing the exception
   * reason.  This string is set when the exception is thrown.
   */
  char const * 
  Exception::what () const throw() 
  {
    return what_.c_str (); 
  }

  /**
   * This method pipes a Miro::Exception to a specified ostream.
   *
   * @return Reference to the ostream.
   */
  void 
  Exception::printToStream(std::ostream& _ostr) const
  {
    _ostr << what();
  }

  /**
   * Pass the value of errno and a string describing the cause for this exception
   * to be thrown to this constructor.
   */
  CException::CException(int _error, const std::string& _what) throw() :
    Super(_what),
    error_(_error)
  {}

  /** Noop implementation. */
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
   * This method pipes a Miro::CException to a specified ostream.
   *
   * @return Reference to the ostream.
   */
  void
  CException::printToStream(std::ostream& _ostr) const
  {
    _ostr << what() << " - " 
	  << error_num() << ": " << std::strerror(error_num());
  }

  /**
   * Pass the value of errno and a string describing the cause for this exception
   * to be thrown to this constructor.
   */
  ACE_Exception::ACE_Exception(int _errno, const std::string& _what) throw() :
    Super(_errno, _what) 
  {}

  /** Noop implementation. */
  ACE_Exception::~ACE_Exception() throw()
  {}
}
