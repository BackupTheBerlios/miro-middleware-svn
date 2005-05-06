// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast RequestIndex
//
//
//  (c) 2002
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <phbaer@openums.org>
//
//
//  Version:
//    1.0.0
//
//
//  Description:
//
//    ... to be written ...
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////
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
  namespace NMC 
  {
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
    RequestIndex::hash() const {
      return ((from_.get_ip_address() << 24)
	      | (from_.get_port_number () << 8)
	      | (requestId_ & 0x000000ff));
    }

    inline 
    bool 
    RequestIndex::operator==(RequestIndex const &rhs) const {
      return (this->from_ == rhs.from_ &&
	      this->requestId_ == rhs.requestId_);
    }

    inline
    bool 
    RequestIndex::operator!=(RequestIndex const &rhs) const {
      return !(*this == rhs);
    }
  }
}

#endif // NotifyMulticastRequestIndex_h
