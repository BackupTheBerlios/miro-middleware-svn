// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (Middleware for Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Miro_VIP_FilterService_h
#define Miro_VIP_FilterService_h

#include <vector>

namespace Miro
{
  namespace VIP
  {
    //! Helper class registering all available filters at the repository
    // TODO derive this from some ACE service object class or something
    class FilterService
    {
    public:
      virtual int init();
      virtual int fini();
      
    protected:
    };
  }
}
#endif // Miro_VIP_FilterService_h
