// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include <ace/Reactor.h>

#include "miro/TtyConnection.h"
#include "miro/DevEventHandler.h"
#include "miro/TtyParameters.h"

#include "miro/Exception.h"

// #undef DEBUG

#ifdef DEBUG
#include <iostream>

using std::cout;
using std::cerr;
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Miro
{
  //
  // Constructors / Destructors
  //

  TtyConnection::TtyConnection(ACE_Reactor * _reactor,
			       DevEventHandler * _eventHandler,
			       const TtyParameters& _parameters) :
    reactor(_reactor),
    eventHandler(_eventHandler),
    ttyName(_parameters.device.c_str()),
    ioBuffer(),
    connector(),
    selectHandlerId(-1)
  {
    DBG(cout << "TtyConnection intizialising" << endl);

    if (connector.connect(ioBuffer, ttyName, 0, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      cerr << "Failed to open device." << endl
	   << "Propably running on the wrong machine?" << endl;
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
    cout << "new tty status:" << endl << hex
	 << "c_iflag = 0x" << terminfo.c_iflag << endl
	 << "c_oflag = 0x" << terminfo.c_oflag << endl
	 << "c_cflag = 0x" << terminfo.c_cflag << endl
	 << "c_lflag = 0x" << terminfo.c_lflag << endl
	 << "c_cc[4/5] = 0x" << (int)terminfo.c_cc[VTIME] << " " << (int)terminfo.c_cc[VMIN] 
	 << dec << endl;
#endif 

#endif // ACE_HAS_TERM_IOCTLS
#endif // DEBUG
    
    // we need a handler to listen to the asynchronous file handler
    // of the device
    selectHandlerId = 
      reactor->register_handler(ioBuffer.get_handle(),
				eventHandler, ::ACE_Event_Handler::READ_MASK);
    if (selectHandlerId == -1)
      throw ACE_Exception(errno, "Failed to register handler for file descriptor.");
  }

  TtyConnection::~TtyConnection()
  {
    DBG(cout << "Destructing TtyConnection" << endl);

    // Stop hardware triggered communication
    if (selectHandlerId != -1)
      reactor->remove_handler(selectHandlerId);
    ioBuffer.close();
  }
};
