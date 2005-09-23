// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MCL_WorldModel_h
#define MCL_WorldModel_h

namespace Miro
{
  class Sample;

  namespace MCL
  {
    class WorldModel
    {
    public:
      virtual ~WorldModel();
      virtual void init(Sample const& _sample) = 0;
    };
  }
}

#endif // MCL_WorldModel_h
