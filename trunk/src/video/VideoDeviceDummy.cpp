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

namespace Video
{
//--------------------------------------------------------------------
  VideoDeviceDummy::VideoDeviceDummy(Parameters const * _params) :
    Super(_params),
    is_connected_(false),
    buffer_(NULL) 
  {
  }
    
//--------------------------------------------------------------------
  VideoDeviceDummy::~VideoDeviceDummy()
  {
    delete buffer_;
  }
	
  //--------------------------------------------------------------------
  void * 
  VideoDeviceDummy::grabImage(ACE_Time_Value & _timeStamp) const
  {
    if (is_connected_) {
      _timeStamp = ACE_OS::gettimeofday();
      iCurrentBuffer = (iCurrentBuffer + 1) % iNBuffers;
      return buffer_ + iCurrentBuffer*getImageSize();
    }
    return 0;
  }
    
//--------------------------------------------------------------------
  void
  VideoDeviceDummy::connect()
  {
    formatID = ::Video::getFormat(params_->format);
    sourceID = ::Video::getSource(params_->source);
    requestedPaletteID = ::Video::getPalette(params_->palette);
    devicePaletteID = ::Video::getPalette(params_->palette);
    requestedSubfieldID = ::Video::getSubfield(params_->subfield);
    deviceSubfieldID = ::Video::getSubfield(params_->subfield);
    imgWidth = params_->width;
    imgHeight = params_->height;
    iNBuffers = params_->buffers;
    iCurrentBuffer = 0;
    iNFramesCaptured = 1;
    is_connected_ = true;
	
    delete buffer_;
    buffer_ = new char [iNBuffers*getImageSize()];
    memset(buffer_, 0, iNBuffers*getImageSize());
    
    for (int i= 0; i < iNBuffers; i++)
      memset(buffer_+i*getImageSize(), 255, 5*getPixelSize(requestedPaletteID));
  }
        
//--------------------------------------------------------------------
  void
  VideoDeviceDummy::disconnect()
  {
    is_connected_ = false;
  }
};


