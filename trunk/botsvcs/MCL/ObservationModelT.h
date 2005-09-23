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
#ifndef MCL_ObservationModelT_h
#define MCL_ObservationModelT_h

namespace Miro
{
  namespace MCL
  {
    // forward declaration
    class Sample;

    //! Base template for observation models.
    /** Contains all necessary method definitions, that need to be
     * implemented by a specialization of that class.
     */
    template<class T>
    class ObservationModelT
    {
    public:
      //! Type specification for specialization.
      typedef T ModelType;

      //! Template inherited method
      double match(Sample const & _sample);
    };
  }
}

#endif // MCL_ObservationModelT_h
