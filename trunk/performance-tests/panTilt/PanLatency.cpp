// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "idl/PanC.h"
#include "idl/VideoC.h"
#include "miro/Client.h"
#include "miro/TimeHelper.h"
#include "miro/VideoHelper.h"
#include <ace/Date_Time.h>
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
  using std::ostringstream;

  using Miro::Client;
  using Miro::Pan;
  using Miro::Pan_var;
  using Miro::Video;
  using Miro::Video_var;
  using Miro::ImageHandleIDL_var;

  // global variables
  bool bgr = false;
  string panName = "PanTilt";
  bool verbose = false;
  int stop = 2000;

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


  string createFileName()
  {
    ACE_Date_Time dt;
    dt.microsec(ACE_OS::gettimeofday().usec());

    ostringstream ostr;

    ostr << dt.year() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.month() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.day() << "-";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.hour() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.minute() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.second() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << (dt.microsec() / 10000);

    return ostr.str();
  }

  int parseArgs(int& argc, char* argv[])
  {
    ACE_Get_Opt get_opts (argc, argv, "n:rt:v?");

    int rc = 0;
    int c;
    
    while ((c = get_opts()) != -1) {
      switch (c) {
      case 'n':
	panName = get_opts.optarg;
	break;
      case 'r':
	bgr = true;
	break;
      case 't':
	stop = ACE_OS::atoi(get_opts.optarg);
	break;
      case 'v':
	verbose = true;
	break;
      case '?':
      default:
	cout << "usage: " << argv[0] << " [-r] [-n=name] [-t=msec] [-v] [-?]" << endl
	     << "  -r reverse byte order: BGR palette" << endl
	     << "  -n name of the pan in naming service (default is PanTilt)" << endl
	     << "  -t time: how long to grab images (default is 5000)" << endl
	     << "  -v verbose mode" << endl
	     << "  -? help: emit this text and stop" << endl;
	rc = 1;
      }
    }

    if (verbose)
      cout << "bgr: " << bgr << endl
	   << "name: " << panName << endl
	   << "time: " << stop << endl;

    return rc;
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

  ImageVector v;

  Client client(argc, argv);

  if ((rc = parseArgs(argc, argv)))
    return rc;

  try {
    // get reference to video service
    Pan_var pan = client.resolveName<Pan>(panName.c_str());
    Video_var video = client.resolveName<Video>("Video");
    char * imageData;
    char buffer[256];
    char c;
    double alpha;
    string angle;
    Miro::VideoConnection connection(video.in());


    // set pan to initial position
    pan->setPan(0);

    while(true) {
      cout << "Pan latency test!" << endl
	   << "Chose Command: " << endl
	   << "  0 - get pan position" << endl
	   << "  1 - set pan position" << endl
	   << " q to quit" << endl;
      cin.getline(buffer,256);
      c = buffer[0];
      if (c == 'q')
	break;

      switch (c) { // test cases
      case '0':
	cout << "current pan angle: " 
	     << Miro::rad2Deg(pan->getPan()) << "°" << endl;
	break;
      case '1':
	{
	  cout << "new pan angle (deg): " << flush;
	  cin >> angle;
	  
	  std::istringstream istr(angle);
	  istr >> alpha;
	  alpha = Miro::deg2Rad(alpha);
	  
	  // set time stamp
	  string stamp = path() + client.namingContextName + "_" + createFileName() + ".deg" + angle;
	  
	  if (verbose)
	    cout << stamp << endl;
	  
	  FILE* stampFile = std::fopen(stamp.c_str(), "wb");
	  std::fclose(stampFile);
	  
	  
	  // set pan
	  pan->setPan(alpha);
	  
	  // save image burst
          bool first = true;
	  ACE_Time_Value start = ACE_OS::gettimeofday();
	  while(true) {
	    // init streaming timer
	    if (!first) {
	      ACE_Time_Value elapsed = ACE_OS::gettimeofday();
	      elapsed -= start;
	      if (elapsed.msec() > stop)
		break;
	  }
	    first = false;
    
	    // get image
	    Miro::VideoAcquireImage frame(connection, Miro::VideoAcquireImage::Next); 
	    // fill image structure
	    Image image;
	    image.fileName = path() + client.namingContextName + "_" + createFileName() + ".ppm";
	    image.width = connection.handle->format.width;
	    image.height = connection.handle->format.height;
	    
	    // fill image buffer
	    image.buffer = new char[3 * image.width * image.height];
	    
	    // byte swapping
	    if (bgr) {
	      int offset = 0;
	      for (int i = 0; i < image.width; ++i) {
		for (int j = 0; j < image.height; ++j, offset += 3) {
		  image.buffer[offset + 0] = frame.buffer[offset + 3]; // r
		  image.buffer[offset + 1] = frame.buffer[offset + 2]; // g
		  image.buffer[offset + 2] = frame.buffer[offset + 1]; // b
		}
	      }
	    }
	    else {
	      std::memcpy(image.buffer, frame.buffer, 3 * image.width * image.height);
	    }
	    
	    // save image to buffer
	    if (verbose)
	      cout << "Save image to buffer." << endl;
	    v.push_back(image);

	  }
	  // write images to disc
	  if (verbose)
	    cout << "Write images to disc." << endl;
	  for (ImageVector::const_iterator i = v.begin(); i != v.end(); ++i) {
	    i->writePpm();
	    delete i->buffer;
	  }
	  v.clear();
	  break;
	}

      default:
	cerr << "Unknown option: " << buffer << endl;
      }
    }      

    cout << endl;

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




