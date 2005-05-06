// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>

#include "idl/VideoC.h"
#include "miro/Client.h"
#include "miro/IO.h"

using namespace Miro;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int main(int argc, char *argv[])
{
  Client client(argc, argv);

  int count = 10;
  CORBA::ULong height, width;

  try {
    Video_var	video = client.resolveName<Video>("Video");
    ImageHandleIDL_var imageIDL;
    CORBA::ULong id;
    imageIDL = video->connect(id);
    
    if  (argc == 1) {
      fprintf (stderr, "%s                   ..gets  10 images of original size\n", argv[0]);
      fprintf (stderr, "%s num               ..gets num images of original size\n", argv[0]);
      fprintf (stderr, "%s width height      ..gets  10 images of size width*height\n", argv[0]);
      fprintf (stderr, "%s width height num  ..gets num images of size width*height\n", argv[0]);
    }
    
    if  (argc > 2) {
      width  = atoi(argv[1]);
      height = atoi(argv[2]);
    } 
    else {
      width = imageIDL->format.width;
      height = imageIDL->format.height;
    }

    if ((argc == 4) || (argc == 2))
       count = atoi(argv[argc - 1]);

    fprintf (stderr, " width = %d, height = %d, count = %d  ",
	     width, height, count);

    for (int j=0; j < count; j++) {

      unsigned char * buffer = new unsigned char[3 * width * height];
      
      SubImageDataIDL_var totake = video->exportWaitSubImage(width, height);

      buffer = totake->get_buffer();

      char fname[128];
      std::sprintf(fname, "video%.5d.ppm", j);
      FILE*	imgFile = std::fopen(fname, "wb");
      if (imgFile) {
	std::fprintf(imgFile, "P6\n");
	std::fprintf(imgFile, "%d %d\n255\n", width, height);
	std::fwrite(buffer, 3*sizeof(char), width * height, imgFile);
	std::fclose(imgFile);
      }
    }
    video->disconnect(id);
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




