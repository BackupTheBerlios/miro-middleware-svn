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

namespace 
{
  // Pioneer1 Sonar

  const int sonarDistance[8] = { 150, 150, 150, 150, 150, 150, 150, 150 };
  const int sonarAlpha[8] = { 90, 60, 15, 0, -15, -60, -90, 180 };
  const int sonarBeta[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  // PeopleBot Tactiles
  
  const int tactileDistance[10] = { 150, 150, 150, 150, 150, 150, 150, 150, 150, 150 };
  const int tactileAlpha[10] = { 90, 60, 15, 0, -15, -60, -90, 180, -180, -120 };
  const int tactileBeta[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  // PeopleBot Infrareds
  
  const int infraredHeight[4] = { 100, 100, 150, 150 };
  const int infraredDistance[4] = { 150, 150, 150, 150 };
  const int infraredAlpha[4] = { 45, -45, 90, -90 };
  const int infraredBeta[4] = { 0, 0, 0, 0};
  const int infraredGamma[4] = { 70, 70, 90, 90};
};

namespace Pioneer
{
  using Miro::deg2Rad;

  Miro::Singleton<Parameters> Parameters::instance;

  Parameters::Parameters()
  {
    // Motion parameter defaults
    //
    // Pioneer1

    motion.minTranslation = -500;
    motion.maxTranslation = 500;

    motion.minLTranslation = -500;
    motion.maxLTranslation = 500;
    motion.minRTranslation = -500;
    motion.maxRTranslation = 500;
    motion.wheelBase = 350;

    //
    // Sonar description defaults
    //
    sonarDescription.scanType = ::Miro::RangeSensor::BUNCHWISE;
    sonarDescription.eventName = "Sonar";
    sonarDescription.group.length(1);
    sonarDescription.group[0].description.minRange = 200;
    sonarDescription.group[0].description.maxRange = 5000;
    sonarDescription.group[0].description.focus = deg2Rad(30);
    sonarDescription.group[0].sensor.length(8);
    for (unsigned int i = 0; i < 8; ++i) {
      sonarDescription.group[0].sensor[i].height = 350;
      sonarDescription.group[0].sensor[i].distance = sonarDistance[i];
      sonarDescription.group[0].sensor[i].alpha = Miro::deg2Rad(sonarAlpha[i]);
      sonarDescription.group[0].sensor[i].beta = Miro::deg2Rad(sonarBeta[i]);
      sonarDescription.group[0].sensor[i].gamma = 0;
    }

    //
    // Tactile description defaults
    //
    tactileDescription.scanType = ::Miro::RangeSensor::BUNCHWISE;
    tactileDescription.eventName = "Tactile";
    tactileDescription.group.length(1);
    tactileDescription.group[0].description.minRange = 0;
    tactileDescription.group[0].description.maxRange = 10;
    tactileDescription.group[0].description.focus = deg2Rad(30);
    tactileDescription.group[0].sensor.length(10);
    for (unsigned int i = 0; i < 10; ++i) {
      tactileDescription.group[0].sensor[i].height = 350;
      tactileDescription.group[0].sensor[i].distance = tactileDistance[i];
      tactileDescription.group[0].sensor[i].alpha = Miro::deg2Rad(tactileAlpha[i]);
      tactileDescription.group[0].sensor[i].beta = Miro::deg2Rad(tactileBeta[i]);
      tactileDescription.group[0].sensor[i].gamma = 0;
    }

    //
    // Infrared description defaults
    //
    infraredDescription.scanType = ::Miro::RangeSensor::GROUPWISE;
    infraredDescription.eventName = "Infrared";
    infraredDescription.group.length(1);
    infraredDescription.group[0].description.minRange = 0;
    infraredDescription.group[0].description.maxRange = 100;
    infraredDescription.group[0].description.focus = deg2Rad(5);
    infraredDescription.group[0].sensor.length(4);
    for (unsigned int i = 0; i < 4; ++i) {
      infraredDescription.group[0].sensor[i].height = infraredHeight[i];;
      infraredDescription.group[0].sensor[i].distance = infraredDistance[i];
      infraredDescription.group[0].sensor[i].alpha = Miro::deg2Rad(infraredAlpha[i]);
      infraredDescription.group[0].sensor[i].beta = Miro::deg2Rad(infraredBeta[i]);
      infraredDescription.group[0].sensor[i].gamma = Miro::deg2Rad(infraredGamma[i]);
    }

    camera=false;
    cameraUpsideDown=false;
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
		  sonarDescription <<= n2;
		}
		n2 = n2.nextSibling();
	      }
	    }
	    else if (n1.nodeName() == "tactile") {
	      while (!n2.isNull()) {
		if (n2.nodeName() == "scandescription") {
		  tactileDescription <<= n2;
		}
		n2 = n2.nextSibling();
	      }
	    }
	    else if (n1.nodeName() == "infrared") {
	      while (!n2.isNull()) {
		if (n2.nodeName() == "scandescription") {
		  infraredDescription <<= n2;
		}
		n2 = n2.nextSibling();
	      }
	    }
	    else if (n1.nodeName() == "Camera") {
	      QDomText t = n2.toText();
	      if (!t.isNull()) {
		if ((t.data()=="normal")||(t.data()=="yes")) {
		  camera=true;
		  cameraUpsideDown=false;
		}
		else if ((t.data()=="inverse") || (t.data()=="upsideDown")) {
		  camera=true;
		  cameraUpsideDown=true;
		} else {
		  camera=false;
		}
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
	 << "sonar" << desc.sonarDescription << endl
	 << "tactile" << desc.tactileDescription << endl
	 << "infrared" << desc.infraredDescription << endl
	 << "motion" << endl << desc.motion << endl
	 << "camera: ";
    if (!desc.camera) ostr << "none" << endl;
    else if (!desc.cameraUpsideDown) ostr << "normal mounted" << endl;
    else ostr << "inverse mounted" << endl;

    return ostr;
  }
};
