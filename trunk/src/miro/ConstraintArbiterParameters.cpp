// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ConstraintArbiterParameters.h"

#include <miro/IO.h>
#include <miro/TimeHelper.h>
#include <miro/XmlParse.h>
#include <qdom.h>

namespace Miro
{
  ConstraintArbiterParameters::ConstraintArbiterParameters() :
    pace(0, 50000),
    maxAccel(2000),
    maxDecel(2000),
    spaceResolution(20),
    maxVelocity(1500),
    viewerTask(false),
    velocitySpace()
  {
    double p = (double)pace.sec() + (double)pace.usec() / 1000000.;
    velocitySpace.init(maxVelocity, spaceResolution,
		       maxAccel, maxDecel, 1/p);
  }

  void
  ConstraintArbiterParameters::operator <<= (const QDomNode& _node)
  {
    this->Super::operator <<= (_node);

    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "Pace") {
              this->pace <<= n;
            }
            else if (i == "MaxAccel") {
              this->maxAccel <<= n;
            }
            else if (i == "MaxDecel") {
              this->maxDecel <<= n;
            }
            else if (i == "SpaceResolution") {
              this->spaceResolution <<= n;
            }
            else if (i == "MaxVelocity") {
              this->maxVelocity <<= n;
            }
            else if (i == "ViewerTask") {
              this->viewerTask <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
    double p = (double)pace.sec() + (double)pace.usec() / 1000000.;
    velocitySpace.init(maxVelocity, spaceResolution,
		       maxAccel, maxDecel, 1/p);
  }

  QDomElement
  ConstraintArbiterParameters::operator >>= (QDomNode& _node) const
  {
    QDomElement e = Super::operator >>= (_node);
    QDomElement g;

    g = (this->pace >>= e);
    g.setAttribute("name", "Pace");
    g = (this->maxAccel >>= e);
    g.setAttribute("name", "MaxAccel");
    g = (this->maxDecel >>= e);
    g.setAttribute("name", "MaxDecel");
    g = (this->spaceResolution >>= e);
    g.setAttribute("name", "SpaceResolution");
    g = (this->maxVelocity >>= e);
    g.setAttribute("name", "MaxVelocity");

    return e;
  }

  void
  ConstraintArbiterParameters::printToStream(std::ostream& ostr) const
  {
    Super::printToStream(ostr);

    ostr << "Pace: " << pace << "Hz" << std::endl
         << "MaxAccel: " << maxAccel << "mm/s" << std::endl
         << "MaxDecel: " << maxDecel << "mm/s" << std::endl
         << "SpaceResolution: " << spaceResolution << "mm/s" << std::endl
         << "MaxVelocity: " << maxVelocity << "mm/s" << std::endl;
  }
}
