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
#include "idl/LocalizationC.h"

#include "miro/Log.h"
#include "miro/TimeHelper.h"
#include "miro/BuiltIn.h"
#include <iostream>

#include <algorithm>
#include <numeric>

#include <vector>

using namespace std;

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
    {
        nCountSamples_ = 0;
		bestSample_ = Sample(0,0,0);
    }

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

/*!
\fn int ParticleFilter<S, F, M, O, H, C, E>::statistics()
\return LocalizeStatisticsIDL & 
\brief Localization statistic
*/
    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    LocalizeStatisticsIDL & 
    ParticleFilter<S, F, M, O, H, C, E>::statistics()
    {
      return statistics_;
    }

/*!
\fn int ParticleFilter<S, F, M, O, H, C, E>::clear()
\return 
\brief clear the samples_ vector
*/
    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    void
    ParticleFilter<S, F, M, O, H, C, E>::clear() {
      samples_.clear();
    }

/*!
\fn void ParticleFilter<S, F, M, O, H, C, E>::setParaSamples(int nSamples)
\param nSamples		number of samples to set 
\return 
\brief from the parameter file LocalizationParameters.xml
*/
    template<class S, class F, class M, class O, class H, bool C, bool E>
    inline
    void
    ParticleFilter<S, F, M, O, H, C, E>::setParaSamples(int nSamples) {
      nSampleCount_ = nSamples;
    }

/*!
\fn void ParticleFilter<S, F, M, O, H, C, E>::add(unsigned int _num, double nX, double nY, double nAngle, bool lRandom, const Sample& p) 
\param _num			Number of samples to add
\param nX			x-position on the field
\param nY			y-position on the field 
\param nAngle		angle from the sample
\param lRandom		random type
\param p			Sample
\return 			nothing
\brief This template add a number of samples 

This template contains the function to delete Samples on the Field
to delete a number of samples you can change the percentage in the paramter file LocalizeParameters.xml
*/
    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::add(unsigned int _num, double nX, double nY, double nAngle, const Sample& p) 
	{
		int nTemp = nCountSamples_ / 2;
		_num = nCountSamples_ - nTemp;
		NIX_DBG_OSTR(LOCALIZE, LL_DEBUG, "RANDOM BEST: " << nTemp );
		
		for (unsigned int i = 0; i < nTemp; ++i) 
		{
			// add Samples with randXY and rand...
			samples_.push_back(sampleFactory_.getBestGuess(true,p));
		}
		
		NIX_DBG_OSTR(LOCALIZE, LL_DEBUG, "RANDOM: " << _num);
	  for (unsigned int i = 0; i < _num; ++i) 
	  {

		Sample s(nX, nY, nAngle );
		
		samples_.push_back(sampleFactory_.getGauss(s, 300, Miro::deg2Rad(5)));
      }

    }

/*!
\fn int ParticleFilter<S, F, M, O, H, C, E>:: add(Sample const& _sample)
\param _sample		sample we want to add
\return void
\brief This template add a sample to the samples_ vector
*/
    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::add(Sample const& _sample) {
      samples_.push_back(_sample);
    }
	
