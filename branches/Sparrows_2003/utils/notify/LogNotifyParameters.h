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
#ifndef LogNotifyParameters_h
#define LogNotifyParameters_h

#include "miro/Parameters.h"

#include <string>
#include <vector>

class LogNotifyParameters : public Miro::Parameters
{
  typedef Miro::Parameters Super;
public:
  typedef std::vector<std::string> LogTypesVector;
  
  unsigned int maxFileSize;
  LogTypesVector typeNames;

  LogNotifyParameters();
  virtual void operator <<= (const QDomNode& node);
};

std::ostream&
operator << (std::ostream& ostr, const LogNotifyParameters& desc);

#endif
