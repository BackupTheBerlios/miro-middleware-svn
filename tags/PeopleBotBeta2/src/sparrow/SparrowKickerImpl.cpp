// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "SparrowKickerImpl.h"
#include "SparrowConnection.h"

#include "miro/Exception.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Sparrow
{
  KickerImpl::KickerImpl(Connection& _connection) :
    connection(_connection)
  {
    DBG(cout << "Constructing SparrowKickerImpl" << endl);
  }

  KickerImpl::~KickerImpl()
  {
    DBG(cout << "Destructing SparrowKickerImpl" << endl);
  }

  // 
  // IDL interface implementation

  //--------------------------------------------------------------------------
  // Kicker interface implementation
  //--------------------------------------------------------------------------

  void
  KickerImpl::kick(const Miro::TimeIDL& time) throw()
  {
    connection.kick(time.sec * 1000 + time.usec / 1000);
  }
};
