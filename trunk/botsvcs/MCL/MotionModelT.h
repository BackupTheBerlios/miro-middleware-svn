// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2001, 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MCL_MotionModel_h
#define MCL_MotionModel_h

namespace Miro
{
  namespace MCL
  {
    template<class S>
    class MotionModelT
    {
    public:
      //! Type of the sample to model its motion.
      typedef S Sample;

      //! Update sample position.
      void update(Sample& _sample);
      //! Blur sample position.
      void blur(Sample& _sample);
    };
  }
}

#endif // MCL_MotionModel_h
