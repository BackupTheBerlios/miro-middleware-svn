// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef laserDescription_hh
#define laserDescription_hh

#include "miro/TtyParameters.h"
#include "miro/ScanDescriptionC.h"

#include <string>

namespace Laser
{
  class Parameters;

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);

  class Parameters : public Miro::TtyParameters
  {
    typedef Miro::TtyParameters Super;

  public:
    /**
     * true for any usual tty, that produces the baudrates it is programmed to
     * (it is a 14.something MHz crystal)
     * false for card with crystal of 16.0MHz, like ours
     * 500000 baud are only possible with a 16.0MHz crystal installed
     */
    bool             stdcrystal;
    /**
     * this is the number of microseconds we want to pass between two
     * consecutive polls from the laser
     */
    int             pollintervall;

    /**
     * the sick laser scanner provides a continous mode, in that it
     * sends about 40 scans per second to the host. This provides the fastest
     * access to data, but may provide problems when the FIFO of the receiver is too
     * small. set this to true for using it, false uses active polling of laserscans 
     * every 1/10th of a second
     */
    bool             continousmode;


    bool notify;
    bool positionStamps;
    bool statistics;

    /** this contains the physical description of the laser sensor,
     * i.e. installtion and other characteristics needed for a
     * adaptive, interpretation, that can handle different range
     * sensors uniformly
     * */
    Miro::ScanDescriptionIDL laserDescription;

    Parameters();
    virtual void operator <<= (const QDomNode& node);

    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };

};

#endif
