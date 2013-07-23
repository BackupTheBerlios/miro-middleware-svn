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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
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
