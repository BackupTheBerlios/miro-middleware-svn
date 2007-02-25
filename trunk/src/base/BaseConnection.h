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
#ifndef BaseConnection_h
#define BaseConnection_h

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "idl/ExceptionC.h"

#include "mcp/McpDevice.h"
#include "mcp/McpConnection.h"

#include "BaseConsumer.h"
#include "BaseDevice.h"


// forward decleration
class ACE_Reactor;
namespace Mcp
{
  class EventHandler;
}

namespace Base
{
  using Mcp::OutMessage;
  using Mcp::MSG_USER_MESSAGE;

  // forward declerations
  class EventHandler;
  class Parameters;

  class Connection : public Mcp::Connection
  {
    typedef Mcp::Connection Super;

  public:
    Connection(ACE_Reactor * _reactor,
	       Mcp::EventHandler * _mcpEventHandler, 
	       Consumer * _consumer,
	       EventHandler * _eventHandler,
	       const Parameters& _parameters)
      throw(Miro::Exception, Miro::EDevIO);
    virtual ~Connection();
  
    // base commands, to be send to the hardware
    void batteryCurrent() throw(Miro::EDevIO);
    void batteryVoltage() throw(Miro::EDevIO);

    // rotation
    void rotateHalt() throw(Miro::EDevIO);
    void rotateLimp() throw(Miro::EDevIO);
    void rotateVelocityPos() throw(Miro::EDevIO);
    void rotateVelocityNeg() throw(Miro::EDevIO);
    void rotateToPosition(unsigned long position) throw(Miro::EDevIO);
    void rotateRelative(long relative) throw(Miro::EDevIO);
    void rotateTorque(long relative) throw(Miro::EDevIO);
    void rotatePower(long relative) throw(Miro::EDevIO);

    void setRotateVelocity(unsigned long velocity) throw(Miro::EDevIO);
    void setRotateFriction(unsigned long friction) throw(Miro::EDevIO);
    void setRotateSlope(unsigned long slope) throw(Miro::EDevIO);
    void setRotateTorque(unsigned long tork) throw(Miro::EDevIO);
    void setRotateZero(unsigned long zero) throw(Miro::EDevIO);
    void setRotateAcceleration(unsigned long accel) throw(Miro::EDevIO);

    void rotateCurrent() throw(Miro::EDevIO);
    void rotateWhere() throw(Miro::EDevIO);

    void translateLimp() throw(Miro::EDevIO);
    void translateHalt() throw(Miro::EDevIO);
    void translateVelocityPos() throw(Miro::EDevIO);
    void translateVelocityNeg() throw(Miro::EDevIO);
    void translateToPosition(unsigned long position) throw(Miro::EDevIO);
    void translateRelative(long relative) throw(Miro::EDevIO);
    void translateTorque(long relative) throw(Miro::EDevIO);
    void translatePower(long relative) throw(Miro::EDevIO);

    void setTranslateVelocity(unsigned long velocity) throw(Miro::EDevIO);
    void setTranslateSlope(unsigned long slope) throw(Miro::EDevIO);
    void setTranslateTorque(unsigned long torque) throw(Miro::EDevIO);
    void setTranslateZero(unsigned long zero) throw(Miro::EDevIO);
    void setTranslateAcceleration(unsigned long accel) throw(Miro::EDevIO);

    void translateCurrent() throw(Miro::EDevIO);
    void translateWhere() throw(Miro::EDevIO);

    void clock() throw(Miro::EDevIO);
    
    void limp() throw(Miro::EDevIO);
    void halt() throw(Miro::EDevIO);
    
    // the internal state of the base
    // the defaults are given as comment
    // these are the defaults of the hardware on reset
    // as documented in the B21 users Guide

    StateType rotateState;
    
    unsigned long rotateVelocity;     // 0x0080
    unsigned long rotateAcceleration; // 0x0400
 
    StateType translateState;
    
    unsigned long translateVelocity;     // 0x800 Default 100 mm/s
    unsigned long translateAcceleration; // Default 50 mm/s 0x4000

    // we have to make access to it threadsafe

    Miro::Mutex stateMutex;

    // the Consumer class needs to access the state too

    friend class Consumer;

  protected:
    EventHandler* eventHandler;
    Consumer* baseConsumer;
    
    int batteryTimerId;
  };

