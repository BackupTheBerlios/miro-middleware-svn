// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PioneerParameters.h"

#include "miro/RangeSensorC.h"
#include "miro/ScanDescriptionHelper.h"
#include "miro/Angle.h"

#include <qdom.h>

#include <iostream>

namespace Pioneer
{
  using Miro::deg2Rad;

  Miro::Singleton<Parameters> Parameters::instance;

  Parameters::Parameters()
  {
    // Motion parameter defaults

    motion.minTranslation = -500;
    motion.maxTranslation = 500;

    motion.minLTranslation = -500;
    motion.maxLTranslation = 500;
    motion.minRTranslation = -500;
    motion.maxRTranslation = 500;
    motion.wheelBase = 350;

    //
    // Infrared description defaults
    //
    static int distance[8] = { 150, 150, 150, 150, 150, 150, 150, 150 };
    static int alpha[8] = { 90, 60, 15, 0, -15, -60, -90, 180 };
    static int beta[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    sonarDescription.scanType = ::Miro::RangeSensor::GROUPWISE;
    sonarDescription.eventName = "Sonar";
    sonarDescription.group.length(1);
    sonarDescription.group[0].description.minRange = 200;
    sonarDescription.group[0].description.maxRange = 5000;
    sonarDescription.group[0].description.focus = deg2Rad(30);
    sonarDescription.group[0].sensor.length(8);
    for (unsigned int i = 0; i < 8; ++i) {
      sonarDescription.group[0].sensor[i].height = 350;
      sonarDescription.group[0].sensor[i].distance = distance[i];
      sonarDescription.group[0].sensor[i].alpha = Miro::deg2Rad(alpha[i]);
      sonarDescription.group[0].sensor[i].beta = Miro::deg2Rad(beta[i]);
      sonarDescription.group[0].sensor[i].gamma = 0;
    }
  }

  void
  Parameters::operator <<= (const QDomNode& node)
  {
    Super::operator <<= (node);

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull()) {
	if (n1.nodeName() == "Motion")
	  motion <<= n1;
	else {
	  QDomNode n2 = n1.firstChild();
	  if (!n2.isNull()) {
	    if (n1.nodeName() == "sonar") {
	      while (!n2.isNull()) {
		if (n2.nodeName() == "scandescription") {
		  cout << "parsing scandescription" << endl;
		  Miro::operator <<= (sonarDescription, n2);
		}
		n2 = n2.nextSibling();
	      }
	    }
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc) 
  {
    ostr << static_cast<const Parameters::Super&>(desc) << endl
	 << "sonar" << desc.sonarDescription;

    return ostr;
  }
};
