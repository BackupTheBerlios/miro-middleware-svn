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


#include "SparrowParameters.h"

#include "miro/TimeHelper.h"
#include "miro/ScanDescriptionHelper.h"
#include "miro/Angle.h"

#include <qdom.h>

#include <string>
#include <strstream>
#include <iterator>
#include <cmath>

namespace Sparrow
{
  using Miro::deg2Rad;
  using Miro::Singleton;
  using std::ostream;

  InfraredScaling::InfraredScaling() :
    offset(0),
    scaling(1.),
    minDistance(30),
    maxDistance(40)
  {}

  std::ostream& operator << (std::ostream& ostr, const InfraredScaling& rhs)
  {
    ostr << "Ir offset: " << rhs.offset << endl
	 << "Ir scaling: " << rhs.scaling << endl
	 << "minDistance: " << rhs.minDistance << endl
	 << "maxDistance: " << rhs.maxDistance;
    return ostr;
  }

  std::istream& operator >> (std::istream& istr, InfraredScaling& rhs)
  {
    istr >> rhs.offset >> rhs.scaling >> rhs.minDistance >> rhs.maxDistance;
    return istr;
  }


  Singleton<Parameters> Parameters::instance;

  Parameters::Parameters() :
    Super(),
    goalie(false),
    ticksL(30),
    ticksR(30),
    diameter(2310),
    maxAccel(1500),
    maxPower(30720),
    maxTransVelocity(1000),
    maxRotVelocity(200),
    stallThreshold(14),
    stallA2(1),
    stallA1(100),
    stallA0(5000),
    initialX(0),
    initialY(0),
    initialHeading(0),
    watchdogTimeout(1),
    buttonsPollInterval(0, 50000),
    servo0MidPulse(3600),
    servo1MidPulse(3300),
    deg2servoTicks(27.),
    panLatency(0),
    panMSecPerRad(100),
    panSwing(200),
    transFactor(1.0776),
    writeTables(false),
    irScaling(8),
    infraredInterval(40),
    odometryInterval(40),
    channelSharing(true)
  {
    device = "/dev/can/0";
    //
    // Infrared description defaults
    //
    static int distance[8] = { 150, 150, 150, 150, 150, 150, 150, 150 };
    static int alpha[8] = { 90, 15, 0, -15, -90, 45, -45, 180 };
    static int beta[8] = { 0, -15, 0, 15, 0, 0, 0, 0 };

    infraredDescription.scanType = Miro::RangeEvent::GROUPWISE;
    infraredDescription.eventName = "Infrared";
    infraredDescription.group.length(1);
    infraredDescription.group[0].description.minRange = 0;
    infraredDescription.group[0].description.maxRange = 800;
    infraredDescription.group[0].description.focus = deg2Rad(1);
    infraredDescription.group[0].sensor.length(8);
    for (unsigned int i = 0; i < 8; ++i) {
      infraredDescription.group[0].sensor[i].height = 150;
      infraredDescription.group[0].sensor[i].distance = distance[i];
      infraredDescription.group[0].sensor[i].alpha = Miro::deg2Rad(alpha[i]);
      infraredDescription.group[0].sensor[i].beta = Miro::deg2Rad(beta[i]);
      infraredDescription.group[0].sensor[i].gamma = 0;
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
	if (n1.nodeName() == "infrared") {
	  while (!n2.isNull()) {
	    if (n2.nodeName() == "scandescription")
	      infraredDescription <<= n2;
	    n2 = n2.nextSibling();
	  }
	}
	else if (!n2.isNull()) {
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) {        // the node was really a text element.
	    if (n1.nodeName() == "goalie") {
	      goalie = (t.data()  == "true");
	    }
	    if (n1.nodeName() == "writetable") {
	      writeTables = (t.data() == "true");
	    }
	    else if (n1.nodeName() == "ticksl") {
	      ticksL = t.data().toInt();
	    }
	    else if (n1.nodeName() == "ticksr") {
	      ticksR = t.data().toInt();
	    }
	    else if (n1.nodeName() == "diameter") {
	      diameter = t.data().toInt();
	    }
	    else if (n1.nodeName() == "maxaccel") {
	      maxAccel = t.data().toInt();
	    }
	    else if (n1.nodeName() == "maxpower") {
	      maxPower = t.data().toInt();
	    }
	    else if (n1.nodeName() == "maxptransvelocity") {
	      maxTransVelocity = t.data().toInt();
	    }
	    else if (n1.nodeName() == "maxrotvelocity") {
	      maxRotVelocity = Miro::deg2Rad(t.data().toDouble());
	    }
	    else if (n1.nodeName() == "stallthreshold") {
	      stallThreshold = t.data().toInt();
	    }
	    else if (n1.nodeName() == "stalla2") {
	      stallA2 = t.data().toInt();
	    }
	    else if (n1.nodeName() == "stalla1") {
	      stallA1 = t.data().toInt();
	    }
	    else if (n1.nodeName() == "stalla0") {
	      stallA0 = t.data().toInt();
	    }
	    else if (n1.nodeName() == "initialX") {
	      initialX = t.data().toInt();
	    }
	    else if (n1.nodeName() == "initialY") {
	      initialY = t.data().toInt();
	    }
	    else if (n1.nodeName() == "initialHeading") {
	      initialHeading = t.data().toDouble();
	    }
	    else if (n1.nodeName() == "watchdogtimeout") {
	      watchdogTimeout.msec(t.data().toInt());
	    }
	    else if (n1.nodeName() == "buttonspollinterval") {
	      buttonsPollInterval.msec(t.data().toInt());
	    }
	    else if (n1.nodeName() == "servo0midpulse") {
	      servo0MidPulse = t.data().toInt();
	    }
	    else if (n1.nodeName() == "servo1midpulse") {
	      servo1MidPulse = t.data().toInt();
	    }
	    else if (n1.nodeName() == "panlatency") {
	      panLatency = t.data().toInt();
	    }
	    else if (n1.nodeName() == "panmsecperrad") {
	      panMSecPerRad = t.data().toInt();
	    }
	    else if (n1.nodeName() == "panswing") {
	      panSwing = t.data().toInt();
	    }
	    else if (n1.nodeName() == "transfactor") {
	      transFactor = t.data().toDouble();
	    }
	    else if (n1.nodeName() == "table1") {
	      std::string buffer;
	      buffer = t.data();
	      std::istrstream istr(buffer.c_str());
	      std::copy(std::istream_iterator<short>(istr),
			std::istream_iterator<short>(),
			std::back_inserter(table1));
	    }
	    else if (n1.nodeName() == "table2") {
	      std::string buffer;
	      buffer = t.data();
	      std::istrstream istr(buffer.c_str());
	      std::copy(std::istream_iterator<short>(istr),
			std::istream_iterator<short>(),
			std::back_inserter(table2));
	    }
	    else if (n1.nodeName() == "irscaling") {
	      std::string buffer;
	      buffer = t.data();
	      irScaling.clear();
	      std::istrstream istr(buffer.c_str());
	      std::copy(std::istream_iterator<InfraredScaling>(istr),
			std::istream_iterator<InfraredScaling>(),
			std::back_inserter(irScaling));
	      irScaling.resize(8);
	    }
	    else if (n1.nodeName() == "infraredinterval") {
	      infraredInterval = t.data().toInt();
	    }
	    else if (n1.nodeName() == "odometryinterval") {
	      odometryInterval = t.data().toInt();
	    }
	    if (n1.nodeName() == "channelsharing") {
	      channelSharing = (t.data()  == "true");
	    }
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  ostream&
  operator << (ostream& ostr, const Parameters& desc)
  {
    ostr << static_cast<const Parameters::Super&>(desc) << endl
	 << ((desc.goalie)? "Goalie" : "Feldspieler")<< endl
	 << "ticksL = " << desc.ticksL << endl
	 << "ticksR = " << desc.ticksR << endl
	 << "diameter = " << desc.diameter << endl
	 << "maxAccel = " << desc.maxAccel << endl
	 << "maxPower = " << desc.maxPower << endl
	 << "maxTransVelocity = " << desc.maxTransVelocity << endl
	 << "maxRotVelocity = " << desc.maxRotVelocity << endl
	 << "stallThreshold = " << desc.stallThreshold << endl
	 << "stallA2 = " << desc.stallA2 << endl
	 << "stallA1 = " << desc.stallA1 << endl
	 << "stallA0 = " << desc.stallA0 << endl
	 << "initialX = " << desc.initialX << endl
	 << "initialY = " << desc.initialY << endl
	 << "initialHeading = " << desc.initialHeading << endl
	 << "watchdogTimeout = " << desc.watchdogTimeout << endl
	 << "buttonsPollInterval = " << desc.buttonsPollInterval << endl
	 << "servo0MidPulse = " << desc.servo0MidPulse << endl
	 << "servo1MidPulse = " << desc.servo1MidPulse << endl
	 << "deg2ServoTicks = " << desc.deg2servoTicks << endl
	 << "pan latency = " << desc.panLatency << endl
	 << "pan msec per rad = " << desc.panMSecPerRad << endl
	 << "pan swing = " << desc.panSwing << endl
	 << "translation factor = " << desc.transFactor << endl
	 << "writing tables = " << desc.writeTables << endl
	 << "irscaling:" << endl;
    std::copy(desc.irScaling.begin(),
	      desc.irScaling.end(), std::ostream_iterator<InfraredScaling>(ostr, "\n"));
    cout << "table1:" << endl;
    std::copy(desc.table1.begin(),
	      desc.table1.end(), std::ostream_iterator<short>(ostr, " "));
    cout << endl << "table2:" << endl;
    std::copy(desc.table2.begin(),
	      desc.table2.end(), std::ostream_iterator<short>(ostr, " "));
    cout << "infrared = " << desc.infraredDescription << endl 
	 << "channel sharing  = " << desc.channelSharing << endl;

    return ostr;
  }
};
