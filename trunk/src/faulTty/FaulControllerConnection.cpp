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

using std::cout;
using std::endl;

namespace FaulController
{
  Connection::Connection()
  {
    DBG(cout << "Constructing FaulControllerConnection" << endl);
  }


  Connection::~Connection()
  {
    DBG(cout << "Destructing FaulControllerConnection" << endl);
  }

  void
  Connection::writeMessage(char const * const _message)
  {

  }
};
