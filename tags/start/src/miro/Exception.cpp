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


#include "miro/Exception.h"

#include <iostream>

namespace Miro
{
  Exception::Exception() throw() : 
    Super(),
    what_()
  {}

  Exception::Exception(const std::string& _what)  throw() : 
    Super(),
    what_(_what) 
  {}

  Exception::~Exception() throw()
  {}

  const char* Exception::what () const throw() 
  {
    return what_.c_str (); 
  }

  std::ostream& operator << (std::ostream& ostr, const Exception& x) 
  {
    return ostr << x.what();
  }

  CException::CException(int _error, const std::string& _what) throw() :
    Super(_what),
    error_(_error)
  {}

  CException::~CException() throw()
  {}

  int CException::error_num() const throw() 
  { 
    return error_; 
  }

  std::ostream& operator << (std::ostream& ostr, const CException& x) 
  {
    return ostr << x.what() << " - " 
		<< x.error_num() << ": " << std::strerror(x.error_num());
  }

  ACE_Exception::ACE_Exception(int _errno, const std::string& _what) throw() :
    Super(_errno, _what) 
  {}

  ACE_Exception::~ACE_Exception() throw()
  {}
};
