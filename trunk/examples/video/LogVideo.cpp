// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "miro/VideoC.h"
#include "miro/Client.h"
#include "miro/TimeHelper.h"
#include "miro/VideoHelper.h"

#include <ace/Get_Opt.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <stdio.h>

namespace 
{
  using std::string;

  using Miro::Client;
  using Miro::Video;
  using Miro::Video_var;
  using Miro::ImageHandleIDL;

  // global variable for cooperative shutdown
  bool canceled = false;

  // global variables
  bool bgr = false;
  bool streaming = false;
  bool verbose = false;
  int interval = 0;
  int stop = 5000;
  std::string interfaceName;

  string path()
  {
    char * miro_log = ACE_OS::getenv("MIRO_LOG");
    string p;

    if (miro_log)
	p = miro_log;

    if (p.size() > 0 && p[p.size() - 1] != '/')
      p += "/";
    return p;
  }


  int parseArgs(int& argc, char* argv[])
  {
    ACE_Get_Opt get_opts (argc, argv, "i:n:rst:v?");

    int rc = 0;
    int c;
    
    while ((c = get_opts()) != -1) {
      switch (c) {
      case 'i':
	interval = ACE_OS::atoi(get_opts.optarg);
	break;
      case 'n':
	interfaceName = get_opts.optarg;
	break;
      case 'r':
	bgr = true;
      break;
      case 's':
	streaming = true;
	break;
      case 't':
	stop = ACE_OS::atoi(get_opts.optarg);
	break;
      case 'v':
	verbose = true;
	break;
      case '?':
      default:
	cout << "usage: " << argv[0] << " [-r] [-s [-i=msec] [-t=msec]] [-?]" << endl
	     << "  -n interface name (default: Video)" << endl
	     << "  -r reverse byte order: BGR palette" << endl
	     << "  -s streaming mode: continuos grabbing" << endl
	     << "  -i interval: stop between each frame (default is 0)" << endl
	     << "  -t time: how long to grab images (default is 5000)" << endl
	     << "  -? help: emit this text and stop" << endl;
	rc = 1;
      }
    }

    if (verbose)
      cout << "bgr: " << bgr << endl
	   << "streaming: " << streaming << endl
	   << "interval: " << interval << endl
	   << "time: " << stop << endl;

    return rc;
  }

  // we need to shut down the video service properly on sig term
  class Event : public ACE_Event_Handler
  {
  public:
    Event() {}
    //  private:
    virtual ~Event() {}
  public:
    
    virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  };
  
  int
  Event::handle_signal(int, siginfo_t *, ucontext_t *)
  {
    canceled = true;
    return 0;
  }

  struct Image
  {
    string fileName;
    int width;
    int height;
    char * buffer;

    void writePpm() const;
  };
  typedef std::vector<Image> ImageVector;

  void
  Image::writePpm() const
  {
    if (verbose)
     cout << fileName << endl;

    FILE* imgFile = std::fopen(fileName.c_str(), "wb");
    if (buffer) {
      std::fprintf(imgFile, "P6\n");
      std::fprintf(imgFile, "%d %d 255\n", width, height);
      std::fwrite(buffer, 3*sizeof(char), width * height, imgFile);
    }
    else
      throw string("Null pointer as image buffer.");
    std::fclose(imgFile);
  }
};

int main(int argc, char *argv[])
{
  int rc = 0;
  bool first = true;

  ImageVector v;

  Client client(argc, argv);

  if ((rc = parseArgs(argc, argv)))
    return rc;

  try {
    // event handler
    Event * event = new Event();
    
    // Signal set to be handled by the event handler.
    ACE_Sig_Set sig;
    
    // register Signal handler for Ctr+C
    sig.sig_add(SIGINT);
    sig.sig_add(SIGTERM);
    
    // Reactor, misused as signal handler
    ACE_Reactor reactor;
    
    if (reactor.register_handler(sig, event) == -1) {
      throw Miro::ACE_Exception(errno, "failed to register signal handler");
    }
    
    // get reference to video service
    Video_var video = client.resolveName<Video>("Video");
    Miro::VideoConnection connection(video.in());

    ACE_Time_Value start;
    while(!canceled) {

      // wait for key
      if (first || !streaming) {
	first = false;
	cout << "Press key to grab next image: " << flush;
	getchar();
	start = ACE_OS::gettimeofday();
      }
    
      // init streaming timer
      if (!first && streaming) {
	ACE_Time_Value elapsed = ACE_OS::gettimeofday();
	elapsed -= start;
	if (elapsed.msec() > stop)
	  break;
      }

      {
	// get image
	Miro::VideoAcquireImage 
	  frame(connection, 
		(interval)?
		Miro::VideoAcquireImage::Current :
		Miro::VideoAcquireImage::Next);

	// fill image structure
	Image image;
	image.fileName = path() + client.namingContextName + "_" + Miro::timeString() + ".ppm";
	image.width = connection.handle->format.width;
	image.height = connection.handle->format.height;
	
	// fill image buffer
	if (!bgr && !streaming)
	  image.buffer = (char*) frame.buffer;
	else {
	  image.buffer = new char[3 * image.width * image.height];

	  // byte swapping
	  if (bgr) {
	    int offset = 0;
	    for (int i = 0; i < image.width; ++i) {
	      for (int j = 0; j < image.height; ++j, offset += 3) {
		image.buffer[offset + 0] = frame.buffer[offset + 2]; // r
		image.buffer[offset + 1] = frame.buffer[offset + 1]; // g
		image.buffer[offset + 2] = frame.buffer[offset + 0]; // b
	      }
	    }
	  }
	  else
	    std::memcpy(image.buffer, frame.buffer, 3 * image.width * image.height);
	}
	
	// save image
	if (!streaming) {
	  image.writePpm();
	  if (bgr)
	    delete[] image.buffer;
	}
	else 
	  v.push_back(image);
      }

      // sleep
      if (interval) {
	ACE_Time_Value delta;
	delta.msec(interval);
	ACE_OS::sleep(delta);
      }
    }
    cout << "exiting on signal" << endl;

    for (ImageVector::const_iterator i = v.begin(); i != v.end(); ++i) {
      i->writePpm();
      delete i->buffer;
    }
  }
  catch (const Miro::ETimeOut& e) {
    cerr << "Miro Timeout Exception: " << endl;
    rc = 1;
  }
  catch (const Miro::EDevIO & e) {
    cerr << "Miro Device I/O exception: " << endl;
    rc = 1;
  }
  catch (const Miro::EOutOfBounds & e) {
    cerr << "Miro out of bounds exception: " << endl;
    rc = 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    rc = 1;
  }
  catch (...) {
    cerr << "Uncaught local exception. " << endl;
    rc = 1;
  }
  return rc;
}




