// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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




