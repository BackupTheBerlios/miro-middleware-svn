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
#ifndef PanTiltConsumer_h
#define PanTiltConsumer_h

#include "miro/DevConsumer.h"

#include "PanTiltData.h"
#include "PanTiltMessage.h"

namespace DpPanTilt 
{
  /**
   * handles complete packages as assembled by panTiltEvent
   * gets the packages from its message queue, where 
   * panTiltEvent puts them.
   * keeps statistics about corrupted packages
   * uses the reference to PanTiltImpl to signal the data
   * received via condition variables
   */
  class Consumer : public Miro::DevConsumer
  {
  public:
    /**
     * Constructor
     */
    Consumer(Data &panTiltData_);

    /**
     * PanTilt message interface: the asynchronously called handler
     * does the (hardware specific decoding of the message
     */ 
    virtual void handleMessage(const Miro::DevMessage * message);

  protected:
    /**
     * internal function that updates the statistics, 
     * if argument is true, a corrupted packet is counted, else 
     * an uncorrupted packet is counted
     */
    void incStatistics( bool corrupted );

    /**
     * reference to the PanTiltImpl class, for signaling data
     * via condition variables
     */
    Data&       panTiltData;
    /**
     * number of packets received in total
     */
    long packetsProcessed;
    /**
     * number of packets that had a wrong CRC
     */
    long packetsCorrupted;
  };
}
#endif




