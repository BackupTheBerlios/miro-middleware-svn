// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoDevice_h
#define VideoDevice_h

#include "VideoFilter.h"

#include "miro/Thread.h"
#include "miro/Synch.h"

// forward declarations
class ACE_Time_Value;

namespace Video
{
  //! Base class of all video devices.
  /**
   * 
   */
  class Device : public Filter
  {
    typedef Filter Super;

  public:
    Device(Miro::ImageFormatIDL const & _format);
    virtual ~Device();

    void enqueueBrokerRequest(BrokerRequest const& _request);

    //--------------------------------------------------------------------------
    // inherited interface methods

    FILTER_PARAMETERS_FACTORY(Device);

    //    virtual void init(const Video::FilterParameters *);
    //    virtual int svc();
    
    //--------------------------------------------------------------------------
    // static methods

    static Miro::VideoPaletteIDL getPalette(std::string const & _pal);

    //--------------------------------------------------------------------------
    // static public constants

    static const unsigned int NUM_PALETTE_ENTRIES = Miro::YUV_420P + 1;

  protected:
    //--------------------------------------------------------------------------
    // protected methods
    int getPixelSize(const int) const;

    //--------------------------------------------------------------------------
    // protected inherited methods
    virtual void setBrokerRequests();

    //--------------------------------------------------------------------------
    // protected types
    
    //--------------------------------------------------------------------------
    // protected data
    Miro::Mutex mutex_;
    Miro::Condition condition_;

    int paletteLookup[NUM_PALETTE_ENTRIES];

    BrokerRequestVector brokerRequest_;
  };
};
#endif
