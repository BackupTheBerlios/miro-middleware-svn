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
#include "NotifyMulticastDomainEventFilter.h"

namespace Miro
{
  namespace NMC {
    DomainEventFilter::DomainEventFilter(const std::string & _domain_name) :
        domain_name_(_domain_name)
    {}

    bool
    DomainEventFilter::isAccepted(const CosNotification::StructuredEvent &_event)
    {
      return (!ACE_OS::strcmp((char const *)_event.header.fixed_header.event_type.domain_name,
                              domain_name_.c_str()));
    }
  }
}

