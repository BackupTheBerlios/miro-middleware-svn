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


#include "miro/Task.h"

#undef DEBUG

namespace Miro
{
  Miro::Task::Task() : 
    super() 
  {}
  Miro::Task::~Task() 
  {}

  int
  Miro::Task::open(void *)
  {
    // Activate the task
    return activate();
  }
};
