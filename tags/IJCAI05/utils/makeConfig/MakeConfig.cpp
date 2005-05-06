// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/Parameters.h"
#include "miro/NotifyMulticastParameters.h"
#include "base/Parameters.h"
#include "panTilt/Parameters.h"
#include "msp/Parameters.h"
//#include "laser/Parameters.h"
//#include "video/Parameters.h"
#include "sparrow/Parameters.h"
#include "pioneer/Parameters.h"
#include "../notify/Parameters.h"

#include "qdom.h"

int
main(int argc, char *argv[])
{
  int rc = 0;

  Miro::RobotParameters * robotParameters =  Miro::RobotParameters::instance();
  Miro::NotifyMulticast::Parameters * multicastParameters =  Miro::NotifyMulticast::Parameters::instance();
  Base::Parameters * baseParameters =  Base::Parameters::instance();
  Msp::Parameters * mspParameters =  Msp::Parameters::instance();
  //  Laser::Parameters * laserParameters =  Laser::Parameters::instance();
  //  Video::Parameters * videoParameters =  Video::Parameters::instance();
  Miro::CameraParameters * cameraParameters =  Miro::CameraParameters::instance();
  Sparrow::Parameters * sparrowParameters = Sparrow::Parameters::instance();
  Pioneer::Parameters * pioneerParameters = Pioneer::Parameters::instance();
  LogNotifyParameters * logNotifyParameters = LogNotifyParameters::instance();
  
  QDomDocument doc("MiroConfigDocument");
  QDomElement e = doc.createElement("config");
  doc.appendChild(e);

  QDomElement f = e;
  for (int i = 1; i < argc; ++i) {
    QString arg = argv[i];

    if (arg == "-s" && i < (argc - 1)) {
      ++i;
      QString name = argv[i];
      f = doc.createElement("section");
      f.setAttribute("name", name);
      e.appendChild(f);
    }
    else if (arg == "-p" && i < (argc - 1)) {
      ++i;
      QString name = argv[i];
      QDomElement g;
      if (name == "Robot") {
	g = (*robotParameters) >>= f;
      }
      else if (name == "NotifyMulticast") {
	g = (*multicastParameters) >>= f;
      }
      else if (name == "Base") {
	g = (*baseParameters) >>= f;
      }
      else if (name == "Msp") {
	g = (*mspParameters) >>= f;
      }
      else if (name == "Laser") {
	//	g = (*laserParameters) >>= f;
      }
      else if (name == "Video") {
	//	g = (*videoParameters) >>= f;
      }
      else if (name == "Camera") {
	g = (*cameraParameters) >>= f;
      }
      else if (name == "SparrowBoard") {
	g = (*sparrowParameters) >>= f;
      }
      else if (name == "PioneerBoard") {
	g = (*pioneerParameters) >>= f;
      }
      else if (name == "Logging") {
	g = (*logNotifyParameters) >>= f;
      }
      else {
	std::cerr << "unknown parameter class: " << name << std::endl;
	rc = 1;
	break;
      }

      if (!g.isNull()) {
	g.setAttribute("name", name);
      }
    }
  }

  if (rc == 0) {
    QString s = doc.toString();
    std::cout << s;
  }

  return rc;
}




