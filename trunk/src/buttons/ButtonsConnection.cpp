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


#include "ButtonsConnection.h"
#include "ButtonsEventHandler.h"
#include "ButtonsDevice.h"

#include "idl/ExceptionC.h"
#include "miro/Exception.h"

#include <ace/Reactor.h>

// #undef DEBUG

#ifdef DEBUG
#include <iostream>
using std::cout;
using std::cerr;
#define DBG(x) x
#else
#define DBG(x)
#endif

using Miro::CException;
using Miro::ACE_Exception;
using Miro::EDevIO;

namespace B21Buttons
{
  //
  // Constructors / Destructors
  //

  Connection::Connection(ACE_Reactor * _reactor,
			 EventHandler * _pEventHandler)
    throw(Miro::CException) :
    reactor(_reactor),
    pEventHandler(_pEventHandler),
    // open file descriptor
    fileName("/dev/port"),
    ioBuffer(),
    connector(),
    // internal status
    buttonLights(0x00),
    // timer handler
    pollingTimerId(-1),
    blinkingTimerId(-1)
  {
    DBG(cout << "Constructing B21Buttons::Connection" << endl);

    if (connector.connect(ioBuffer, fileName, 0, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      cerr << "C Error " << errno << ": " << std::strerror(errno) << endl;
      throw CException(errno, std::strerror(errno));
    }

    // reset buttons
    setButtonLights(LEFT_KILL_BIT | RIGHT_KILL_BIT);
    
    // init timers
    
#ifdef  DEBUG
    cout << "Polling timer interval: " << POLLING_TIME << endl;
    cout << "Blinking timer interval: " << BLINKING_TIME << endl;
#endif // DEBUG

    pollingTimerId = 
      reactor->schedule_timer(pEventHandler, 
			      (void *)POLLING, // timer id
			      POLLING_TIME,  // delay
			      POLLING_TIME); // respawn every timeout sec.
    if (pollingTimerId == -1)
      throw ACE_Exception(errno, "Failed to register timer for polling.");
    
    blinkingTimerId = 
      reactor->schedule_timer(pEventHandler, 
			      (void *)BLINKING, // timer id
			      BLINKING_TIME,    // delay
			      BLINKING_TIME);   // respawn every timeout sec.
    if (blinkingTimerId == -1)
      throw ACE_Exception(errno, "Failed to register timer for blinking.");
  }
  
  Connection::~Connection()
  {
    DBG(cout << "Destructing B21Buttons::Connection" << endl);

    // clear buttons
    setButtonLights(0x00);

    // Stop timers
    if (blinkingTimerId != -1)
      reactor->cancel_timer(blinkingTimerId);
    if (pollingTimerId != -1)
      reactor->cancel_timer(pollingTimerId);

    delete pEventHandler;

    ioBuffer.close();
  } 
  
  unsigned char 
  Connection::getButtonStatus() throw(EDevIO)
  {
    unsigned char c;
    ioBuffer.seek(PRESSED_ADDR, SEEK_SET);
    if (ioBuffer.recv_n(&c, 1) == -1) {
      DBG(cout << "C Error " << errno << ": " << strerror(errno) << endl);
      throw EDevIO();
    }
    return (c >> 4) ^ 0x08;  // the upper 4 bits hold the button status
  }

  void 
  Connection::setButtonLights(unsigned char value) throw(EDevIO) 
  {
    if (value != buttonLights) {
      unsigned char inBits;

      buttonLights = value;

      ioBuffer.seek(LIGHTS_ADDR, SEEK_SET);
      if (ioBuffer.recv_n(&inBits, 1) == -1) {
	DBG(cerr << "C Error " << errno << ": " << strerror(errno) << endl);
	throw EDevIO();
      }

      value |= (inBits & 0x80);

      ioBuffer.seek(LIGHTS_ADDR, SEEK_SET);
      if (ioBuffer.send_n(&value, 1) == -1) {
	cerr << "C Error " << errno << ": " << std::strerror(errno) << endl;
	throw EDevIO();
      }
    }
  }
};