  //---------------------------------------------------------------------------
  // inlines
  //---------------------------------------------------------------------------

// Request base to send up value, and get it later during callback
  inline
  void 
  Connection::batteryVoltage() throw(Miro::EDevIO)
  { 
    sendCmd(&MSG_BATTERY_VOLTAGE);
  }
  inline
  void 
  Connection::batteryCurrent() throw(Miro::EDevIO)
  { 
    sendCmd(&MSG_BATTERY_CURRENT);
  }
  inline
  void 
  Connection::rotateCurrent() throw(Miro::EDevIO)
  {
    static const iovec vec[2]  = { {&MSG_ROT_CURRENT, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void
  Connection::rotateWhere() throw(Miro::EDevIO)
  {
    static const iovec vec[2] = { {&MSG_ROT_WHERE, sizeof(OutMessage)}, 
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void
  Connection::translateCurrent() throw(Miro::EDevIO)
  {
    static const iovec vec[2] = { {&MSG_TRANS_CURRENT, sizeof(OutMessage)},
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void 
  Connection::translateWhere() throw(Miro::EDevIO)
  {
    static const iovec vec[2] = { {&MSG_TRANS_WHERE, sizeof(OutMessage)}, 
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }


  //---------------------------------------------------------------------------
  // ROTATION COMMANDS
  //---------------------------------------------------------------------------
  inline
  void 
  Connection::setRotateFriction(unsigned long arg) throw(Miro::EDevIO)
  {
    OutMessage message(OP_ROT_FRICTION,arg);
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void 
  Connection::setRotateAcceleration(unsigned long arg) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    if (rotateAcceleration != arg) {
      rotateAcceleration = arg;

      OutMessage message(OP_ROT_ACCEL, arg);
      const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			    {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
      sendCommands(vec, 2);
    }
  }
  inline
  void 
  Connection::setRotateSlope(unsigned long arg) throw(Miro::EDevIO)	
  {
    OutMessage message(OP_ROT_SLOPE, arg);
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }	
  inline
  void 
  Connection::setRotateTorque(unsigned long arg) throw(Miro::EDevIO)
  {
    OutMessage message(OP_ROT_MAX_TORQUE, arg);
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void 
  Connection::setRotateZero(unsigned long arg) throw(Miro::EDevIO)
  {
    OutMessage message(OP_ROT_TRQ_ZERO, arg);
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void 
  Connection::setRotateVelocity(unsigned long arg) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    if (rotateVelocity != arg) {
      rotateVelocity = arg;

      OutMessage message(OP_ROT_VEL, arg);
      const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			    {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
      sendCommands(vec, 2);
    }
  }
  inline
  void 
  Connection::rotateToPosition(unsigned long arg) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    rotateState = TO_POSITION;

    OutMessage message(OP_ROT_POSITION, arg);
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void 
  Connection::rotateHalt() throw(Miro::EDevIO)
  {
    static const iovec vec[2] = { {&MSG_ROT_HALT, sizeof(OutMessage)}, 
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    Miro::Guard guard(stateMutex);

    rotateState = HALT;
    sendCommands(vec, 2);
    baseConsumer->rotateHaltCond.broadcast();
  }
  inline
  void 
  Connection::rotateLimp() throw(Miro::EDevIO)
  {
    static const iovec vec[2] = { {&MSG_ROT_LIMP, sizeof(OutMessage)}, 
			  {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    Miro::Guard guard(stateMutex);

    rotateState = LIMP;
    sendCommands(vec, 2);	
    baseConsumer->rotateHaltCond.broadcast();
  }
  inline
  void 
  Connection::limp() throw(Miro::EDevIO)
  {
    static const iovec vec[3] = { {&MSG_ROT_LIMP, sizeof(OutMessage)}, 
				  {&MSG_TRANS_LIMP, sizeof(OutMessage)},
				  {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    Miro::Guard guard(stateMutex);

    rotateState = LIMP;
    translateState = LIMP;
    sendCommands(vec, 3);
    baseConsumer->rotateHaltCond.broadcast();
    baseConsumer->translateHaltCond.broadcast();
  }
  inline
  void 
  Connection::halt() throw(Miro::EDevIO)
  {
    static const iovec vec[3] = { {&MSG_ROT_HALT, sizeof(OutMessage)}, 
			   {&MSG_TRANS_HALT, sizeof(OutMessage)},
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)} };

    Miro::Guard guard(stateMutex);

    rotateState = HALT;
    translateState = HALT;
    sendCommands(vec, 3);
    baseConsumer->rotateHaltCond.broadcast();
    baseConsumer->translateHaltCond.broadcast();
  }
  inline
  void
  Connection::clock() throw(Miro::EDevIO)
  { 
    sendCmd(&MSG_CLOCK); 	
  }

  inline
  void
  Connection::rotateVelocityPos() throw(Miro::EDevIO)
  {
    static iovec vec[2] = { {&MSG_ROT_VEL_POS, sizeof(OutMessage)}, 
			    {&MSG_USER_MESSAGE, sizeof(OutMessage)} };

    Miro::Guard guard(stateMutex);

    if (rotateState != CONTINUOUS_POS) {
      rotateState = CONTINUOUS_POS;
      sendCommands(vec, 2);	
    }
  }
  inline
  void
  Connection::rotateVelocityNeg() throw(Miro::EDevIO)
  {
    static iovec vec[2] = { {&MSG_ROT_VEL_NEG, sizeof(OutMessage)}, 
			    {&MSG_USER_MESSAGE, sizeof(OutMessage)} };

    Miro::Guard guard(stateMutex);

    if (rotateState != CONTINUOUS_NEG) {
      rotateState = CONTINUOUS_NEG;
      sendCommands(vec, 2);	
    }
  }
  inline
  void
  Connection::rotateRelative(long relative) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    rotateState = TO_POSITION;

    OutMessage message((relative >= 0)? OP_ROT_REL_NEG : OP_ROT_REL_POS,
			abs((int)relative));
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    sendCommands(vec, 2);	
  }
  inline
  void
  Connection::rotateTorque(long relative) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    rotateState = TORQUE;

    OutMessage message((relative >= 0)? OP_ROT_TRQ_LEFT : OP_ROT_TRQ_RIGHT,
		       abs((int)relative));
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    sendCommands(vec, 2);	
  }
  inline
  void
  Connection::rotatePower(long relative) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    rotateState = POWER;

    OutMessage message((relative >= 0)? OP_ROT_PWR_NEG : OP_ROT_PWR_POS,
		       abs((int)relative));
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    sendCommands(vec, 2);	
  }

  //--------------------------------------------------------------------------
  // TRANSLATION COMMANDS
  //--------------------------------------------------------------------------
  inline
  void 
  Connection::translateHalt() throw(Miro::EDevIO)
  {
    static const iovec vec[2] = { {&MSG_TRANS_HALT, sizeof(OutMessage)}, 
				  {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    Miro::Guard guard(stateMutex);

    translateState = HALT;
    sendCommands(vec, 2);	
    baseConsumer->translateHaltCond.broadcast();
  }
  inline
  void
  Connection::translateLimp() throw(Miro::EDevIO)
  {
    static const iovec vec[2] = { {&MSG_TRANS_LIMP, sizeof(OutMessage)}, 
				  {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    Miro::Guard guard(stateMutex);

    translateState = LIMP;
    sendCommands(vec, 2);	
    baseConsumer->translateHaltCond.broadcast();
  }
  inline
  void 
  Connection::translateToPosition(unsigned long arg) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    translateState = TO_POSITION;

    OutMessage message(OP_TRANS_POSITION, arg);
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void 
  Connection::setTranslateTorque(unsigned long arg) throw(Miro::EDevIO)
  {
    OutMessage message(OP_TRANS_MAX_TORQUE, arg);
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void 
  Connection::setTranslateZero(unsigned long arg) throw(Miro::EDevIO)
  {
    OutMessage message(OP_TRANS_TRQ_ZERO, arg);
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void 
  Connection::setTranslateSlope(unsigned long arg) throw(Miro::EDevIO)
  {
    OutMessage message(OP_TRANS_SLOPE, arg);
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)}};
    sendCommands(vec, 2);
  }
  inline
  void 
  Connection::translateVelocityPos() throw(Miro::EDevIO)
  {
    static const iovec vec[2] = { {&MSG_TRANS_VEL_POS, sizeof(OutMessage)}, 
				  {&MSG_USER_MESSAGE, sizeof(OutMessage)} };

    Miro::Guard guard(stateMutex);
    if (translateState != CONTINUOUS_POS) {
      translateState = CONTINUOUS_POS;
      sendCommands(vec, 2);	
    }
  }
  inline
  void 
  Connection::translateVelocityNeg() throw(Miro::EDevIO)
  {
    static const iovec vec[2] = { {&MSG_TRANS_VEL_NEG, sizeof(OutMessage)}, 
				  {&MSG_USER_MESSAGE, sizeof(OutMessage)} };

    Miro::Guard guard(stateMutex);

    if (translateState != CONTINUOUS_NEG) {
      translateState = CONTINUOUS_NEG;
      sendCommands(vec, 2);	
    }
  }
  inline
  void 
  Connection::translateTorque(long relative) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    translateState = TORQUE;

    OutMessage message((relative >= 0)? OP_TRANS_TRQ_RIGHT : OP_TRANS_TRQ_LEFT, 
		       abs((int) relative));
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    sendCommands(vec, 2);	
  }
  inline
  void 
  Connection::translatePower(long relative) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    translateState = POWER;
    OutMessage message((relative >= 0)? OP_TRANS_PWR_POS : OP_TRANS_PWR_NEG, 
		       abs((int) relative));
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    sendCommands(vec, 2);	
  }
  inline
  void
  Connection::translateRelative(long relative) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    translateState = TO_POSITION;
    OutMessage message((relative >= 0)? OP_TRANS_REL_POS : OP_TRANS_REL_NEG, 
		       convertMMToEncoders(abs((int) relative)));
    const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
			   {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
    sendCommands(vec, 2);	
  }
  inline
  void 
  Connection::setTranslateAcceleration(unsigned long arg) 
    throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    if (translateAcceleration != arg) {
      translateAcceleration = arg;
      OutMessage message(OP_TRANS_ACCEL, convertMMToEncoders(arg));
      const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
                             {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
      sendCommands(vec, 2);	
    }
  }
}
#endif
