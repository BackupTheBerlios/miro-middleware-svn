// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Miro_Configuration
#define Miro_Configuration

#include "Exception.h"
#include "Singleton.h"
#include "ConfigDocument.h"

namespace Miro
{
  struct Configuration
  {
    static void init(int& argc, char * argv[]) throw (Exception);
    static Singleton<ConfigDocument> document;
  };
}

#endif // Miro_Configuration
