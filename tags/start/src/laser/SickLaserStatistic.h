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

#ifndef laserStatistic_hh
#define laserStatistic_hh

namespace Miro {
  class LaserStatistic {
  public:
    /**
     * number of events we had to handle
     */
    long events;

     /**
     * number of packets received in total
     */
    long packetsProcessed;

    /**
     * number of packets that had an impossible large length
     */
    long packetsToLong;

    /**
     * number of packets that where addressed to someone else
     */
    long packetsWrongAddress;

    /**
     * number of packets that had a wrong CRC
     */
    long packetsCRCError;

    /**
     * timeouts while waiting for ack 
     */
    long timeouts;

    /**
     * timestamp since when the we are counting
     */
    ACE_Time_Value startTime;

    /**
     * accumulated time spent with receiving
     */
    ACE_Time_Value recvTime;

    /**
     * reset all statistics
     */
    void reset() {
      events = 0;
      packetsProcessed = 0;
      packetsToLong = 0;
      packetsWrongAddress = 0;
      packetsCRCError = 0;
      timeouts = 0;
      startTime = ACE_OS::gettimeofday();
      recvTime = ACE_Time_Value(0,0);
    }
  };
};
#endif
