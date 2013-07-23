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
#include "PsosDevice.h"
#include "PsosMessage.h"

namespace Psos
{
  // predefined const messages
  const Message MSG_SYNC_0(SF_SYNC_0);
  const Message MSG_SYNC_1(SF_SYNC_1);
  const Message MSG_SYNC_2(SF_SYNC_2);

  const Message MSG_COMPULSE(SF_COMPULSE);
  const Message MSG_COMOPEN(SF_COMOPEN);
  const Message MSG_COMCLOSE(SF_COMCLOSE);
  const Message MSG_COMSETO(SF_COMSETO);

  const Message MSG_COMSTEP(SF_COMSTEP);
}
