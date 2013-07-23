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
