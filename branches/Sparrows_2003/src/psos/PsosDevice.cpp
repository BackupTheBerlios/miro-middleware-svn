// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
};
