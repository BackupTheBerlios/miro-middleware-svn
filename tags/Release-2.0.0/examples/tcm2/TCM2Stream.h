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
#ifndef TCM2Stream_h
#define TCM2Stream_h

#include "miro/StructuredPushConsumer.h"

#include <string>

class TCM2Stream : public Miro::StructuredPushConsumer
{
  typedef Miro::StructuredPushConsumer Super;

public:
  /**
   *Initialization
   *
   * Connect the Consumer to the EventChannel.
   * Creates a new proxy supplier and connects to it.
   *
   * Registers for the events, that it wants to get pushed.
   */
  TCM2Stream(CosNotifyChannelAdmin::EventChannel_ptr _ec,
	     const std::string& domainName);
  /** Disconnect from the supplier. */
  virtual ~TCM2Stream();

  /** inherited IDL interface: StructuredPushSupplier methods */
  virtual void push_structured_event(const CosNotification::StructuredEvent & notification
				     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    throw(CORBA::SystemException, CosEventComm::Disconnected);
};

#endif
