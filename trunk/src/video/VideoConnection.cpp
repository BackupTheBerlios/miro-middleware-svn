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


#include "VideoConnection.h"
#include "VideoConsumer.h"
#include "VideoDevice.h"
#include "Parameters.h"

#include "miro/Exception.h"

#include <ace/Reactor.h>

#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif

namespace Video
{
  //------------------------//
  //----- constructors -----//
  //------------------------//

  Connection::Connection(Consumer * _consumer) : 
    consumer(_consumer),
    parameters(*Parameters::instance()),
    devName(parameters.device.c_str()),
    ioBuffer(),
    connector(),
    videoDevice(parameters.grabber)
  { 
    DBG(cout << "Constructing VideoConnection." << endl);

    if (connector.connect(ioBuffer, devName, 0, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      cerr << "Failed to open device." << endl
	   << "Propably running on the wrong machine?" << endl;
      throw Miro::CException(errno, std::strerror(errno));
    }

    videoDevice.connect(ioBuffer.get_handle(), parameters.buffers, parameters.format, parameters.source,
			parameters.palette, parameters.subfield, parameters.width, parameters.height);

    consumer->open(NULL);
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//

  Connection::~Connection()
  {
    DBG(cout << "Destructing VideoConnection." << endl);
    
    consumer->cancel();
    videoDevice.disconnect();
  }

  //-------------------//
  //----- methods -----//
  //-------------------//

};
