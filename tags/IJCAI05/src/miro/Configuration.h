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
  //! Class holding static methods for initializing and accessing the configuration.
  struct Configuration
  {
    //! Initializing the document.
    /** 
     * Loading the document from default location if not overridden
     * by command line parameter -MiroConfigFile (-MCF).
     */
    static void init(int& argc, char * argv[]) throw (Exception);
    //! Singleton accesor for the robots configuation document.
    static Singleton<ConfigDocument> document;
  };
}

#endif // Miro_Configuration
