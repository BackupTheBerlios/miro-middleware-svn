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


#include "MspParameters.h"
#include "miro/ScanDescriptionHelper.h"
#include "miro/Angle.h"

#include <qdom.h>

#include <iostream>

using Miro::deg2Rad;

namespace Msp
{
  Parameters::Parameters()
  {
    device = "/dev/abus";

    //
    // Sonar scan description
    //
    sonarDescription.scanType = Miro::RangeEvent::BUNCHWISE;
    sonarDescription.eventName = "Sonar";
    sonarDescription.group.length(1);
    sonarDescription.group[0].description.minRange = 50;
    sonarDescription.group[0].description.maxRange = 5000;
    sonarDescription.group[0].description.focus = deg2Rad(30);
    sonarDescription.group[0].sensor.length(24);
    for (unsigned int i = 0; i < 24; ++i) {
      sonarDescription.group[0].sensor[i].height = 740;
      sonarDescription.group[0].sensor[i].distance = 248;
      sonarDescription.group[0].sensor[i].alpha = deg2Rad(172.5 - i * 15);
      sonarDescription.group[0].sensor[i].beta = 0;
      sonarDescription.group[0].sensor[i].gamma = 0;
    }

    //
    // Infrared description
    //
    static int infraredHeight[4] = {400, 120, 120};

    infraredDescription.scanType = Miro::RangeEvent::BUNCHWISE;
    infraredDescription.eventName = "Infrared";
    infraredDescription.group.length(3);
    for (unsigned int j = 0; j < 3; ++j) {
      unsigned int sensors = (j < 2)? 24 : 8;
      double angle = 360. / (double) sensors;
      infraredDescription.group[j].description.minRange = 0;
      infraredDescription.group[j].description.maxRange = 200;
      infraredDescription.group[j].description.focus = deg2Rad(30);
      infraredDescription.group[j].sensor.length(sensors);
      for (unsigned int i = 0; i < sensors; ++i) {
	infraredDescription.group[j].sensor[i].height = infraredHeight[j];
	infraredDescription.group[j].sensor[i].distance = (j < 1)? 248 : 267;
	infraredDescription.group[j].sensor[i].alpha = 
	  deg2Rad(180 - (angle / 2.) - i * angle);
	infraredDescription.group[j].sensor[i].beta = 0;
	infraredDescription.group[j].sensor[i].gamma = (j < 2)? 0. : -60.;
      }
    }
    
    //
    // Tactile scan description
    //
    static int tactileHeight[4] = {950, 450, 300, 50};

    tactileDescription.scanType = Miro::RangeEvent::BUNCHWISE;
    tactileDescription.eventName = "Tactile";
    tactileDescription.group.length(4);
    for (unsigned int j = 0; j < 4; ++j) {
      unsigned int sensors = (j < 2)? 12 : 16;
      double angle = 360. / (double) sensors;

      tactileDescription.group[j].description.minRange = 0;
      tactileDescription.group[j].description.maxRange = 10;
      tactileDescription.group[j].description.focus = deg2Rad(angle / 2.);
      tactileDescription.group[j].sensor.length(sensors);
      for (unsigned int i = 0; i < sensors; ++i) {
	tactileDescription.group[j].sensor[i].height = tactileHeight[j];
	tactileDescription.group[j].sensor[i].distance = (j < 2)? 248 : 267;
	tactileDescription.group[j].sensor[i].alpha = 
	  deg2Rad(180. - (angle / 2.) -  i * angle);
	tactileDescription.group[j].sensor[i].beta = 0;
	tactileDescription.group[j].sensor[i].gamma = 0;
      }
    }
  }

  Parameters::~Parameters()
  {}

  void
  Parameters::operator <<= (const QDomNode& node)
  {
    Super::operator <<= (node);

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull()) {
	QDomNode n2 = n1.firstChild();
	if (n1.nodeName() == "sonar") {
	  while (!n2.isNull()) {
	    if (n2.nodeName() == "scandescription") 
	      sonarDescription <<= n2;
	    n2 = n2.nextSibling();
	  }
	}
	else if (n1.nodeName() == "infrared") {
	  while (!n2.isNull()) {
	    if (n2.nodeName() == "scandescription") 
	      infraredDescription <<= n2;
	    n2 = n2.nextSibling();
	  }
	}
	else if (n1.nodeName() == "tactile") {
	  while (!n2.isNull()) {
	    if (n2.nodeName() == "scandescription") 
	      tactileDescription <<= n2;
	    n2 = n2.nextSibling();
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
	 << "sonar" << desc.sonarDescription << endl
	 << "infrared" << desc.infraredDescription << endl
	 << "tactile" << desc.tactileDescription;

    return ostr;
  }
};
