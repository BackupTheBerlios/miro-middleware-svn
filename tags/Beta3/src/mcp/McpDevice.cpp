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


#include "McpDevice.h"

namespace Mcp
{
  OutMessage MSG_SYNCH(OP_SYNCH, 0);
  OutMessage MSG_VERSION_REPORT(OP_VERSION_REPORT, 0);
  OutMessage MSG_KILL(OP_KILL, 0);
  OutMessage MSG_SHUTDOWN(OP_SHUTDOWN, 0);
  OutMessage MSG_WAIT_EVENT(OP_DELAY, 0);
  OutMessage MSG_DELAY(OP_USER_MESSAGE, 0);
  OutMessage MSG_USER_MESSAGE(OP_USER_MESSAGE, 0xa0);
  OutMessage MSG_DIRECT_MODE(OP_ERROR_DELAY, 0);
  OutMessage MSG_ERROR_DELAY(OP_ERROR_ACKNOWLEDGE, 0);
  OutMessage MSG_ERROR_ACKNOWLEDGE(OP_ERROR_ACKNOWLEDGE, 0);
  OutMessage MSG_FULL_DUPLEX(OP_FULL_DUPLEX, 0);
  OutMessage MSG_HALF_DUPLEX(OP_HALF_DUPLEX, 0);
  OutMessage MSG_JOYSTICK_DISABLE(OP_JOYSTICK_DISABLE, 0);
  OutMessage MSG_STATUS_REPORT(OP_STATUS_REPORT, 0);
  OutMessage MSG_PSR_DATA(OP_PSR_DATA, 0);
  OutMessage MSG_PSR_PERIOD(OP_PSR_PERIOD, 0);
  OutMessage MSG_LOCATE_ROT_INDEX(OP_LOCATE_ROT_INDEX, 0);
  OutMessage MSG_LOAD_POSITION(OP_LOAD_POSITION, 0);
  OutMessage MSG_LOAD_HEADING(OP_LOAD_HEADING, 0);
  OutMessage MSG_WATCH_DOG(OP_WATCH_DOG, 0);
  OutMessage MSG_ADJUST_HEADING(OP_ADJUST_HEADING, 0);
};
