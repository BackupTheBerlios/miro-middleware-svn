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


#include "SickLaserParameters.h"
#include "miro/ScanDescriptionHelper.h"
#include "miro/Angle.h"

#include <qdom.h>

#include <iostream>

namespace Laser
{
  Parameters::Parameters() :
    stdcrystal(false),
    pollintervall(100000),
    continousmode(false),
    notify(true),
    positionStamps(false),
    statistics(true)
  {
    // device the laser is connected to, usually /dev/laser, but maybe any tty
    device = "/dev/laser";
    // bit rate which should be used for communication, should be one of:
    // 9600, 19200, 38400, 500000, for 500000 see also stdcrystal below
    ttyParams.baudrate = 500000;

    laserDescription.scanType = Miro::GROUPWISE;
    laserDescription.eventName = "Laser";
    laserDescription.group.length(1);
    laserDescription.group[0].description.minRange = 50;
    laserDescription.group[0].description.maxRange = 8000;
    laserDescription.group[0].description.focus = 0.00435;
    laserDescription.group[0].sensor.length(361);
    for (unsigned int i = 0; i < 361; ++i) {
      laserDescription.group[0].sensor[i].height = 300;
      laserDescription.group[0].sensor[i].distance = 0;
      laserDescription.group[0].sensor[i].alpha = Miro::deg2Rad(-90. + (double)i * 0.5);
      laserDescription.group[0].sensor[i].beta = 0;
      laserDescription.group[0].sensor[i].gamma = 0;
    }

  }

  void
  Parameters::operator <<= (const QDomNode& node)
  {
    Super::operator <<= (node);
    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomNode n2 = n1.firstChild();
	if (!n2.isNull()) {
	  if (n1.nodeName() == "stdcrystal") {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    if(!t.isNull() ) {        // the node was really a text element.
	      stdcrystal = (t.data() == "true");
	    }
	  } 
	  else if (n1.nodeName() == "continousmode") {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    if(!t.isNull() ) {        // the node was really a text element.
	      continousmode = (t.data() == "true");
	    }
	  } 
	  else if (n1.nodeName() == "pollintervall") {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    if(!t.isNull() ) {        // the node was really a text element.
	      pollintervall = t.data().toInt();
	    }
	  } 
	  else if (n1.nodeName() == "notify") {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    if(!t.isNull() ) {        // the node was really a text element.
	      notify = (t.data() == "true");
	    }
	  } 
	  else if (n1.nodeName() == "positionstamps") {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    if(!t.isNull() ) {        // the node was really a text element.
	      positionStamps = (t.data() == "true");
	    }
	  } 
	  else if (n1.nodeName() == "statistics") {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    if(!t.isNull() ) {        // the node was really a text element.
	      statistics = (t.data() == "true");
	    }
	  } 
	  else if (n1.nodeName() == "laser") {
	    while (!n2.isNull()) {
	      if (n2.nodeName() == "scandescription") 
		laserDescription <<= n2;
	      n2 = n2.nextSibling();
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
	 << "stdcrystal: " << desc.stdcrystal << endl
	 << "continousmode: " << desc.continousmode << endl
	 << "pollintervall: " << desc.pollintervall << endl
	 << "notify: " << desc.notify << endl
	 << "positionStamps: " << desc.notify << endl
	 << "statistics: " << desc.notify << endl
	 << "laser scandescription = " << desc.laserDescription << endl;
    
    return ostr;
  }
};
