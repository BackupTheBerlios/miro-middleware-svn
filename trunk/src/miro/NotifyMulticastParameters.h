// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors:
//   Philipp Baer
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef NotifyMulticastParameters_h
#define NotifyMulticastParameters_h

#include "Parameters.h"
#include "Singleton.h"
#include <ace/INET_Addr.h>
#include <set>
#include <string>

namespace Miro {

    namespace NotifyMulticast {

	class Parameters : public Miro::Parameters {
	    typedef Miro::Parameters Super;

	public:
	    typedef std::set<std::string> Subscription;

	    Subscription  subscription;
            unsigned int  messagetimeout;
	    ACE_INET_Addr multicastgroup;

	    Parameters();
	    virtual void operator <<= (const QDomNode& node);

	   void printToStream(std::ostream &ostr) const;

	    static Singleton<Parameters> instance;
	};

	std::ostream&
	    operator << (std::ostream& ostr, const Parameters& desc);
    };
};

#endif /* notifyMulticastParameters_hh */
 
