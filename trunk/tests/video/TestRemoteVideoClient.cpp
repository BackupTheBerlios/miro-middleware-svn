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
  int height, width;

  try {
    Video_var	video = client.resolveName<Video>("Video");
    ImageHandleIDL imageIDL;
	
    imageIDL = video->connect();

    if  (argc == 1) {
        fprintf (stderr, "%s                   ..gets  10 images of original size\n", argv[0]);
        fprintf (stderr, "%s num               ..gets num images of original size\n", argv[0]);
        fprintf (stderr, "%s width height      ..gets  10 images of size width*height\n", argv[0]);
        fprintf (stderr, "%s width height num  ..gets num images of size width*height\n", argv[0]);
    }

    if  (argc > 2) {
        width  = atoi(argv[1]);
	height = atoi(argv[2]);
    } else {
        width = imageIDL.width;
        height = imageIDL.height;
    }
    if ((argc == 4) || (argc == 2))
       count = atoi(argv[argc - 1]);

    fprintf (stderr, " width = %d, height = %d, count = %d  ",
	     width, height, count);

    for (int j=offset; j < offset+count; j++) {

        unsigned char * buffer = new unsigned char[3 * width * height];

        SubImageDataIDL_var totake = new SubImageDataIDL
                           (3 * width * height, 3 * width * height, buffer, 1);

        totake = video->exportWaitSubImage(width, height);

        buffer = totake->get_buffer();

	//int w = width;
	//int h= height;
	//unsigned char * p = buffer;
	//for (int x=0; x<w*h; x++,p+=3 ){
	//        char tmp = *(p+2);
	//        *(p+2) = *p;
	//	  *p = tmp;
	//}

        char	fname[128];
        std::sprintf(fname, "video%.5d.ppm", j);
        FILE*	imgFile = std::fopen(fname, "wb");
        if (imgFile)
	    {
      	    std::fprintf(imgFile, "P6\n");
      	    std::fprintf(imgFile, "%d %d\n255\n", width, height);
            std::fwrite(buffer, 3*sizeof(char), width * height, imgFile);
	    std::fclose(imgFile);
	    }
    }
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