/*!
\fn int ParticleFilter<S, F, M, O, H, C, E>::delSamples(int nPercentage, bool lOwnGoalSeen, bool lOppGoalSeen) 
\param nPercentage	Number of samples to delete in percentage
\param lOwnGoalSeen true -> I can see my own goal otherwise false
\param lOppGoalSeen true -> I can see the opponent goale, otherwise false
\return returns the number of deleted samples 
\brief This template delete a number of samples 

This template contains the function to delete Samples on the Field
to delete a number of samples you can change the percentage in the paramter file LocalizeParameters.xml
*/
	template<class S, class F, class M, class O, class H, bool C, bool E>
	int 
	ParticleFilter<S, F, M, O, H, C, E>::delSamples(int nPercentage, bool lOwnGoalSeen, bool lOppGoalSeen) 
	{
		typename SampleVector::iterator nCount = samples_.begin();

	 

		std::vector<int> histo;
		std::vector<int>::iterator run = histo.begin();
		histo.assign(20,0);

		DoubleVector::iterator scoreI = sampleScore_.begin();

		unsigned int nCountSample = 0;

		int nSample = samples_.size();

		if(nSample <= 0)
			return 0;

		int nDelSamples = (int)(nSample / 100) * nPercentage;

		NIX_DBG_OSTR(LOCALIZE, LL_DEBUG, "Zu loeschende Samples: " << nDelSamples);
		NIX_DBG_OSTR(LOCALIZE, LL_DEBUG, "Zu setzende Samples: " << nSampleCount_);

		sampleScore_.resize(samples_.size());

		DoubleVector::iterator j = sampleScore_.begin();

		for(scoreI; scoreI != sampleScore_.end(); ++scoreI)
		{
			if(*scoreI <= 0.05)
				histo[0]++;
			else if(*scoreI <= 0.10)
				histo[1]++;
			else if(*scoreI <= 0.15)
				histo[2]++;
			else if(*scoreI <= 0.20)
				histo[3]++;
			else if(*scoreI <= 0.25)
				histo[4]++;
			else if(*scoreI <= 0.30)
				histo[5]++;
			else if(*scoreI <= 0.35)
				histo[6]++;
			else if(*scoreI <= 0.40)
				histo[7]++;
			else if(*scoreI <= 0.45)
				histo[8]++;
			else if(*scoreI <= 0.50)
				histo[9]++;
			else if(*scoreI <= 0.55)
				histo[10]++;
			else if(*scoreI <= 0.60)
				histo[11]++;
			else if(*scoreI <= 0.65)
				histo[12]++;
			else if(*scoreI <= 0.70)
				histo[13]++;
			else if(*scoreI <= 0.75)
				histo[14]++;
			else if(*scoreI <= 0.80)
				histo[15]++;
			else if(*scoreI <= 0.85)
				histo[16]++;
			else if(*scoreI <= 0.90)
				histo[17]++;
			else if(*scoreI <= 0.95)
				histo[18]++;
			else if(*scoreI <= 1.00)
				histo[19]++;
		}


		NIX_DBG_OSTR(LOCALIZE, LL_DEBUG, "------------------------- HISTOGRAMM --------------------------" );

		for(int j = 0; j < 4 ; j++)
		{

			NIX_DBG_OSTR(LOCALIZE, LL_DEBUG, (double)1/20*(j+1) << " ->  "  << histo[j] << "\t" <<
											 (double)1/20*(j+5) << " ->  "  << histo[j+4] << "\t" <<
											 (double)1/20*(j+9) << " ->  "  << histo[j+8] << "\t" <<
											 (double)1/20*(j+13) << " ->  "  << histo[j+12] << "\t" <<
											 (double)1/20*(j+17) << " ->  "  << histo[j+16] );

		}

		int nTemp = 0;
		double nGrenze = 0.00;
		for(int i = 0; i<= 19; ++i, nGrenze += 0.05)
		{
			if(histo[i] < nDelSamples)
			{
				if(nDelSamples <= nTemp + histo[i])
					nTemp += histo[i];
				else
				{
					nGrenze += 0.05;
					break;
				}
			}
			else
			{
				sampleScore_.resize(samples_.size());
				break; 
			}
		}

		if(!lOwnGoalSeen && !lOppGoalSeen)
		{
			for(j; j != sampleScore_.end(); j++,nCount++) 
			{
				if(*j <= nGrenze && nDelSamples > 0 )
				{
					samples_.erase(nCount);
					nDelSamples--;
				}
			}
		}
		else
		{
///\todo Loeschen der Samples im erlaubten Winkel, wenn ein Tor erkannt wurde ! 
/*			
			for(nCount; nCount != samples_.end(); nCount++) 
			{
				if(*nCount[2] <= nGrenze && nDelSamples > 0 )
				{
					samples_.erase(nCount);
					nDelSamples--;
				}
			}
*/
		}

		bestSample_ = samples_.at(samples_.size()-1);
		nCountSamples_ = nDelSamples;

		if(nDelSamples == 0)
			{
				int nSize = samples_.size();
			}
		return nCountSamples_;
	}
	
   template<class S, class F, class M, class O, class H, bool C, bool E>
   void
   ParticleFilter<S, F, M, O, H, C, E>::setSelect() 
	{
          select(true);
   }

