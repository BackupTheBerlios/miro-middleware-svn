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

#include "miro/DevConsumer.h"
#include "miro/DevMessage.h"

#include <iostream>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Miro
{
  using std::cout;
  using std::cerr;

  DevConsumer::DevConsumer()
  {
    DBG(cout << "Constructing DevConsumer." << endl);
  }

  DevConsumer::~DevConsumer()
  {
    DBG(cout << "Destructing DevConsumer." << endl);
  }

  void 
  DevConsumer::handleMessage(const DevMessage *)
  {
    cerr << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ 
	 << "() - Unhandled Message!" << endl;
  }
}
