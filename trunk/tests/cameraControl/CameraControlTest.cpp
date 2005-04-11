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
#include "miro/Client.h"
#include "miro/IO.h"
#include "miro/Angle.h"

#include <iostream>
#include <cmath>

using Miro::Client;
using Miro::CameraControl;
using Miro::CameraControl_var;
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
  short focus;
  int time;
  char c, buffer[256];

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    CameraControl_var cameraControl = client.resolveName<CameraControl>("CameraControl");

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
	   << " q to quit" << endl;
      cin.getline(buffer,256);
      c = buffer[0];
      if (c == 'q')
	break;

      Miro::ZoomRangeIDL zoomRange;
      Miro::FocusRangeIDL focusRange;
      Miro::ShutterRangeIDL shutterRange;

      switch (c) {
      case '0':
	zoomRange=cameraControl->getZoomRange();
	cout << "Zoom limits: " 
	     << rad2Deg(zoomRange.min) << " <-> " 
	     << rad2Deg(zoomRange.max) << endl;
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
	     << focusRange.min << " <-> " 
	     << focusRange.max << endl;
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
      case 'q':
	quit = true;
	break;
      default:
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




