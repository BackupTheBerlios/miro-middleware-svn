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
#ifndef VideoConnection_h
#define VideoConnection_h

#include <ace/DEV_Connector.h>

#include "VideoDevice.h"

//------------------------ VideoClass ---------------------------//

namespace Video
{
  // forward declaration
  class Consumer;
  class Parameters;

  /**
   * Class for using the Video robot
   */
  class Connection
  {
  public:
    Connection(Consumer * _consumer,
	       const Parameters& _parameters);
    virtual ~Connection();

    //------------------------------------------------------------------------
    // methods to hardware
    //------------------------------------------------------------------------

  protected:
  	friend class Consumer;

    void open(int);
    void close(int);
    //	?????? int close(int);

	Consumer * consumer;

    const Parameters& parameters;

    ACE_DEV_Addr devName;
    ACE_DEV_IO ioBuffer;
    ACE_DEV_Connector connector;

	VideoDevice videoDevice;
  };
};

#endif

