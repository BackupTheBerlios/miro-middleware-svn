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




