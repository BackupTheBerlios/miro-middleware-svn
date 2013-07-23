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
//   Philipp Baer
//
#ifndef NotifyMulticastRequestIndex_h
#define NotifyMulticastRequestIndex_h

#include <tao/Version.h>

#if TAO_MAJOR_VERSION > 1 || \
    TAO_MAJOR_VERSION == 1 && TAO_MINOR_VERSION >= 4
#  include <tao/Basic_Types.h>
#else
#  include <tao/corbafwd.h>
#endif
#include <ace/INET_Addr.h>

namespace Miro
{
  namespace NMC {
    class RequestIndex
    {

    public:
      //! Default constructor.
      RequestIndex();
      //! Initializing constructor.
      RequestIndex(const ACE_INET_Addr &_from, CORBA::ULong _requestId);

      //! Return hash value.
      u_long hash() const;
      //! Comparison operator.
      bool operator==(RequestIndex const &rhs) const;
      //! Comparison operator.
      bool operator!=(RequestIndex const &rhs) const;

    private:
      ACE_INET_Addr from_;
      CORBA::ULong  requestId_;
    };

    inline
    RequestIndex::RequestIndex() :
        requestId_(0)
    {}

    inline
    RequestIndex::RequestIndex(const ACE_INET_Addr &_from,
                               CORBA::ULong _requestId) :
        from_(_from),
        requestId_(_requestId)
    {}

    inline
    u_long
    RequestIndex::hash() const
    {
      return ((from_.get_ip_address() << 24)
              | (from_.get_port_number() << 8)
              | (requestId_ & 0x000000ff));
    }

    inline
    bool
    RequestIndex::operator==(RequestIndex const &rhs) const
    {
      return (this->from_ == rhs.from_ &&
              this->requestId_ == rhs.requestId_);
    }

    inline
    bool
    RequestIndex::operator!=(RequestIndex const &rhs) const
    {
      return !(*this == rhs);
    }
  }
}

#endif // NotifyMulticastRequestIndex_h