/*!
\fn void ParticleFilter<S, F, M, O, H, C, E>::setSamples(int nSamples, double nX, double nY, double nAngle, bool lRandom) 
\param nSamples number of samples to set
\param nX		x-position of the Sample
\param nY		y-position of the Sample
\param nAngle   angle of the Sample
\param lRandom  random Type: true -> surround the best sample otherwise random on the Field
\return void
\brief add new samples to the field, half of the deleted samples around the best samples and the rest random on the field

*/
    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::setSamples(int nSamples, double nX, double nY, double nAngle) 
	{

	if(bestSample_.x() == 0 || bestSample_.y() == 0 || bestSample_.direction() == 0)
	{
		bestSample_ = sampleFactory_.getBestGuess(true,	bestSample_);
	}

        add(nCountSamples_, nX, nY, nAngle, bestSample_);

  	}

/*
    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::newReSample(int nSize) 
	{
		SampleVector initialPositions;
				for( int ia = 0; ia <= 360; ia += 10 )
				{
					for( int ix = 0; ix <= 11000; ix += 500 ) 
					{
						for( int iy = 0; iy <= 6000; iy += 500 ) 
						{
							initialPositions.push_back(Sample(ix-5500, iy-3000, Miro::deg2Rad(ia)));
						}
					}
				}
		
			typename SampleVector::const_iterator first, last = initialPositions.end();
			for (first = initialPositions.begin(); first != last; ++first) 
			{
				Sample s = *first;
						
				s = sampleFactory_.getBestGuess(false, s);
				NIX_DBG_OSTR(LOCALIZE, LL_DEBUG, "NEW SAMPLES ADDED!!!  => RESAMPLE");	

				add(s);

			}
		
		}
*/
	
    /** Performing a complete iteration of the localization process.
     * Returns the new postion hypothesis, the variance and the number of samples,
     * that belong the selected cluster.
     */
    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::process() 
	{
 		update();		// update the position and the score of each sample
      
  		select(true);		// Select the new sample set by choosing old ones according to their weights.
      	
 		//unsigned int num = hypothesis_(samples_, _hypothesis, _var);
      	
// 		unsigned int num = 10;
    }

    template<class S, class F, class M, class O, class H, bool C, bool E>
    unsigned int
    ParticleFilter<S, F, M, O, H, C, E>::process(Sample& _hypothesis, double& _var) {

      // update of hypthesis wheights
      if (evalStatistics())
	updating_.start();
      update();
      if (evalStatistics()) {
	updating_.done();
	Miro::timeA2C(updating_.last(), statistics_.updateTime);
      }

      // selecting samples
      if (evalStatistics())
	selecting_.start();
      select();
      if (evalStatistics()) {
	selecting_.done();
	Miro::timeA2C(selecting_.last(), statistics_.selectTime);
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
	Miro::timeA2C(choosing_.last(), statistics_.hypothesisTime);
      }
      return num;
    }
	
    /** update the position of every sample according to the motion model and update
		the score of each sample*/

    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::update() {
      typename SampleVector::iterator i, iEnd = samples_.end();

      sampleScore_.resize(samples_.size());
      DoubleVector::iterator scoreI = sampleScore_.begin();

	  ACE_Time_Value startUpdateTime = ACE_OS::gettimeofday();
      // for each sample
      for(i = samples_.begin(); i != iEnd; ++i, ++scoreI) 
	  {
		MIRO_PREFETCH(i.base() + 1, 1, 3);
	
		// update the position hypothesis according to the motion model
		
		motion_.update(*i);

		// match the observation model with the hypothesis
		*scoreI = observation_.match(*i);
		
	  }
		//cout << "MOTION UPDATE TIME: " << (ACE_OS::gettimeofday()- startUpdateTime) << endl;

    }

    /** Select the new sample set by choosing old ones according to their weights. */
    template<class S, class F, class M, class O, class H, bool C, bool E>
    void
    ParticleFilter<S, F, M, O, H, C, E>::select(bool oldValues=false) 
	{
		
	if(oldValues)
    {
		// calculate partial sum
      	unsigned int size = samples_.size();
      	partialSum_.resize(size);
      	partial_sum(sampleScore_.begin(), sampleScore_.end(), partialSum_.begin());
      	SampleVector newSamples;
      	newSamples.reserve(size);

      	// total sum 
      	double s = partialSum_.back();
	
	NIX_DBG_OSTR(LOCALIZE, LL_DEBUG, "Partial Sum from Samples: " << s);	
      	// statistic counters
      	double mean = 0.;
      	//int compCount = 0;
      	//int newCount = 0;

      	double weightSum = 0.0;
      	//    while (weightSum < samplesWeight_) {

		//More samples on the field
		//pb[]
		// ToDo Samples set to parameter file
      	double step = (double)s / nSampleCount_;

		double w = step / 2.;
      	DoubleVector::iterator l = partialSum_.begin();
		
      	for (int i = nSampleCount_; i > 0; --i, w += step)
		{

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

			// Use the 'old' samples 
			newSamples.push_back(samples_[index]);
			newSamples.back().survive();
			//	motion_.blur(newSamples.back());

			motion_.blur(newSamples.back());
			newScore = std::min(1.0, newScore);
		
					
			mean += newScore;
			weightSum += std::max(minWeight_, newScore);
      	}

      	samples_.swap(newSamples);

		Sample p;

      	if (resample_ && !competitiveResampling_)
			for (int i = 0; i < 10; ++i)
	  			samples_.push_back(sampleFactory_.getBestGuess(true,p));

      	MIRO_DBG_OSTR(MCL, LL_DEBUG, "mean after draw: " << mean / (double) samples_.size());
	}
	else
	{
		if (evalStatistics()) 
		{
			// Sample wheight distribution
			for (unsigned int i = 0; i < 10; ++i) 
			{
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
		for (int i = 500; i > 0; --i, w += step) 
		{
	
		// non-randomized resampling
		// this version does not proper randomized drawing of samples,
		// but still ensures a propper wheigted resampling.
		// This can impose some theoretical problems, but in practice it
		// works nice and is much, much faster.
		while( *l < w)
		{
		++l;
		}
	
		unsigned int index = l - partialSum_.begin();
	
		MIRO_ASSERT(index < samples_.size());
	
		double newScore = sampleScore_[index];
	
		// competitive resampling:
			if (competitiveResampling_ &&	resample_ &&	newScore < sampleFactory_.guessingLevel()) 
			{
				++compCount;
				newSamples.push_back(sampleFactory_.getBestGuess());
				newScore = sampleFactory_.guessingLevel() * 
					observation_.match(newSamples.back());
				if (newScore <= sampleScore_[index]) 
				{
					newScore = sampleScore_[index];
					newSamples.back() = samples_[index];
					newSamples.back().survive();
					//	  motion_.blur(newSamples.back());
				}
				else
					++newCount;
			}
			// sample survival
			else 
			{
				newSamples.push_back(samples_[index]);
				newSamples.back().survive();
				//	motion_.blur(newSamples.back());
			}		
		
			motion_.blur(newSamples.back());
			newScore = std::min(1.0, newScore);
		
			// Sample wheight distribution
			if (evalStatistics())
			{
				++statistics_.histogram[(unsigned int)floor((newScore) * 10.)];
			}
		
			mean += newScore;
			weightSum += std::max(minWeight_, newScore);
		}
	
		if (evalStatistics()) 
		{
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
			{
				statistics_.variance += (*first) * (*first);
			}

			statistics_.variance -= ((double) size) * statistics_.mean * statistics_.mean;
			statistics_.variance /= ((double) (size - 1));
		}
	
		samples_.swap(newSamples);
	
		if (resample_ && !competitiveResampling_)
		{
			for (int i = 0; i < 10; ++i)
			{
				samples_.push_back(sampleFactory_.getBestGuess());
			}
		}
	
		MIRO_DBG_OSTR(MCL, LL_DEBUG,
				"mean after draw: " << mean / (double) samples_.size());
		}


    }
  }
}
