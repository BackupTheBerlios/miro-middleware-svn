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

#include "DevConsumer.h"
#include "DevMessage.h"
#include "Exception.h"

#include <iostream>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Miro
{
  DevConsumer::DevConsumer()
  {
    DBG(std::cout << "Constructing DevConsumer." << std::endl);
  }

  DevConsumer::~DevConsumer()
  {
    DBG(std::cout << "Destructing DevConsumer." << std::endl);
  }

  void 
  DevConsumer::handleMessage(const DevMessage *)
  {
    std::cerr << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ 
	 << "() - Unhandled Message!" << std::endl;
  }
}
