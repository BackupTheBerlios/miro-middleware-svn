// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoFilterOmni2Pan_h
#define VideoFilterOmni2Pan_h

/*! \file VideoFilterOmni2Pan.h
 *  \brief Image conversion routines for omnidirectional camera
 *  \author Roland Reichle
 */

#include "VideoFilter.h"

namespace Video
{
  //! Lookup table driven conversion from omni to panoramic view
  /** The center and the radius of the circle that should be "unrolled" can be
      assigned. In case, that parts of the circle are outside of the image, these pixels
      are filled with the value of the nearest pixel inside the image (relativ to the
      center of the circle). */
  class FilterOmni2Pan : public Filter
  {
    typedef Filter Super;
    
  public:
    FilterOmni2Pan(Miro::ImageFormatIDL const& _inputFormat);
    virtual ~FilterOmni2Pan();
    virtual void process();
    virtual void init(Miro::Server& _server, Video::FilterParameters const *);

    FILTER_PARAMETERS_FACTORY(FilterOmni2Pan);

    static unsigned int const IMAGE_WIDTH = 360;
    static unsigned int const IMAGE_HEIGHT = 120;

  protected:
    void buildLookupTables();

    int middleX_;
    int middleY_;
    int radius_;

    int srcOffset_[IMAGE_WIDTH * IMAGE_HEIGHT];

    static double cosLookup_[360];
    static double sinLookup_[360];

  };
}

#endif // VideoFilterOmni2Pan_h

