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
#ifndef VideoDevice_h
#define VideoDevice_h

#include "VideoFilter.h"
#include "DeviceAsynchLinkManager.h"

//#include "miro/Thread.h"
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
    //--------------------------------------------------------------------------
    // private types
    //--------------------------------------------------------------------------
    typedef Filter Super;

  public:
    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    Device(Miro::ImageFormatIDL const & _format);
    virtual ~Device();

    void enqueueBrokerRequests(BrokerRequestVector const& _request);

    //--------------------------------------------------------------------------
    // inherited interface methods
    //--------------------------------------------------------------------------

    FILTER_PARAMETERS_FACTORY(Device);

//     virtual void init(Miro::Server & _server, 
// 		      Video::FilterParameters const * _params);
    //    virtual int svc();
    //! Skip the image from processing.
    /**
     * This can become necessary due to synchronization latencies
     * between asynchronous filter trees.
     */
    virtual void skipImage();
    
    void setBrokerRequestQueue();
    //! Set flag that the connectivity of the filter graph has changed.
    /** Has to be called with Video::Filter::connectionMutex_ held! */
    void connectionChange() throw();
    //! Calcualte all currently active filters.
    /** 
     * Returns true, if all asynchronous predecessor devices have a
     * synchronized connection status. 
     */
    bool calcConnectivity();
    //! Returns the time stamp of the last connection change.
    /** Has to be called with Video::Filter::connectionMutex_ held! */
    ACE_Time_Value const& connectivityTimeStamp() const throw();
    DeviceAsynchLinkManager * deviceAsynchLinkManager() throw();

    //! Init the synchonization model with other devices.
    void synchModel(bool _synchWithCurrent, bool _synchAllNew) throw();

    //! The filters of each synchronous are numbered in the order of evaluation.
    unsigned int getNextFilterNumber() throw();

    //--------------------------------------------------------------------------
    // static methods
    //--------------------------------------------------------------------------

    static Miro::VideoPaletteIDL getPalette(std::string const & _pal);

    //--------------------------------------------------------------------------
    // static public constants
    //--------------------------------------------------------------------------

    static const unsigned int NUM_PALETTE_ENTRIES = Miro::YUV_420P + 1;

  protected:
    //--------------------------------------------------------------------------
    // protected methods
    //--------------------------------------------------------------------------

    int getPixelSize(const int) const;

    //--------------------------------------------------------------------------
    // protected types
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------
    Miro::Mutex mutex_;
    Miro::Condition condition_;

    int paletteLookup[NUM_PALETTE_ENTRIES];

    DeviceAsynchLinkManager deviceAsynchLinkManager_;
    BrokerRequestVector brokerRequest_;

    //! Flag indicating the connectivity has been changed.
    int connectionChange_;
    //! Time stamp of the last connectivity calculation.
    ACE_Time_Value connectivityTimeStamp_;
    //! Time stamp of the last connection change.
    ACE_Time_Value connectionChangeTimeStamp_;

    //! Number of attached filters.
    unsigned int numberFilters_;
  };

  inline
  void 
  Device::connectionChange() throw() {
    connectionChange_ = 2;
    connectionChangeTimeStamp_ = ACE_OS::gettimeofday();
  }
  inline
  ACE_Time_Value const& 
  Device::connectivityTimeStamp() const throw() {
    return connectivityTimeStamp_;
  }
  inline
  DeviceAsynchLinkManager *
  Device::deviceAsynchLinkManager() throw() {
    return &deviceAsynchLinkManager_;
  }

  inline
  unsigned int
  Device::getNextFilterNumber() throw() {
    ++numberFilters_;
    return numberFilters_;
  }
}
#endif
