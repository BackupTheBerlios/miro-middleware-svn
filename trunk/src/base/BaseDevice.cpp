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
#include "BaseDevice.h"
#include "mcp/McpMessage.h"

namespace Base
{
  OutMessage MSG_TRANS_VEL(OP_TRANS_VEL, 0);
  OutMessage MSG_TRANS_ACCEL(OP_TRANS_ACCEL, 0);
  OutMessage MSG_TRANS_HALT(OP_TRANS_HALT, 0);
  OutMessage MSG_TRANS_LIMP(OP_TRANS_LIMP, 0);
  OutMessage MSG_TRANS_REL_POS(OP_TRANS_REL_POS, 0);
  OutMessage MSG_TRANS_REL_NEG(OP_TRANS_REL_NEG, 0);
  OutMessage MSG_TRANS_VEL_POS(OP_TRANS_VEL_POS, 0);
  OutMessage MSG_TRANS_VEL_NEG(OP_TRANS_VEL_NEG, 0);
  OutMessage MSG_TRANS_TRQ_LEFT(OP_TRANS_TRQ_LEFT, 0);
  OutMessage MSG_TRANS_TRQ_RIGHT(OP_TRANS_TRQ_RIGHT, 0);
  OutMessage MSG_TRANS_PWR_POS(OP_TRANS_PWR_POS, 0);
  OutMessage MSG_TRANS_PWR_NEG(OP_TRANS_PWR_NEG, 0);
  OutMessage MSG_TRANS_CL_RANGE(OP_TRANS_CL_RANGE, 0);
  OutMessage MSG_TRANS_MAX_TORQUE(OP_TRANS_MAX_TORQUE, 0);
  OutMessage MSG_TRANS_SLOPE(OP_TRANS_SLOPE, 0);
  OutMessage MSG_TRANS_FRICTION(OP_TRANS_FRICTION, 0);
  OutMessage MSG_TRANS_TRQ_ZERO(OP_TRANS_TRQ_ZERO, 0);
  OutMessage MSG_TRANS_CURRENT(OP_TRANS_CURRENT, 0);
  OutMessage MSG_TRANS_POSITION(OP_TRANS_POSITION, 0);
  OutMessage MSG_TRANS_WHERE(OP_TRANS_WHERE, 0);

  OutMessage MSG_ROT_VEL(OP_ROT_VEL, 0);
  OutMessage MSG_ROT_ACCEL(OP_ROT_ACCEL, 0);
  OutMessage MSG_ROT_HALT(OP_ROT_HALT, 0);
  OutMessage MSG_ROT_LIMP(OP_ROT_LIMP, 0);
  OutMessage MSG_ROT_REL_POS(OP_ROT_REL_POS, 0);
  OutMessage MSG_ROT_REL_NEG(OP_ROT_REL_NEG, 0);
  OutMessage MSG_ROT_VEL_POS(OP_ROT_VEL_POS, 0);
  OutMessage MSG_ROT_VEL_NEG(OP_ROT_VEL_NEG, 0);
  OutMessage MSG_ROT_TRQ_LEFT(OP_ROT_TRQ_LEFT, 0);
  OutMessage MSG_ROT_TRQ_RIGHT(OP_ROT_TRQ_RIGHT, 0);
  OutMessage MSG_ROT_PWR_POS(OP_ROT_PWR_POS, 0);
  OutMessage MSG_ROT_PWR_NEG(OP_ROT_PWR_NEG, 0);
  OutMessage MSG_ROT_CL_RANGE(OP_ROT_CL_RANGE, 0);
  OutMessage MSG_ROT_MAX_TORQUE(OP_ROT_MAX_TORQUE, 0);
  OutMessage MSG_ROT_SLOPE(OP_ROT_SLOPE, 0);
  OutMessage MSG_ROT_FRICTION(OP_ROT_FRICTION, 0);
  OutMessage MSG_ROT_TRQ_ZERO(OP_ROT_TRQ_ZERO, 0);
  OutMessage MSG_ROT_CURRENT(OP_ROT_CURRENT, 0);
  OutMessage MSG_ROT_POSITION(OP_ROT_POSITION, 0);
  OutMessage MSG_ROT_WHERE(OP_ROT_WHERE, 0);

  OutMessage MSG_BATTERY_VOLTAGE(OP_BATTERY_VOLTAGE, 0);
  OutMessage MSG_BATTERY_CURRENT(OP_BATTERY_CURRENT, 0);

  OutMessage MSG_CLOCK(OP_CLOCK, 0);
  
  // undocumented
  OutMessage MSG_INDEX_REPORT(OP_INDEX_REPORT, 0);

};
