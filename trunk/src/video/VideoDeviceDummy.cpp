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

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(DeviceDummy);


  //--------------------------------------------------------------------
  DeviceDummy::DeviceDummy(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    is_connected_(false)
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
    cout << "processing dummy" << endl;
    if (is_connected_) {
      ACE_OS::sleep(ACE_Time_Value(0, 100000));
      cout << "sleeping dummy" << endl;
    }
  }

  //--------------------------------------------------------------------
  void
  DeviceDummy::init(Miro::Server& _server, FilterParameters const * _params)
  {
    Super::init(_server, _params);

    DeviceParameters const * params = dynamic_cast<DeviceParameters const *>(_params);
    assert(params != NULL);

    is_connected_ = true;
	
    cout << "loading file" << endl;;

    FILE * file;
    if ((file = fopen(params->device.c_str(), "r")) != NULL) {

      // copied from Nix
      
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

      if (xdim != outputFormat_.width ||
	  ydim != outputFormat_.height)
	throw Miro::Exception("Image dimensions differ from output format.");

      cout << "get buffer" << endl;
      unsigned int index = bufferManager_->acquireNextWriteBuffer();
      cout << "get buffer address" << endl;
      unsigned char * buffer = bufferManager_->bufferAddr(index);
      
      if (rawflag) {
	for (unsigned int i = 0; i < xdim * ydim; ++i) {
	  buffer[i*3 + 0] = fgetc(file);
	  buffer[i*3 + 1] = fgetc(file);
	  buffer[i*3 + 2] = fgetc(file);
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
	  buffer[i*3 + 0] = red;
	  buffer[i*3 + 1] = green;
	  buffer[i*3 + 2] = blue;
	}
	colordepth = 255;
      }

      fclose(file);

      inputFormat_.width = xdim;
      inputFormat_.height = ydim;
      inputFormat_.palette = Miro::RGB_24;

      outputFormat_ = inputFormat_;

      cout << "release buffer" << endl;
      bufferManager_->switchWrite2ReadBuffer(index, 0);
    }
    else {
      throw Miro::CException(errno, "Cannot open file." );
    }

    cout << "DeviceDummy::init end" << endl;
  }
        
  //--------------------------------------------------------------------
  void
  DeviceDummy::fini()
  {
    is_connected_ = false;
  }
}
