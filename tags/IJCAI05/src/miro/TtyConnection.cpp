// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "TtyConnection.h"
#include "DevEventHandler.h"
#include "SvcParameters.h"
#include "Exception.h"
#include "miro/Log.h"

#include <ace/Reactor.h>
#include <ace/OS.h>

namespace Miro
{

  //
  // Constructors / Destructors
  //

  TtyConnection::TtyConnection(ACE_Reactor * _reactor,
			       DevEventHandler * _eventHandler,
			       const TtyParameters& _parameters) :
    ioBuffer(),
    reactor_(_reactor),
    eventHandler_(_eventHandler),
    ttyName_(_parameters.device.c_str()),
    connector_()
  {
    MIRO_DBG(MIRO, LL_NOTICE, "TtyConnection intizialising\n");

    if (connector_.connect(ioBuffer, 
			   ttyName_,
			   0, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      MIRO_LOG_OSTR(LL_CRITICAL,
		    "Failed to open device: " << _parameters.device
		    << "\nPropably running on the wrong machine?");
      throw CException(errno, std::strerror(errno));
    }

    // let's initialize the TTY 
    ioBuffer.control(ACE_TTY_IO::SETPARAMS,   
		     const_cast<ACE_TTY_IO::Serial_Params *>(&_parameters.ttyParams));

#ifdef DEBUG
#if defined (ACE_HAS_TERM_IOCTLS)
#if defined(TCGETS)
    struct termios terminfo;
    ioBuffer.ACE_IO_SAP::control(TCGETS, (void *) &terminfo);
#elif defined(TCGETA)
    struct termio terminfo;
    ioBuffer.ACE_IO_SAP::control(TCGETA, (void *) &terminfo);
#endif

#if defined(TCGETS) || defined(TCGETA)
    MIRO_DBG_OSTR(MIRO,
		  LL_PRATTLE,
		  "new tty status:" << std::endl << std::hex
		  << "c_iflag = 0x" << terminfo.c_iflag << std::endl
		  << "c_oflag = 0x" << terminfo.c_oflag << std::endl
		  << "c_cflag = 0x" << terminfo.c_cflag << std::endl
		  << "c_lflag = 0x" << terminfo.c_lflag << std::endl
		  << "c_cc[4/5] = 0x" << (int)terminfo.c_cc[VTIME] 
		  << " " << (int)terminfo.c_cc[VMIN] 
		  << std::dec << std::endl);
#endif 

#endif // ACE_HAS_TERM_IOCTLS
#endif // DEBUG
    
    // we need a handler to listen to the asynchronous file handler
    // of the device
    eventHandler_->set_handle(ioBuffer.get_handle());
    int rc =
      reactor_->register_handler(eventHandler_, ::ACE_Event_Handler::READ_MASK);
    if (rc == -1)
      throw ACE_Exception(errno, "Failed to register handler for file descriptor.");
  }

  TtyConnection::~TtyConnection()
  {
    MIRO_DBG(MIRO, LL_NOTICE, "Destructing TtyConnection\n");

    // Stop hardware triggered communication
    reactor_->remove_handler(eventHandler_,
			     ::ACE_Event_Handler::READ_MASK);
    ioBuffer.close();
  }
}
