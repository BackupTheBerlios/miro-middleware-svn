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
    VideoDeviceDummy::VideoDeviceDummy()
     : is_connected_(false),
       buffer_(0) 
    {
    }
    
//--------------------------------------------------------------------
    VideoDeviceDummy::~VideoDeviceDummy()
    {
        if (buffer_) delete buffer_;
    }
	
//--------------------------------------------------------------------
    void * VideoDeviceDummy::grabImage(ACE_Time_Value & _timeStamp) const
    {
        if (is_connected_)
	{
	  _timeStamp = ACE_OS::gettimeofday();
            iCurrentBuffer = (iCurrentBuffer + 1) % iNBuffers;
            return buffer_ + iCurrentBuffer*getImageSize();
	}
	return 0;
    }
    
//--------------------------------------------------------------------
  void VideoDeviceDummy::handleConnect(const int /* fd */, const Parameters& params)
    {
	formatID = ::Video::getFormat(params.format);
        sourceID = ::Video::getSource(params.source);
        requestedPaletteID = ::Video::getPalette(params.palette);
        devicePaletteID = ::Video::getPalette(params.palette);
        requestedSubfieldID = ::Video::getSubfield(params.subfield);
        deviceSubfieldID = ::Video::getSubfield(params.subfield);
        imgWidth = params.width;
        imgHeight = params.height;
        iNBuffers = params.buffers;
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
    void VideoDeviceDummy::handleDisconnect()
    {
        is_connected_ = false;
    }
};


