// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef DevParameters_h
#define DevParameters_h

#include "Parameters.h"

#include <string>

namespace Miro
{
  class DevParameters : public Parameters
  {
    typedef Parameters Super;
  public:
    //! Fully qualified path to the device file.
    std::string  device;

    DevParameters();
    virtual ~DevParameters();
    virtual void operator <<= (const QDomNode& node);
  };

  std::ostream&
  operator << (std::ostream& ostr, const DevParameters& desc);
};

#endif
 
