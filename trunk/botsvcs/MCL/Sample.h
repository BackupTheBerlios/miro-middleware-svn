// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
