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
  Connection::Connection()
  {
    DBG(std::cout << "Constructing FaulControllerConnection" << endl);
  }


  Connection::~Connection()
  {
    DBG(std::cout << "Destructing FaulControllerConnection" << endl);
  }
};
