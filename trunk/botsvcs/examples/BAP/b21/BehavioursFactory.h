// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef B21_BehavioursFactory_h
#define B21_BehavioursFactory_h

#include "botsvcs/BAP/BAFactory.h"

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class Server;
};

class B21BehavioursFactory : public Miro::BAP::BAFactory
{
  typedef Miro::BAP::BAFactory Super;

public:
  // Initialization and Termination methods.
  B21BehavioursFactory(Miro::Server& _server, ACE_Reactor& _reactor);
  // Constructor.
};
#endif




