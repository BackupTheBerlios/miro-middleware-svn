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

#include "FeatureManager.h"
#include "Feature.h"
#include "Sample.h"
#include "WorldModel.h"

#include <algorithm>

namespace Miro
{
  namespace MCL
  {
    ObservationModelT<MultiFeature>::ObservationModelT() :
      worldModel_(NULL)
    {}

    ////////////////////////////////////////////////////////////////////////////
    // administration
    ////////////////////////////////////////////////////////////////////////////

    ObservationModelT<MultiFeature>::FeatureVector
    ObservationModelT<MultiFeature>::features() const
    {
      Miro::Guard guard(mutex_);
      return features_; 
    }

    void
    ObservationModelT<MultiFeature>::addFeature(Feature * _feature)
    {
      Miro::Guard guard(mutex_);
      features_.push_back(_feature);
    }

    void
    ObservationModelT<MultiFeature>::delFeature(Feature * _feature)
    {
      Miro::Guard guard(mutex_);

      while (true) {
	FeatureVector::iterator j = 
	  std::find(features_.begin(), features_.end(), _feature);
	if (j != features_.end())
	  features_.erase(j);
	else 
	  break;
      }
    }

    unsigned int 
    ObservationModelT<MultiFeature>::selectFeatures() 
    {
      Miro::Guard guard(mutex_);

      currentFeatures_.clear();

      FeatureVector::const_iterator first, last = features_.end();
      for(first = features_.begin(); first != last; ++first) {
	if ((*first)->validData())
	  currentFeatures_.push_back(*first);
      }

      return currentFeatures_.size();
    }

    double
    ObservationModelT<MultiFeature>::match(Sample const& _sample) 
    {
      double score = 1.;
      FeatureVector::iterator first, last = currentFeatures_.end();

      // init the world model
      worldModel_->init(_sample);

#define NIX_USES_BAYES
#ifdef NIX_USES_BAYES
      for(first = currentFeatures_.begin(); first != last; ++first) {
	double s = (*first)->evaluate(_sample);
	score *= s;
      }
      return 1. - 1. / (1. + score);
#else
      for(first = currentFeatures_.begin(); first != last; ++first) {
	score *= (*first)->evaluate(_sample);
      }
      return score;
#endif // NIX_USES_BAYES
    }

    ////////////////////////////////////////////////////////////////////////////
    // statistic evaluation
    ////////////////////////////////////////////////////////////////////////////

    void
    ObservationModelT<MultiFeature>::statistics(std::ostream& _ostr) const
    {
      if (Feature::logStatistics) {
	FeatureVector::const_iterator first, last = currentFeatures_.end();
	for(first = currentFeatures_.begin(); first != last; ++first) {
	  (*first)->statistics(_ostr);
	}
      }
    }

    void
    ObservationModelT<MultiFeature>::printLast(std::ostream& _ostr) const
    {
      FeatureVector::const_iterator first, last = currentFeatures_.end();
      for(first = currentFeatures_.begin(); first != last; ++first) {
	(*first)->printLast(_ostr);
      }
    }

    bool
    ObservationModelT<MultiFeature>::isAdded(Feature const * _feature) const {
      return (std::find(features_.begin(), features_.end(), _feature) != features_.end());
    }
  }
}
