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


#include "NotifyMulticastParameters.h"
#include "Angle.h"

#include <qdom.h>
#include <iostream>

using std::ostream;
using std::string;

namespace Miro {

    namespace NotifyMulticast {

	Singleton<Parameters> Parameters::instance;

	Parameters::Parameters() :
	    Super(),
	    subscription(),
            messagetimeout(500),  // ms
	    multicastgroup("225.2.2.1:41006")
	{
	    cout << "NotifyMulticast: Parameters initialized" << endl;
	}

	void Parameters::operator <<= (const QDomNode& node) {
	    if (!node.isNull()) {
		QDomNode n1 = node.firstChild();
		while(!n1.isNull() ) {
		    if (n1.nodeName() == "subscription") {
			QDomNode n2 = n1.firstChild();
			QDomText t = n2.toText(); // try to convert the node to a text
                        if (!t.isNull())
			    subscription.insert((const char *)t.data());
		    }
		    else if (n1.nodeName() == "messagetimeout") {
			QDomNode n2 = n1.firstChild();
			QDomText t = n2.toText(); // try to convert the node to a text
			if(!t.isNull())
                            messagetimeout = t.data().toInt();
		    }
		    else if (n1.nodeName() == "multicastgroup") {
			QDomNode n2 = n1.firstChild();
			QDomText t = n2.toText(); // try to convert the node to a text
			if(!t.isNull())
                            multicastgroup.set((const char *)t.data());
		    }
		    n1 = n1.nextSibling();
		}
	    }
	}

        void Parameters::printToStream(std::ostream &ostr) const {
	    int i = 0;
	    ACE_TCHAR buffer[255];

	    multicastgroup.addr_to_string(buffer, 255);

	    for (Subscription::iterator itr = subscription.begin(); itr != subscription.end(); itr++)
		ostr << "subscription[" << i++ << "]=" << *itr << endl;
	    ostr << "messagetimeout=" << messagetimeout << endl
		<< "multicastgroup=" << buffer << endl;
	}

        ostream& operator << (ostream& ostr, const Parameters& params) {
	    params.printToStream(ostr);
            return ostr;
	}
    };
};
