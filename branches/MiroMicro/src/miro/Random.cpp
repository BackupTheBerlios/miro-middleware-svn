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
#include "Random.h"

#include <stdlib.h>

#include <ace/OS.h>

namespace Miro
{
  Singleton<Random> Random::instance;

  void
  Random::initRandByTime()
  {
    srand(ACE_OS::gettimeofday().usec());
  }

  Random::Random() :
      iset(false),
      gset(0.),
      gaussI_(0),
      gauss2dI_(0),
      uniform01I_(0),
      uniform11I_(0)
  {
    init();
  }

  void
  Random::init()
  {
    double * iEnd = gauss_ + gaussS_;
    for (double * i = gauss_; i != iEnd; ++i)
      *i = randomGauss();
    std::complex<double> * jEnd = gauss2d_ + gaussS_;
    for (std::complex<double> * j = gauss2d_; j != jEnd; ++j)
      *j = randomGauss2d();
    iEnd = uniform01_ + uniformS_;
    for (double * i = uniform01_; i != iEnd; ++i)
      *i = random01();
    iEnd = uniform11_ + uniformS_;
    for (double * i = uniform11_; i != iEnd; ++i)
      *i = random11();
  }

  double
  Random::random01()
  {
    return (double)rand() / (RAND_MAX + 1.0);
  }

  double
  Random::random11()
  {
    return (double)rand() / ((RAND_MAX + 1.0) * .5) - 1.;
  }

  double
  Random::randomGauss()
  {
    double fac, rsq, v1, v2;
    if (!iset) {
      do {
        v1 = random11();
        v2 = random11();
        rsq = v1 * v1 + v2 * v2;
      }
      while (rsq >= 1.0 || rsq == 0.0);

      fac = sqrt(-2.0 * log(rsq) / rsq);
      gset = v1 * fac;
      iset = true;
      return v2 * fac;
    }
    else {
      iset = false;
      return gset;
    }
  }

  std::complex<double>
  Random::randomGauss2d()
  {
    double r = randomGauss();
    double a = 2. * M_PI * random01();

    return std::complex<double>(r * cos(a), r * sin(a));
  }
}
