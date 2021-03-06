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
#ifndef MCL_ParticleFilter_h
#define MCL_ParticleFilter_h

#include "idl/LocalizationC.h"

#include "miro/TimeSeries.h"
#include "miro/TimeHelper.h"

#include <vector>

namespace Miro 
{
  //! Namespace of Monte Carlo localization framework.
  /**
   * This namespace contains classes used by the Monte Carlo
   * localization framework.  It implements a particle filter based
   * marcov localization model, that can naturally handle ambiguity in
   * localization hypotheses.
   */
  namespace MCL
  {
    //! The main class of the framework.
    /**
     * The particle filter class is extensively templated to allow for
     * a flexible but efficient implementation.
     */
    template<class S, class F, class M, class O, class H, bool C = true, bool E = true>
    class ParticleFilter
    {
    public:
      //! Sample type.
      typedef S Sample;
      //! A factory class that can generate new samples.
      typedef F SampleFactory;
      //! Type denoting the motion model.
      typedef M MotionModel;
      //! Type denoting the observation model.
      typedef O ObservationModel;
      //! Type denoting a localization hypothesis.
      typedef H Hypothesis;

      //! Default constructor.
      ParticleFilter();

      //! Returns reference to the sample factory.
      SampleFactory& sampleFactory();
      //! Returns reference to the world model.
      MotionModel& motionModel();
      //! Returns reference to the observatin model.
      ObservationModel& observation();
      //! Returns the current localization hypothesis.
      Hypothesis& hypothesis();

      //! Returns const reference to the sample factory.
      SampleFactory const& sampleFactory() const;
      //! Returns const reference to the world model.
      MotionModel const& motionModel() const;
      //! Returns const reference to the observatin model.
      ObservationModel const& observation() const;
      //! Returns const the current localization hypothesis.
      Hypothesis const& hypothesis() const;

      void minWeight(double _minWeight);
      double minWeight() const;
      void samplesWeight(double _samplesWeight);
      double samplesWeight() const;

      //! Clears the sample set.
      void clear();
      //! Adds a specified sample to the current sample set.
      void add(Sample const& _sample);
      //! Adds a number of samples to the current sample set.
      void add(unsigned int _num, double nX, double nY, double nAngle, const Sample& p, int field_length, int width_length );

      //! Adds a specified sample to the current sample set.
//      void add(Sample const& _sample);

      void resampling(bool _flag);
      
	  void process();

	  unsigned int process(Sample& _hypothesis, double& _var);
      //! Returns the number of samples in the set.
      unsigned int samples() const { return samples_.size(); }
	  
      bool evalStatistics() const;
      LocalizeStatisticsIDL & statistics();
      LocalizeStatisticsIDL const & statistics() const;

    protected:
      void update();
      void select(bool);

      typedef typename std::vector<double> DoubleVector;
      typedef typename std::vector<Sample> SampleVector;

      //! Instance of the sample factory.
      SampleFactory sampleFactory_;
      //! Instance of the motion model.
      MotionModel motion_;
      //! Instance of the observation model.
      ObservationModel observation_;
      //! The latest localization hypothesis.
      Hypothesis hypothesis_;
      bool resample_;

    public:
      //! The sample set.
      SampleVector samples_;

	  //! Method to delete a count of Samples 
	  //! the first parameter says how many can we destroy in percentage
	  //!  the second parameter tells us if we can see the goal !
      int delSamples(int,bool lOwnGoalSeen, bool lOppGoalSeen, double nCurrentAngleRobot);

	  //! Sets new Samples with random coord. and angle !
	  //! but if we can see the goal, only the position is random 
	  //! in front of the robot!
      void setSamples(int nSamples, double nX, double nY, double nAngle, int field_length, int width_length);

	  //void newReSample(int);

      void setSelect();

	  //! contains the parameters from LocalizationParameters.xml
	  void setParaSamples(int);

      unsigned int nCountSamples_ ;

	  //! Number of Samples
	  int nSampleCount_ ;

	  //! best Sample on the Field
      Sample bestSample_ ;
		
	  Sample fSample_;

	  //!contains true if the goal was seen otherwise false
	  bool lSeenGoal_ ;

      //! The score of the individual sample.
      DoubleVector sampleScore_;
    protected:

      //! Partial sum vector for resampling.
      DoubleVector partialSum_;

      double samplesWeight_;
      double minWeight_;

      //------------------------------------------------------------------------
      // statistic data
      //------------------------------------------------------------------------

      //! The maximum sample score.
      double maxScore_;
      //! The maximum scoring sample.
      typename SampleVector::const_iterator maxSample_;

      //! Statistical measures of the particle filter performance.
      LocalizeStatisticsIDL statistics_;

      //! Timing of the sample update.
      Miro::TimeSeries<10> updating_;
      //! Timing of sample selection.
      Miro::TimeSeries<10> selecting_;
      //! Timing of position hypothesis.
      Miro::TimeSeries<10> choosing_;

      //------------------------------------------------------------------------
      // statistics parameters
      //------------------------------------------------------------------------

      //! Enable statistical analysis of particle filter performance.
      static bool const evalStatistics_ = E;
      //! Enable competitive resampling mode.
      static bool const competitiveResampling_ = C;
    };
  }
}

#include "ParticleFilterT.cpp"

#endif // ParticleFilter_hh
