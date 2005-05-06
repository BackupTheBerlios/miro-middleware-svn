// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
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

#include <ace/OS.h>

using namespace Miro;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int main(int argc, char *argv[])
{
  Client client(argc, argv);
  ACE_Time_Value last, now, total;


  try {
    last = ACE_OS::gettimeofday();
    Video_var	video = client.resolveName<Video>("Video");
    CORBA::ULong id;
    ImageHandleIDL_var imageIDL;
    char*		imageData;

    imageIDL = video->connect(id);
    imageData = (char*)::shmat(imageIDL->key, NULL, 0);
    if ((int)imageData == -1)
      throw Miro::EDevIO();
    now = ACE_OS::gettimeofday();
    cout << "connecting: " << " [" << (now - last).msec()  << "ms] " << endl ;

    int	j;

    total = 0;
    CORBA::ULong offset;
    for (j=0; j<50; j++)
    {
      last = ACE_OS::gettimeofday();
      video->acquireNextImage(id, offset);
      video->releaseImage(id, offset);
      now = ACE_OS::gettimeofday();
      total += (now - last);
    }

    cout << "ms per grabbing  via 'getWaitImage': " << total.msec()/j << endl;

    total = 0;
    for (j=0; j<50; j++)
    {
      last = ACE_OS::gettimeofday();
      video->acquireCurrentImage(id, offset);
      video->releaseImage(id, offset);
      now = ACE_OS::gettimeofday();
      total += (now - last);
    }

    cout << "ms per grabbing  via 'getImage': " << total.msec()/j << endl;

    total = 0;
    for (j=0; j<50; j++)
    {
      last = ACE_OS::gettimeofday();
      video->acquireNextImage(id, offset);

      char	fname[128];

      std::sprintf(fname, "video%.5d.ppm", j);
      FILE * imgFile = std::fopen(fname, "wb");
      if (imgFile)
      {
	std::fprintf(imgFile, "P6\n");
	std::fprintf(imgFile, "%d %d 255\n", (int)imageIDL->format.width, (int)imageIDL->format.height);
	std::fwrite(imageData + offset, 3*sizeof(char), (int)imageIDL->format.width * (int)imageIDL->format.height, imgFile);
	std::fclose(imgFile);
      }

      video->releaseImage(id, offset);
      now = ACE_OS::gettimeofday();
      total += (now - last);
    }

    cout << "ms per grabbing  via 'getWaitImage' and saving :" << total.msec()/j << endl;

    total = 0;
    for (j=0; j<50; j++)
    {
      last = ACE_OS::gettimeofday();
      video->acquireCurrentImage(id, offset);

      char	fname[128];

      std::sprintf(fname, "video%.5d.ppm", j);
      FILE*	imgFile = std::fopen(fname, "wb");
      if (imgFile)
      {
	std::fprintf(imgFile, "P6\n");
	std::fprintf(imgFile, "%d %d 255\n", (int)imageIDL->format.width, (int)imageIDL->format.height);
	std::fwrite(imageData + offset, 3*sizeof(char), (int)imageIDL->format.width * (int)imageIDL->format.height, imgFile);
	std::fclose(imgFile);
      }

      video->releaseImage(id, offset);
      now = ACE_OS::gettimeofday();
      total += (now - last);
    }

    cout << "ms per grabbing  via 'getImage' and saving :" << total.msec()/j << endl;

    ::shmdt((void*)imageData);
    video->disconnect(id);
    cout << endl;
  }
  catch (const Miro::ETimeOut& e)
  {
    cerr << "Miro Timeout Exception: " << endl;
    return 1;
  }
  catch (const Miro::EDevIO & e)
  {
    cerr << "Miro Device I/O exception: " << endl;
    return 1;
  }
  catch (const Miro::EOutOfBounds & e)
  {
    cerr << "Miro out of bounds exception: " << endl;
    return 1;
  }
  catch (const CORBA::Exception & e)
  {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...)
  {
    cerr << "Uncaught local exception: " << endl;
    return 1;
  }
  return 0;
}




