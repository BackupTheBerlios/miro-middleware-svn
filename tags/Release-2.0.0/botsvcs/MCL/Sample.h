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
#ifndef MCL_Sample_h
#define MCL_Sample_h

namespace Miro
{
  namespace MCL
  {
    //! The base class for samples of the MCL framework.
    /**
     * This class does not contain virtual methods and this is
     * intentional.  Samples are generated, initialized and deleted in
     * large numbers, so resource efficiency is here essential to
     * ensure high performance.
     */
    class Sample
    {
    };

    //! The class template, defining the required default interface.
    /**
     * This class does not contain virtual methods and this is
     * intentional.  Samples are generated, initialized and deleted in
     * large numbers, so resource efficiency is here essential to
     * ensure high performance.
     */
    template<class T>
    class SampleT : public Sample
    {
    public:
      //! Initializing constructor.
      SampleT();
      
      //! Increment the sample age.
      void survive();
      //! Accessor method for the sample age.
      int& age();
      //! Const accessor method for sample age.
      int age() const;

    protected:
      //! The sample age.
      int age_;
    };

    /** The sample age is initialized with 0. */
    template<class T>
    inline
    SampleT<T>::SampleT() :
      age_(0)
    {}

    template<class T>
    inline 
    void
    SampleT<T>::survive() {
      ++age_;
    }
    
    template<class T>
    inline
    int
    SampleT<T>::age() const {
      return age_;
    }

    template<class T>
    inline
    int&
    SampleT<T>::age() {
      return age_;
    }
  }
}
#endif // MCL_Sample_h
