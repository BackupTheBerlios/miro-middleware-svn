// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "FaulControllerConnection.h"
#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace FaulController
{
  Connection::Connection() :
    binaryMode_(false)
  {
    DBG(std::cout << "Constructing FaulControllerConnection" << std::endl);

    enableBinary();
  }


  Connection::~Connection()
  {
    DBG(std::cout << "Destructing FaulControllerConnection" << std::endl);
  }

  void
  Connection::enableBinary()
  {
    // data package to enable binary commands
    char const init[4] = {200, 200, 202, 255};

    writeBinary(init, 4);
    binaryMode_ = true;
  }
}
