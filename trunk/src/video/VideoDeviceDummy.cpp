/*! \file VideoDeviceDummy.cpp
 *  \brief Dummy implementation for a video device
 *
 * -----------------------------------------------------
 *
 * RoboCup Middle Size Team, Technische Universitaet Graz
 * "Mostly Harmless" 
 * http://www.robocup.tugraz.at
 *
 * Project: Miro::Video
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 * $Log$
 * Revision 1.11  2004/05/11 16:37:50  gmayer
 * add another helper macro (used by libVideoFilters)
 *
 * Revision 1.10  2004/03/09 16:28:58  roadrunner
 * Ouch! entrys->entries
 *
 * Revision 1.9  2003/10/27 13:19:21  hutz
 * making DummyDevice capable of playing image series
 *
 * Revision 1.8  2003/10/22 16:35:04  hutz
 * tons of fixes for namespace std conformance
 *
 * Revision 1.7  2003/10/17 13:31:42  hutz
 * big video service update
 * we now support filters with multiple input buffers
 * we also support the first version of a video broker interface for
 * synchronised image access and filter tree monitoring
 * - it is not yet implementation complete...
 * we now release buffers for reading as soon as all processing is done
 * we now free buffers as soon as all successors are done
 * added buffer manager for the individual devices
 * connection management is now thread safe (at least has one severe bug less)
 * TODO: documentation update
 * TODO: video broker impl
 * TODO: multiple devices
 *
 * Revision 1.6  2003/06/03 13:36:27  hutz
 * trying to remove segfaults on shutdown
 *
 * Revision 1.5  2003/06/03 10:25:32  hutz
 * complete revamp of the video service
 * the interface changes slightly to allow for better access
 * removed unnecessary copies
 * added a complete filter framework for server side image processing
 * added a library miroVideo.a for easy customization to own video service
 * derivates
 * the dummy video device now displays an image
 *
 * Revision 1.4  2003/05/16 13:14:57  hutz
 * removing unused parameters and methods from VideoDevice
 *
 * Revision 1.3  2003/05/13 21:58:49  hutz
 * removing the bridge pattern between VideoDevice and VideoDeviceBase
 * making VideoDevice* a direct descendant of VideoDevice
 *
 * Revision 1.2  2003/05/13 20:50:21  hutz
 * cleaning up the video service, getting rid of VideoConnection
 *
 * Revision 1.1  2003/05/12 11:48:05  hutz
 * added the fire wire dc support from graz
 * propagated changes and cleanups introduced by fire wire dc
 * cleaned up the parameter handling of fire wire dc
 *
 * Revision 1.4  2003/03/10 09:11:22  muehlenf
 * bugfix.
 *
 * Revision 1.3  2003/03/10 07:40:34  muehlenf
 * Additional parameters for firewire cameras.
 *
 * Revision 1.2  2003/01/10 13:46:46  muehlenf
 * Some corrections and tests, first working version.
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#include "VideoDeviceDummy.h"
#include "BufferManager.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

namespace 
{
  void getPpm(std::string const& _name, 
	      unsigned int _width,
	      unsigned int _height,
	      unsigned char * _buffer)
  {
    FILE * file;
    if ((file = fopen(_name.c_str(), "r")) != NULL) {

      int rawflag = 0;
      unsigned int xdim;
      unsigned int ydim;
      int colordepth;
      
      // read header
      char magic[1024];
      char k;

      // format identifer
      fscanf(file,"%s", magic);
      if ((!strcmp("P6", magic)) || 
	  (!strcmp("P3", magic))) {
	
	// raw format
	if (!strcmp("P6",magic))
	  rawflag = 1;
	
	fscanf(file,"%s", magic);
	while (!strcmp("#", magic)) {
	  // skip newlines
	  while ( (k = fgetc(file)) != '\n'); 
	  fscanf(file, "%s", magic);
	}
	
	xdim = atoi(magic);
	fscanf(file, "%d \n%d", &ydim, &colordepth);
	
	// skip pending newlines
	while (fgetc(file) != (int)'\n');
      }
      else
	throw Miro::Exception("Unsupported ppm file format.");

      // read image

      if (xdim != _width ||
	  ydim != _height)
	throw Miro::Exception("Image dimensions differ from output format.");

      if (rawflag) {
	for (unsigned int i = 0; i < xdim * ydim; ++i) {
	  _buffer[i*3 + 0] = fgetc(file);
	  _buffer[i*3 + 1] = fgetc(file);
	  _buffer[i*3 + 2] = fgetc(file);
	}
      } 
      else {
	int red, green, blue;
	for (unsigned int i=0; i < xdim * ydim; ++i) {
	  fscanf(file, "%d %d %d", &red, &green, &blue);
	  if (colordepth > 255) {
	    red = int((double)red * 255.0 / (double)colordepth);
	    green = int((double)green * 255.0 / (double)colordepth);
	    blue = int((double)blue * 255.0 / (double)colordepth);
	  }
	  _buffer[i*3 + 0] = red;
	  _buffer[i*3 + 1] = green;
	  _buffer[i*3 + 2] = blue;
	}
	colordepth = 255;
      }

      fclose(file);
    }
    else {
      throw Miro::CException(errno, "Cannot open file." );
    }
  }
}

namespace Video
{
  using std::cout;
  using std::cerr;
  using std::endl;

  FILTER_PARAMETERS_FACTORY_IMPL(DeviceDummy);


  //--------------------------------------------------------------------
  DeviceDummy::DeviceDummy(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    counter_(0)
  {
  }
    
//--------------------------------------------------------------------
  DeviceDummy::~DeviceDummy()
  {
  }
	
  //--------------------------------------------------------------------
  void
  DeviceDummy::process()
  {
    ACE_Time_Value now = ACE_OS::gettimeofday();

    if (params_->cyclic && 
	counter_ >= files_.size()) {
      counter_ = 0;
    }    
    if (counter_ < files_.size()) {
      // do work
      std::cout << "Next image: " << files_[counter_] << std::endl;
      getPpm(files_[counter_], 
	     inputFormat_.width,
	     inputFormat_.height,
	     outputBuffer());
      bufferManager_->bufferTimeStamp(outputBufferIndex_, 
				      ACE_OS::gettimeofday());
      ++counter_;
      std::cout << "Done." << std::endl;
    }
    else {
      throw Miro::Exception("No more images.");
    }

    ACE_Time_Value diff = ACE_OS::gettimeofday() - now;
    if (diff < params_->timeout) {
      std::cout << "Sleeping." << std::endl;
      ACE_OS::sleep(params_->timeout - diff);
    }
  }

  //--------------------------------------------------------------------
  void
  DeviceDummy::init(Miro::Server& _server, FilterParameters const * _params)
  {
    Super::init(_server, _params);

    params_ = dynamic_cast<DeviceDummyParameters const *>(_params);
    assert(params_ != NULL);
	
    cout << "loading file" << endl;;
    
    struct stat buf;
    stat(params_->device.c_str(), &buf);
    if (S_ISREG(buf.st_mode)) { // if the name is a file just use this
      files_.push_back(params_->device);
      cout << "single file: " << params_->device << endl;

    } else if (S_ISDIR(buf.st_mode)) { // if the name is a directory, use each regular file in it
      DIR * dir;
      if ((dir = opendir(params_->device.c_str())) != NULL) {
        struct dirent * entries;
        while ((entries = readdir(dir)) != NULL) {
	  stat((params_->device + entries->d_name).c_str(), &buf);
          if (S_ISREG(buf.st_mode)) {
            files_.push_back(params_->device + entries->d_name);
            cout << "direntry: " << entries->d_name << endl;
          }
        }
        closedir(dir);
      }
    }

    cout << "DeviceDummy::init end" << endl;
  }
}
