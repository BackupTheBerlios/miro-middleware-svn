// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "WindowArbiterParameters.h"
#include "Behaviour.h"
#include "BehaviourRepository.h"

#include <qdom.h>

#include <iostream>

namespace Miro
{
//   void
//   WindowArbiterParameters::operator <<= (const QDomNode& _node)
//   {
//     int priority = 0;

//     Super::operator <<= (_node);

//     if (!_node.isNull()) {
//       QDomNode n1 = _node.firstChild();
//       while(!n1.isNull() ) {
// 	QDomNode n2 = n1.firstChild();
// 	if (!n2.isNull()) {
// 	  QDomText t = n2.toText(); // try to convert the node to a text
// 	  if(!t.isNull() ) {        // the node was really a text element.
// 	    if (n1.nodeName() == "behaviour") {
// 	      Behaviour * behaviour = 
// 		BehaviourRepository::instance()->getBehaviour(std::string(t.data()));
// 	      if (behaviour) {
// 		priorities[behaviour] = priority;
// 		++priority;
// 	      }
// 	      else {
// 		cerr << "WindowArbiterParameters: unknown behaviour " 
// 		     << t.data() << endl; 
// 	      }
// 	    }
// 	  }
// 	}
// 	n1 = n1.nextSibling();
//       }
//     }
//   }

//   void 
//   WindowArbiterParameters::printToStream(std::ostream& ostr) const
//   {
//     Super::printToStream(ostr);

//     WindowArbiterParameters::PriorityMap::const_iterator i;
//     for (i = priorities.begin(); i != priorities.end(); ++i) {
// 	ostr << i->first->getBehaviourName() << "'s priority: " << i->second << endl;
//     }
//   }
};
