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

#include "miro/VideoC.h"
#include "miro/Client.h"
#include "miro/VideoHelper.h"

#include <iostream>

int main(int argc, char * argv[])
{
  int rc = 0;

  Miro::Client client(argc, argv);
  try {
    Miro::Video_var video =   // Get reference to video service.
      client.resolveName<Miro::Video>("Video"); 
    Miro::VideoConnection connection(video.in()); // Build up connection.

    // Get a local copy of the current image,
    // using full resolution.
    CORBA::ULong x = connection.handle->format.width;
    CORBA::ULong y = connection.handle->format.height;
    Miro::SubImageDataIDL_var image1 =
      video->exportSubImage(connection.handle->format.width,
			    connection.handle->format.height);

    // Get a local copy next image,
    // scaled down somehow.
    x = connection.handle->format.width / 2;
    y = connection.handle->format.height / 3;
    Miro::SubImageDataIDL_var image2 = 
      video->exportWaitSubImage(x, y);

    // Acquire the current image buffer.
    Miro::VideoAcquireImage image3(connection, 
				   Miro::VideoAcquireImage::Current);

    // Acquire the next image buffer.
    Miro::VideoAcquireImage image4(connection, 
				   Miro::VideoAcquireImage::Next);

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



