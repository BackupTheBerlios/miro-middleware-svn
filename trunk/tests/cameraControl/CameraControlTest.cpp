// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1995
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/CameraControlC.h"
#include "idl/CanonCameraControlC.h"
#include "idl/SonyCameraControlC.h"
#include "miro/Client.h"
#include "miro/IO.h"
#include "miro/Angle.h"

#include <iostream>
#include <cmath>

using Miro::Client;
using Miro::CameraControl;
using Miro::CameraControl_var;
using Miro::CanonCameraControl;
using Miro::CanonCameraControl_var;
using Miro::SonyCameraControl;
using Miro::SonyCameraControl_var;
using Miro::deg2Rad;
using Miro::rad2Deg;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int main(int argc, char *argv[])
{
  int rc = 0;
  bool quit = false;
  float zoom;
  float focus;
  int time;
  char c, buffer[256];
  bool canon = false, sony = false;

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    CameraControl_var cameraControl = client.resolveName<CameraControl>("CameraControl");

    CanonCameraControl_var canonCameraControl = CanonCameraControl::_narrow(cameraControl);
    if (! CORBA::is_nil(canonCameraControl)) {
      cout << "CanonCameraControl interface detected." << endl;
      canon = true;
    }

    SonyCameraControl_var sonyCameraControl = SonyCameraControl::_narrow(cameraControl);
    if (! CORBA::is_nil(sonyCameraControl)) {
      cout << "SonyCameraControl interface detected." << endl;
      sony = true;
    }

    while(!quit) {
      cout << "CameraControl test!" << endl
	   << "Chose Command: " << endl
	   << "  0 - get zoom limits" << endl
	   << "  1 - get zoom" << endl
	   << "  2 - set zoom" << endl
	   << "  3 - get focus limits" << endl
	   << "  4 - get focus" << endl
	   << "  5 - set focus" << endl
	   << "  6 - query autoFocus" << endl
	   << "  7 - set autoFocus" << endl
	   << "  8 - get shutter limits" << endl
	   << "  9 - get shutter" << endl
	   << "  a - set shutter" << endl;
      if (canon) {
	cout << "  A - set AE lock" << endl;
      } else if (sony) {
	cout << "  A - get auto exposure mode" << endl
	     << "  B - set auto exposure mode" << endl
	     << "  C - get iris aperture" << endl
	     << "  D - set iris aperture" << endl
	     << "  E - get gain limits" << endl
	     << "  F - get gain" << endl
	     << "  G - set gain" << endl;
      }
      cout << " q to quit" << endl;
      cin.getline(buffer,256);
      c = buffer[0];
      if (c == 'q')
	break;

      Miro::ZoomRangeIDL zoomRange;
      Miro::FocusRangeIDL focusRange;
      Miro::ShutterRangeIDL shutterRange;
      Miro::TimeIDL newShutter;

      switch (c) {
      case '0':
	zoomRange=cameraControl->getZoomRange();
	cout << "Zoom limits: " 
	     << rad2Deg(zoomRange.min) << " deg <-> " 
	     << rad2Deg(zoomRange.max) << " deg" << endl;
	break;
      case '1':
	cout << "Current zooming angle: "
	     << rad2Deg(cameraControl->getZoom()) << " deg" << endl;
	break;
      case '2':
	cout << "new zoom angle (deg): " << flush;
	cin >> zoom;
	cameraControl->setZoom(deg2Rad(zoom));
	break;
      case '3':
	focusRange=cameraControl->getFocusRange();
	cout << "Focus limits: " 
	     << focusRange.min << " mm <-> " 
	     << focusRange.max << " mm" << endl;
	break;
      case '4':
	cout << "Current focal distance: " 
	     << cameraControl->getFocus() << " mm" << endl;
	break;
      case '5':
	cout << "new focal distance (mm): " << flush;
	cin >> focus;
	cameraControl->setFocus(focus);
	break;
      case '6':
	if (cameraControl->hasAutoFocus()) {
	  cout << "Camera has autoFocus functionality" << endl;
	} else {
	  cout << "AutoFocus functionality not available" << endl;
	}
	break;
      case '7':
	cout << "set (1) or unset (0) autofocus?" << endl;
	cin >> focus;
	cameraControl->setAutoFocus(focus);
	break;
      case '8':
	shutterRange=cameraControl->getShutterRange();
	cout << "Shutter limits: "
	     << shutterRange.min.usec << " usec <-> "
	     << shutterRange.max.usec << " usec" << endl;
	break;
      case '9':
	cout << "Current shutter interval: "
	     << cameraControl->getShutter() << endl;
	break;
      case 'a':
	cout << "New shutter interval (usec): " << flush;
	newShutter.sec = 0;
	cin >> newShutter.usec;
	cameraControl->setShutter(newShutter);
	break;
      case 'q':
	quit = true;
	break;
      default:
	if (canon) {
	  int ae;
	  switch (c) {
	    case 'A':
	      cout << "set (1) or unset (0) AE lock?" << endl;
	      cin >> ae;
	      canonCameraControl->setAutoFocus((bool)ae);
	      break;
	    default:
	      cout << "unknown option" << endl;
	  }
	} else if (sony) {
	  SonyCameraControl::AutoExposureMode ae;
	  SonyCameraControl::IrisMode iris;
	  Miro::GainRangeIDL gainRange;
	  short gain;

	  switch (c) {
	    case 'A':
	      ae = sonyCameraControl->getAutoExposure();
	      cout << ((ae == SonyCameraControl::AE_AUTO)?
			     "Full auto-exposure" :
		       (ae == SonyCameraControl::AE_MANUAL)?
			     "Manual exposure" :
		       (ae == SonyCameraControl::AE_FIXED_SHUTTER)?
			     "Shutter fixed auto-exposure" :
		       (ae == SonyCameraControl::AE_FIXED_IRIS)?
			     "Iris fixed auto-exposure" :
		       (ae == SonyCameraControl::AE_BRIGHT)?
			     "Bright mode" :
			     "Unknown auto-exposure mode");
	      cout << endl;
	      break;

	    case 'B':
	      cout << "Chose an auto-exposure mode:" << endl
		   << "  1 - Full auto-exposure" << endl
		   << "  2 - Manual exposure" << endl
		   << "  3 - Shutter fixed auto-exposure" << endl
		   << "  4 - Iris fixed auto-exposure" << endl
		   << "  5 - Bright mode" << endl;
	      cin.getline(buffer,256);
	      c = buffer[0];
	      switch (c) {
		case '1':
		  sonyCameraControl->setAutoExposure(SonyCameraControl::AE_AUTO);
		  break;
		case '2':
		  sonyCameraControl->setAutoExposure(SonyCameraControl::AE_MANUAL);
		  break;
		case '3':
		  sonyCameraControl->setAutoExposure(SonyCameraControl::AE_FIXED_SHUTTER);
		  break;
		case '4':
		  sonyCameraControl->setAutoExposure(SonyCameraControl::AE_FIXED_IRIS);
		  break;
		case '5':
		  sonyCameraControl->setAutoExposure(SonyCameraControl::AE_BRIGHT);
		  break;
		default:
		  cout << "unknown option" << endl;
	      }
	      break;

	    case 'C':
	      iris = sonyCameraControl->getIris();
	      cout << ((iris == SonyCameraControl::IRIS_CLOSE)?
			     "Iris closed" :
		       (iris == SonyCameraControl::IRIS_F28)?
			     "Iris aperture: F28" :
		       (iris == SonyCameraControl::IRIS_F22)?
			     "Iris aperture: F22" :
		       (iris == SonyCameraControl::IRIS_F19)?
			     "Iris aperture: F19" :
		       (iris == SonyCameraControl::IRIS_F16)?
			     "Iris aperture: F16" :
		       (iris == SonyCameraControl::IRIS_F14)?
			     "Iris aperture: F14" :
		       (iris == SonyCameraControl::IRIS_F11)?
			     "Iris aperture: F11" :
		       (iris == SonyCameraControl::IRIS_F9_6)?
			     "Iris aperture: F9.6" :
		       (iris == SonyCameraControl::IRIS_F8)?
			     "Iris aperture: F8" :
		       (iris == SonyCameraControl::IRIS_F6_8)?
			     "Iris aperture: F6.8" :
		       (iris == SonyCameraControl::IRIS_F5_6)?
			     "Iris aperture: F5.6" :
		       (iris == SonyCameraControl::IRIS_F4_8)?
			     "Iris aperture: F4.8" :
		       (iris == SonyCameraControl::IRIS_F4)?
			     "Iris aperture: F4" :
		       (iris == SonyCameraControl::IRIS_F3_4)?
			     "Iris aperture: F3.4" :
		       (iris == SonyCameraControl::IRIS_F2_8)?
			     "Iris aperture: F2.8" :
		       (iris == SonyCameraControl::IRIS_F2_4)?
			     "Iris aperture: F2.4" :
		       (iris == SonyCameraControl::IRIS_F2)?
			     "Iris aperture: F2" :
		       (iris == SonyCameraControl::IRIS_F1_8)?
			     "Iris aperture: F1.8" :
			     "Unknown iris aperture");
	      cout << endl;
	      break;

	    case 'D':
	      cout << "Chose an iris aperture:" << endl
		   << "  1 - Iris closed" << endl
		   << "  2 - F28 iris aperture" << endl
		   << "  3 - F22 iris aperture" << endl
		   << "  4 - F19 iris aperture" << endl
		   << "  5 - F16 iris aperture" << endl
		   << "  6 - F14 iris aperture" << endl
		   << "  7 - F11 iris aperture" << endl
		   << "  8 - F9.6 iris aperture" << endl
		   << "  9 - F8 iris aperture" << endl
		   << "  a - F6.8 iris aperture" << endl
		   << "  b - F5.6 iris aperture" << endl
		   << "  c - F4.8 iris aperture" << endl
		   << "  d - F4 iris aperture" << endl
		   << "  e - F3.4 iris aperture" << endl
		   << "  f - F2.8 iris aperture" << endl
		   << "  g - F2.4 iris aperture" << endl
		   << "  h - F2 iris aperture" << endl
		   << "  i - F1.8 iris aperture" << endl;
	      cin.getline(buffer,256);
	      c = buffer[0];
	      switch (c) {
		case '1':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_CLOSE);
		  break;
		case '2':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F28);
		  break;
		case '3':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F22);
		  break;
		case '4':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F19);
		  break;
		case '5':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F16);
		  break;
		case '6':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F14);
		  break;
		case '7':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F11);
		  break;
		case '8':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F9_6);
		  break;
		case '9':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F8);
		  break;
		case 'a':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F6_8);
		  break;
		case 'b':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F5_6);
		  break;
		case 'c':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F4_8);
		  break;
		case 'd':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F4);
		  break;
		case 'e':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F3_4);
		  break;
		case 'f':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F2_8);
		  break;
		case 'g':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F2_4);
		  break;
		case 'h':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F2);
		  break;
		case 'i':
		  sonyCameraControl->setIris(SonyCameraControl::IRIS_F1_8);
		  break;
		default:
		  cout << "unknown option" << endl;
	      }
	      break;

	    case 'E':
	      gainRange=sonyCameraControl->getGainRange();
	      cout << "Gain limits: "
		   << gainRange.min << " <-> "
		   << gainRange.max << endl;
	      break;
	    case 'F':
	      cout << "Current gain value: "
		   << sonyCameraControl->getGain() << "db" << endl;
	      break;
	    case 'G':
	      cout << "New gain value (db): " << flush;
	      cin >> gain;
	      sonyCameraControl->setGain(gain);
	      break;

	    default:
	      cout << "unknown option" << endl;
	  }
	} else
	  cout << "unknown option" << endl;
      }
    }
  }
  catch (const Miro::EOutOfBounds & e) {
    cerr << "Out of bounds exception on server." << endl;
    rc = 1;
  }
  catch (const Miro::EDevIO & e) {
    cerr << "Device IO exception on server." << endl;
    rc = 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    rc = 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    rc = 1;
  }
  return rc;
}




