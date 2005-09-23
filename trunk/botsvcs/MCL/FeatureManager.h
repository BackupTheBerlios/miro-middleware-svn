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
#ifndef MCL_FeatureManager_h
#define MCL_FeatureManager_h

#include "ObservationModelT.h"

#include <miro/Synch.h>

#include <vector>

namespace Miro
{
  namespace MCL
  {
    // forward declaration
    class Feature;
    class WorldModel;

    class MultiFeature {};

    template<>
    class ObservationModelT<MultiFeature>
    {
    public:
      //--------------------------------------------------------------------------
      // public types
      //--------------------------------------------------------------------------

      //! A vector of feature evaluators.
      typedef std::vector<Feature*> FeatureVector;

      //--------------------------------------------------------------------------
      // public methods
      //--------------------------------------------------------------------------

      //! Default constructor.
      ObservationModelT();
  
      //! Template inherited method
      double match(Sample const & _sample);

      void init(WorldModel * _worldModel);

      // administration

      //! Add a feature evaluator to the manager.
      /** Thread safe for use by Localize interface. */
      void addFeature(Feature * _feature);
      //! Delete a feature evaluator from the manager.
      /**
       * Thread safe for use by Localize interface.
       *
       * Warning: The removed feature may not be deleted,
       * as long as it is within the currentFeature_ list.
       * This can be forced by calling selectFeatures, while
       * no localization iteration is in progress.
       */
      void delFeature(Feature * _feature);
      //! Select a subset of feature evaluators.
      /**
       * The selection is based on the method Feature::valid().
       * This way feature evaluators that can not work due to 
       * currently unavailable data (unobservable features), can
       * be excluded from the evaluation process.
       *
       * Thread safe due to interactin of Localize interface.
       *
       * @return The number of used feature evaluators.
       */
      unsigned int selectFeatures();

      //! Test if a feature is in use by the manager.
      bool isAdded(Feature const * _feature) const;
    
      // statistical evaluation

      //! Print statistic summary.
      void statistics(std::ostream& _ostr) const;
      //! Print statistic summary of the last feature.
      void printLast(std::ostream& _ostr) const;
    
      //! Accessor method to the feature vector.
      /** Thread safe for use by Localize interface. */
      FeatureVector features() const;

    protected:
      //--------------------------------------------------------------------------
      // protected data
      //--------------------------------------------------------------------------

      //! The common static world model for all feature evaluators.
      WorldModel * worldModel_;

      //! Synchronization with extern interface.
      mutable Miro::Mutex mutex_;
      //! Vector of available features.
      FeatureVector features_;
      //! Vector of currently used features.
      FeatureVector currentFeatures_;
    };

    typedef ObservationModelT<MultiFeature> FeatureManager;

    inline
    void
    ObservationModelT<MultiFeature>::init(WorldModel * _worldModel)
    {
      worldModel_ = _worldModel;
    }

  }
}

#endif // MCL_FeatureManager_h
