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
#ifndef McpPerformance_h
#define McpPerformance_h

#include "miro/ReactorTask.h"

#include "base/BaseImpl.h"

class McpPerformance
{
  typedef McpPerformance Self;

public:
  // Initialization and Termination methods.
  McpPerformance(int& , char **);
  // Constructor.

  ~McpPerformance();
  // Destructor.

  // since TAO is using the main thread, we need our own reactor
  // actually its worse: Due to deadlocks otherwise, we need a concurrent
  // reactor!
  Miro::ReactorTask reactorTask;

  // B21Motion interface
  Base::Impl b21Motion;
};
#endif




