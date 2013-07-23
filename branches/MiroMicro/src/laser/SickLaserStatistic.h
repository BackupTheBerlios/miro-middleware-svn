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
