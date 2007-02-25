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
#ifndef Miro_Random_h
#define Miro_Random_h

#include "BuiltIn.h"
#include "Singleton.h"

#include <complex>

namespace Miro
{
  //! Lookup tables of random number distributions.
  /** Gausian distributions etc are computationally expensive to
   * calculate.  These lookup tables hold sufficiently large sequences
   * of random numbers of different distributions for use by
   * applications, that need random numbers, but can cope with some
   * determinism in their distribution.
   *
   * As this is a class for high performance computing, no locking is
   * used. The query methods are never the less thread safe in the
   * sence, that no error will occur. The worst thing that can happen
   * is, that the same random nummber is returned twice
   * successively. - Which should result in tolerable inaccuracies in
   * the nondeterminism of the distribution.
   *
   *
   */
  class Random
  {
  public:
   
    //! Initialization using current time as random seed.
    /**
     * This adds some more none-determinism to the random number
     * sequence, but makes test runs sometimes more difficult to
     * compare.
     *
     * Call before initializing the random number tables:
     * - Before the first call of Random::instance().
     * - Call init() afterwards, to reinitialize the table.
     */
    static void initRandByTime();

    //! Default constructor.
    /** Initializes the random number tables. 
     *
     * The class holds some large arrays of random numbers. So don't
     * allocate it on the stack, but use the heap. Usually you don't
     * need to worry as you probably use the global instance anyways.
     */
    Random();

    //! Return a value from a table with standard gauss distribution.
    double const& gauss01();
    //! Return a value from a table with standard 2D gauss distribution.
    std::complex<double> const& gauss2d01();
    //! Return a value from a uniform distribution table with the range [-1...1).
    double const& uniform11();
    //! Return a value from a uniform distribution table with the range [0...1).
    double const& uniform01();

    static Miro::Singleton<Random> instance;

  private:
    //! Initialize random number tables.
    /** This is called by the default constructor. So you just need to
     * call it, if you want to get a new random number distribution.
     */
    void init();

    //! Return a value from a uniform distribution in the range [0...1).
    static double random01();
    //! Return a value from a uniform distribution in the range [-1...1).
    static double random11();
    //! Return a value from a standard gauss distribution.
    double randomGauss();
    //! Return a value from a standard gauss 2D distribution.
    std::complex<double> randomGauss2d();

    //! state of the gauss number generator.
    bool iset;
    //! state of the gauss number generator.
    double gset;

    //! Index in the gauss table.
    unsigned long gaussI_;
    //! Index in the gauss 2D table.
    unsigned long gauss2dI_;
    //! Index in the uniform [0...1) table.
    unsigned long uniform01I_;
    //! Index in the uniform [-1...1) table.
    unsigned long uniform11I_;

    //! Size of the gaussian distribution lookup table.
    /** Needs to be a power of 2. */
    static long const gaussS_ = 0x10000;
    //! Size of the uniform distribution lookup table.
    /** Needs to be a power of 2. */
    static long const uniformS_ = 0x10000;

    //! Gauss table.
    double gauss_[gaussS_];
    //! 2D Gauss table.
    std::complex<double> gauss2d_[gaussS_];
    //! Uniform [0...1) table.
    double uniform01_[uniformS_];
    //! Uniform [-1...1) table.
    double uniform11_[uniformS_];
  };

  inline
  double const& 
  Random::gauss01() {
    double const& rc = gauss_[gaussI_];
    ++gaussI_ &= (gaussS_ - 1);
    MIRO_PREFETCH(&gauss_[gaussI_], 0, 0);
    return rc;
  }
  inline
  std::complex<double> const&
  Random::gauss2d01() {
    std::complex<double> const& rc = gauss2d_[gauss2dI_];
    ++gauss2dI_ &= (gaussS_ - 1);
    MIRO_PREFETCH(&gauss2d_[gauss2dI_], 0, 0);
    return rc;
  }
  inline
  double const& 
  Random::uniform01() {
    double const& rc = uniform01_[uniform01I_];
    ++uniform01I_ &= (uniformS_ - 1);
    MIRO_PREFETCH(&uniform01_[uniform01I_], 0, 0);
    return rc;
  }
  inline
  double const&
  Random::uniform11() {
    double const& rc = uniform11_[uniform11I_];
    ++uniform11I_ &= (uniformS_ - 1);
    MIRO_PREFETCH(&uniform11_[uniform11I_], 0, 0);
    return rc;
  }
}

#endif // Miro_Random_h
