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

#include "idl/VideoC.h"
#include "miro/Client.h"
#include "miro/VideoHelper.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;


int main(int argc, char * argv[])
{
  int rc = 0;

  Miro::Client client(argc, argv);
  try {
    Miro::Video_var video =   // Get reference to video service.
      client.resolveName<Miro::Video>("Video"); 

    cout << "connection" << endl;

    Miro::VideoConnection connection(video.in()); // Build up connection.

    cout << "local copy" << endl;

    // Get a local copy of the current image,
    // using full resolution.
    CORBA::ULong x = connection.handle->format.width;
    CORBA::ULong y = connection.handle->format.height;
    Miro::SubImageDataIDL_var image1 =
      video->exportSubImage(connection.handle->format.width,
			    connection.handle->format.height);

    cout << "local copy next" << endl;

    // Get a local copy next image,
    // scaled down somehow.
    x = connection.handle->format.width / 2;
    y = connection.handle->format.height / 3;
    Miro::SubImageDataIDL_var image2 = 
      video->exportWaitSubImage(x, y);

    cout << "acquire current" << endl;

    // Acquire the current image buffer.
    Miro::VideoAcquireImage image3(connection, 
				   Miro::VideoAcquireImage::Current);

    cout << "acquire next" << endl;

    // Acquire the next image buffer.
    Miro::VideoAcquireImage image4(connection, 
				   Miro::VideoAcquireImage::Next);

    cout << "clean up" << endl;

    // Automatic resource cleanup by object destructors:

    // - image4 destructor releases its buffer to the VideoService.
    // - image3 destructor releases its buffer to the VideoService.
    // - image2 destructor releases the heap memory of its image copy.
    // - image1 destructor releases the heap memory of its image copy.
    // - connection destructor disconnects the client at the VideoService.
    // - video destructor destroys the video interface proxy object.
  }
  catch (const Miro::ETimeOut& e) {
    std::cerr << "Miro Timeout Exception: " << e << endl;
    rc = 1;
  }
  catch (const Miro::EDevIO & e) {
    std::cerr << "Miro Device I/O exception: " << e << endl;
    rc = 1;
  }
  catch (const Miro::EOutOfBounds & e) {
    std::cerr << "Miro out of bounds exception: " << e << endl;
    rc = 1;
  }
  catch (const CORBA::Exception & e) {
    std::cerr << "Uncaught CORBA exception: " << e << endl;
    rc = 1;
  }
  return rc;
}




