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
#ifndef B21BaseDummy_h
#define B21BaseDummy_h

#include "miro/Server.h"
#include "base/BaseDummyImpl.h"

class B21BaseDummy : public Miro::Server
{
  typedef Miro::Server super;
  typedef B21BaseDummy self;

public:
  // Initialization and Termination methods.
  B21BaseDummy(int argc, char *argv[]);
  // Constructor.

  ~B21BaseDummy();
  // Destructor

private:
  Base::Dummy base;
  // base connection
};
#endif




