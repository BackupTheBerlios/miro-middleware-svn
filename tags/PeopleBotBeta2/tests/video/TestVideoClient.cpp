// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>

#include "miro/VideoC.h"
#include "miro/Client.h"
#include "miro/IO.h"

using namespace Miro;

int main(int argc, char *argv[])
{
  Client client(argc, argv);

  int count = 10;
  int offset = 0;

  if (argc > 1)
    count = atoi(argv[1]);
  if (argc > 2)
    count = atoi(argv[2]);

  try {
    Video_var	video = client.resolveName<Video>("Video");
    ImageHandleIDL imageIDL;
    char*		imageData;
	
	imageIDL = video->connect();
	imageData = (char*)::shmat(imageIDL.key, NULL, 0);
	if ((int)imageData == -1)
		throw Miro::EDevIO();

    for (int j=offset; j < offset+count; j++) {
      video->getWaitImage(imageIDL);
	

      int w = (int)imageIDL.width;
	int h= (int)imageIDL.height;

	char* p = imageData;
	for (int x=0; x<w*h; x++,p+=3 ){
		char tmp = *(p+2);
		*(p+2) = *p;
		*p = tmp;
	}

      char	fname[128];

      std::sprintf(fname, "video%.5d.ppm", j);
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
    video->release(imageIDL);
    cout << endl;
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




