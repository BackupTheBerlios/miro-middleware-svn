// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MCL_SampleFactoryT_h
#define MCL_SampleFactoryT_h

namespace Miro
{
  namespace MCL
  {
    //! Factory class for the generation of new position hypothesis.
    template<class W, class S>
    class SampleFactoryT
    {
    public:
      //! Type specification for specialization.
      typedef W WorldModel;
      //! Type of the sample to factorize.
      typedef S Sample;

      //! The level of knowledge for the guessing of samples.
      /**
       * The guessing level is used in conjunction with
       * competitive resampling to choose whether to compare
       * a redrawn sample with a newly drawn one.
       *
       * This method returns a value in the range [0...1].  If the
       * wheigt of a sample from the sample set is below this value
       * and competitive resampling is enabled, a new sample will be
       * drawn from the sample factory using getBestGuess() and
       * wheighted. If this new sample has a heigher weight as the
       * first one, it will be selected instead of the one drawn from
       * the sample set.
       *
       * So a value of 0. basically disables competitive resampling
       * and a value of 1. enforces every sample of the sample set to
       * compete with a sample from the sample factory.
       * 
       * The guessing level is expected to be adjusted before each
       * iteratio of the particle filter, accoring to the localization
       * features available.
       */
      double guessingLevel() const;
      
      //! Draw sample based on the provided sensory information.
      Sample getBestGuess() const;
    };
  }
}

#endif // MCL_SampleFactoryT_h
