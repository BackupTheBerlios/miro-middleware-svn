// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005
// Department of Software Technology, Technical University of Graz, Austria
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
	while (!strncmp("#", magic,1)) {
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

	fread(_buffer, 1, xdim * ydim * 3, file);
// 	for (unsigned int i = 0; i < xdim * ydim; ++i) {
// 	  _buffer[i*3 + 0] = fgetc(file);
// 	  _buffer[i*3 + 1] = fgetc(file);
// 	  _buffer[i*3 + 2] = fgetc(file);
// 	}
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
      throw Miro::CException(errno, "Cannot open file." + _name + "!");
    }
  }
}

namespace Video
{
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
      MIRO_LOG_OSTR(LL_NOTICE, name() << " - next image: " << files_[counter_]);
      getPpm(files_[counter_], 
	     inputFormat_.width,
	     inputFormat_.height,
	     outputBuffer());
      bufferManager_->bufferTimeStamp(outputBufferIndex_, 
				      ACE_OS::gettimeofday());
      ++counter_;
      MIRO_LOG(LL_NOTICE, "Done.");
    }
    else {
      throw Miro::Exception("No more images.");
    }

    ACE_Time_Value diff = ACE_OS::gettimeofday() - now;
    if (diff < params_->timeout) {
      MIRO_LOG(LL_NOTICE, "Sleeping.");
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
	
    MIRO_LOG(LL_NOTICE, "Video::DeviceDummy: loading file");
    
    struct stat buf;
    stat(params_->device.c_str(), &buf);
    if (S_ISREG(buf.st_mode)) { // if the name is a file just use this
      files_.push_back(params_->device);
      MIRO_LOG_OSTR(LL_NOTICE, 
		    "Video::DeviceDummy: single file: " << params_->device);

    }
    else if (S_ISDIR(buf.st_mode)) { // if the name is a directory, use each regular file in it
      DIR * dir;
      if ((dir = opendir(params_->device.c_str())) != NULL) {
        struct dirent * entries;
        while ((entries = readdir(dir)) != NULL) {
	  stat((params_->device + entries->d_name).c_str(), &buf);
          if (S_ISREG(buf.st_mode)) {
            files_.push_back(params_->device + entries->d_name);
	    MIRO_LOG_OSTR(LL_NOTICE, 
			  "Video::DeviceDummy: direntry: " << entries->d_name);
          }
        }
        closedir(dir);
      }
    }
  }
}
