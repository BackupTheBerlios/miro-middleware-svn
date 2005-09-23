// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/LocalizeC.h"
#include "nix/PerformanceLog.hh"

#include "miro/Log.h"
#include "miro/TimeHelper.h"
#include "miro/BuiltIn.h"

#include <algorithm>
#include <numeric>

namespace Miro
{
  namespace MCL
  {
    template<class S, class F, class M, class O, class H, bool C, bool E>
    ParticleFilter<S, F, M, O, H, C, E>::ParticleFilter() :
      samplesWeight_(100.),
      minWeight_(0.01),
      updating_("updating samples"),
      selecting_("selecting samples"),
      choosing_("chosing hypothesis")
    {}

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    typename ParticleFilter<S, F, M, O, H, C, E>::SampleFactory& 
    ParticleFilter<S, F, M, O, H, C, E>::sampleFactory() {
      return sampleFactory_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    typename ParticleFilter<S, F, M, O, H, C, E>::MotionModel& 
    ParticleFilter<S, F, M, O, H, C, E>::motionModel() {
      return motion_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    typename ParticleFilter<S, F, M, O, H, C, E>::ObservationModel& 
    ParticleFilter<S, F, M, O, H, C, E>::observation() {
      return observation_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    typename ParticleFilter<S, F, M, O, H, C, E>::Hypothesis& 
    ParticleFilter<S, F, M, O, H, C, E>::hypothesis() {
      return hypothesis_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    typename ParticleFilter<S, F, M, O, H, C, E>::SampleFactory const& 
    ParticleFilter<S, F, M, O, H, C, E>::sampleFactory()  const{
      return sampleFactory_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    typename ParticleFilter<S, F, M, O, H, C, E>::MotionModel const& 
    ParticleFilter<S, F, M, O, H, C, E>::motionModel() const {
      return motion_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    typename ParticleFilter<S, F, M, O, H, C, E>::ObservationModel const& 
    ParticleFilter<S, F, M, O, H, C, E>::observation() const {
      return observation_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    typename ParticleFilter<S, F, M, O, H, C, E>::Hypothesis const& 
    ParticleFilter<S, F, M, O, H, C, E>::hypothesis() const {
      return hypothesis_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    void
    ParticleFilter<S, F, M, O, H, C, E>::samplesWeight(double _samplesWeight) {
      samplesWeight_ = _samplesWeight;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    double
    ParticleFilter<S, F, M, O, H, C, E>::samplesWeight() const {
      return samplesWeight_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    void
    ParticleFilter<S, F, M, O, H, C, E>::resampling(bool _flag) {
      resample_ = _flag;
    }


    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    void
    ParticleFilter<S, F, M, O, H, C, E>::minWeight(double _minWeight) {
      minWeight_ = _minWeight;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    double
    ParticleFilter<S, F, M, O, H, C, E>::minWeight() const {
      return minWeight_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline 
    bool
    ParticleFilter<S, F, M, O, H, C, E>::evalStatistics() const {
      return evalStatistics_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    LocalizeStatisticsIDL const & 
    ParticleFilter<S, F, M, O, H, C, E>::statistics() const
    {
      return statistics_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    LocalizeStatisticsIDL & 
    ParticleFilter<S, F, M, O, H, C, E>::statistics()
    {
      return statistics_;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    void
    ParticleFilter<S, F, M, O, H, C, E>::clear() {
      samples_.clear();
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::add(unsigned int _num) {
      for (unsigned int i = 0; i < _num; ++i) {
	samples_.push_back(sampleFactory_.getBestGuess());
      }
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::add(Sample const& _sample) {
      samples_.push_back(_sample);
    }

    /** Performing a complete iteration of the localization process.
     * Returns the new postion hypothesis, the variance and the number of samples,
     * that belong the selected cluster.
     */
    template<class S, class F, class M, class O, class H, bool C, bool E>
    unsigned int
    ParticleFilter<S, F, M, O, H, C, E>::process(Sample& _hypothesis, double& _var) {

      // update of hypthesis wheights
      if (evalStatistics())
	updating_.start();
      update();
      if (evalStatistics()) {
	updating_.done();
	//	Miro::timeA2C(updating.mean(), statistics_.updateTime);
      }

      // selecting samples
      if (evalStatistics())
	selecting_.start();
      select();
      if (evalStatistics()) {
	selecting_.done();
	//	Miro::timeA2C(selecting.mean(), statistics_.selectTime);
      }

      // choosing new position hypthesis
      if (evalStatistics())
	choosing_.start();
      unsigned int num = hypothesis_(samples_, _hypothesis, _var);
      if (evalStatistics()) {
	statistics_.hypoSamples = num;
	statistics_.hypoVariance = _var;
      }    
      if (evalStatistics()) {
	choosing_.done();
	//	Miro::timeA2C(choosing.mean(), statistics_.hypothesisTime);
      }
      return num;
    }

    /** Updating the score of the individual samples.*/
    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::update() {
      typename SampleVector::iterator i, iEnd = samples_.end();

      sampleScore_.resize(samples_.size());
      DoubleVector::iterator scoreI = sampleScore_.begin();

      // for each sample
      for(i = samples_.begin(); i != iEnd; ++i, ++scoreI) {
	MIRO_PREFETCH(i.base() + 1, 1, 3);

	// update the position hypothesis according to the motion model
	motion_.update(*i);
	// match the observation model with the hypothesis
	*scoreI = observation_.match(*i);

	// statistic book keeping
	if (*scoreI > maxScore_) {
	  maxScore_ = *scoreI;
	  maxSample_ = i;
	}
      }
    }

    /** Select the new sample set by choosing old ones according to their weights. */
    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::select() {

      if (evalStatistics()) {
	// Sample wheight distribution
	for (unsigned int i = 0; i < 10; ++i) {
	  statistics_.histogram[i] = 0;
	}
      }
      // calculate partial sum
      unsigned int size = samples_.size();
      partialSum_.resize(size);
      partial_sum(sampleScore_.begin(), sampleScore_.end(), partialSum_.begin());
      SampleVector newSamples;
      newSamples.reserve(size);

      // total sum 
      double s = partialSum_.back();

      // statistic counters
      double mean = 0.;
      int compCount = 0;
      int newCount = 0;

      double weightSum = 0.0;
      //    while (weightSum < samplesWeight_) {

      double step = s / 500.;
      double w = step / 2.;
      DoubleVector::iterator l = partialSum_.begin();
      for (int i = 500; i > 0; --i, w += step) {

	// non-randomized resampling
	// this version does not proper randomized drawing of samples,
	// but still ensures a propper wheigted resampling.
	// This can impose some theoretical problems, but in practice it
	// works nice and is much, much faster.
	while( *l < w)
	  ++l;

	unsigned int index = l - partialSum_.begin();

	MIRO_ASSERT(index < samples_.size());

	double newScore = sampleScore_[index];

	// competitive resampling:
	if (competitiveResampling_ &&
	    resample_ &&
	    newScore < sampleFactory_.guessingLevel()) {

	  ++compCount;
	  newSamples.push_back(sampleFactory_.getBestGuess());
	  newScore = sampleFactory_.guessingLevel() * 
	    observation_.match(newSamples.back());
	  if (newScore <= sampleScore_[index]) {
	    newScore = sampleScore_[index];
	    newSamples.back() = samples_[index];
	    newSamples.back().survive();
	    //	  motion_.blur(newSamples.back());
	  }
	  else
	    ++newCount;
	}
	// sample survival
	else {
	  newSamples.push_back(samples_[index]);
	  newSamples.back().survive();
	  //	motion_.blur(newSamples.back());
	}

	motion_.blur(newSamples.back());
	newScore = std::min(1.0, newScore);

	// Sample wheight distribution
	if (evalStatistics())
	  ++statistics_.histogram[(unsigned int)floor((newScore) * 10.)];

	mean += newScore;
	weightSum += std::max(minWeight_, newScore);
      }

      if (evalStatistics()) {
	// Resampling statistics
	statistics_.maxSampleSetSize = (CORBA::ULong) (samplesWeight_ / minWeight_);
	statistics_.oldSampleSet = samples_.size();
	statistics_.newSampleSet = newSamples.size();
	statistics_.competitiveSamples = compCount;
	statistics_.newSamples = newCount;
      
	// Sample wheight statistics
	statistics_.min = *min_element(sampleScore_.begin(), sampleScore_.end());
	statistics_.max = *max_element(sampleScore_.begin(), sampleScore_.end());
	statistics_.mean = s / (double) size;
	statistics_.variance = 0.;
	DoubleVector::const_iterator first, last = sampleScore_.end();
	for (first = sampleScore_.begin(); first != last; ++first)
	  statistics_.variance += (*first) * (*first);
	statistics_.variance -= ((double) size) * statistics_.mean * statistics_.mean;
	statistics_.variance /= ((double) (size - 1));
      }

      samples_.swap(newSamples);

      if (resample_ && !competitiveResampling_)
	for (int i = 0; i < 10; ++i)
	  samples_.push_back(sampleFactory_.getBestGuess());

      MIRO_DBG_OSTR(MCL, LL_DEBUG,
		    "mean after draw: " << mean / (double) samples_.size());
    }
  }
}
