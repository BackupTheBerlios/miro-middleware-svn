// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>

#include "idl/ImageC.h"
#include "miro/Client.h"
#include "miro/IO.h"

using namespace Miro;

int main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
	Image_var	image = client.resolveName<Image>("Image");
	ImageIDL	imageIDL;
	char*		imageData;

    ACE_Time_Value start,end, last, actual;

    start = ACE_OS::gettimeofday();
	
	imageIDL = image->requestImage(384, 288, Miro::kRGB, Miro::kNoTolerance,
									Miro::kVideoSource, Miro::kAllSubfields);
	
	imageData = (char*)::shmat(imageIDL.rgbImage.shMemKey, NULL, 0);
	if ((int)imageData == -1)
		throw Miro::EDevIO();

    for (int j=0; j<50; j++) {
      image->getWaitImage(imageIDL);
      actual = ACE_Time_Value(imageIDL.timestamp.sec, imageIDL.timestamp.usec);
      cout << j << " [" << (last - actual).msec()  << "ms] " << endl ;
      last = actual;

      char	fname[128];

      std::sprintf(fname, "image%.5d.ppm", j);
      FILE*	imgFile = std::fopen(fname, "wb");
      if (imgFile)
      	{
      	std::fprintf(imgFile, "P6\n");
      	std::fprintf(imgFile, "%d %d 255\n", (int)imageIDL.width, (int)imageIDL.height);
      	std::fwrite(imageData, 3*sizeof(char), (int)imageIDL.width * (int)imageIDL.height, imgFile);
      	std::fclose(imgFile);
      	}
    }
	::shmdt((void*)imageData);
    image->releaseImage(imageIDL);
    cout << endl;
    	
    end = ACE_OS::gettimeofday();
    

    cout << "ms per grabbing :" << (end-start).msec() / 1000.0 << endl;

    
  }
  catch (const Miro::ETimeOut& e) {
    cerr << "Miro Timeout Exception: " << endl;
    return 1;
  }
  catch (const Miro::EDevIO & e) {
    cerr << "Miro Device I/O exception: " << endl;
    return 1;
  }
  catch (const Miro::EOutOfBounds & e) {
    cerr << "Miro out of bounds exception: " << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught local exception: " << endl;
    return 1;
  }
  return 0;
}




