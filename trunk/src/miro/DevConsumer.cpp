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
#include <miro/Log.h>


namespace Miro
{
  DevConsumer::DevConsumer()
  {
    MIRO_DBG(MIRO, LL_CTOR_DTOR, "Constructing DevConsumer.\n");
  }

  DevConsumer::~DevConsumer()
  {
    MIRO_DBG(MIRO, LL_CTOR_DTOR, "Destructing DevConsumer.\n");
  }

  void 
  DevConsumer::handleMessage(const DevMessage *)
  {
    MIRO_DBG_OSTR(MIRO,LL_NOTICE,__FILE__ << ":" << __LINE__ << ":" << __FUNCTION__  << "() - Unhandled Message!" << std::endl);
  }
}
