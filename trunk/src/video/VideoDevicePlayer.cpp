/*! \file VideoDevicePlayer.cpp
 *  \brief Player implementation for a video device
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
 * Revision 1.2  2005/02/11 16:48:04  guillem
 * Allow for an interface
 * Unused code cleanup
 *
 * Revision 1.1  2005/02/09 16:29:50  guillem
 * Added support for Player simulated camera. Seems to work...
 *
 * Revision 1.14  2005/01/10 15:41:09  hutz
 * extending the VIP framework to asynchronous video processing
 * now multiple filter trees can be interlinked and are synchronized as needed
 * at least in theroy :-)
 * in theory it should also be completely unintrusive for synchronous trees
 * any deviations from theory can be proven wrong and therefore do not exist
 * - or get fixed ASAP
 *
 * Revision 1.13  2005/01/09 21:40:34  hutz
 * switching debug output to use the Log.h macros.
 *
 * Revision 1.12  2004/11/02 18:04:09  gmayer
 * should handle images with blank lines (as comments) now correct
 *
 * Revision 1.11  2004/05/11 16:37:50  gmayer
 * add another helper macro (used by libVideoFilters)
 *
 * Revision 1.10  2004/03/09 16:28:58  roadrunner
 * Ouch! entrys->entries
 *
 * Revision 1.9  2003/10/27 13:19:21  hutz
 * making PlayerDevice capable of playing image series
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

#include "VideoDevicePlayer.h"
#include "BufferManager.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

#include <playerclient.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(DevicePlayer);


  //--------------------------------------------------------------------
  DevicePlayer::DevicePlayer(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    counter_(0),
    playerClient(NULL),
    playerCamera(NULL)
  {
    interfaceAllowed_=true;
  }
    
//--------------------------------------------------------------------
  DevicePlayer::~DevicePlayer()
  {
  }
	
  //--------------------------------------------------------------------
  void
  DevicePlayer::process()
  {
    ACE_Time_Value now = ACE_OS::gettimeofday();


    if (playerClient->Read()) {
      throw Miro::EDevIO("Could not read from Player camera");
    }

    assert(inputFormat_.width==playerCamera->width);
    assert(inputFormat_.height==playerCamera->height);

    unsigned char * outBuffer = outputBuffer();
    unsigned char * inBuffer = playerCamera->image;
    
    //do work
    unsigned long int i;
    for (i=0; i<playerCamera->imageSize; i++) {
      *outBuffer=*inBuffer++;
      
      outBuffer++;
    }

    bufferManager_->bufferTimeStamp(outputBufferIndex_, 
				      ACE_OS::gettimeofday());

     }

  //--------------------------------------------------------------------
  void
  DevicePlayer::init(Miro::Server& _server, FilterParameters const * _params)
  {
    Super::init(_server, _params);

    MIRO_LOG(LL_NOTICE,"Initializing DevicePlayer");

    params_ = dynamic_cast<DevicePlayerParameters const *>(_params);
    assert(params_ != NULL);
	
    MIRO_LOG(LL_NOTICE, "Video::DevicePlayer: Connecting to Player");
    
    // Just in case init was called more than once...
    delete playerCamera;
    delete playerClient;
    
    int playerPort=params_->playerPort;
    if (playerPort < 0) 
      playerPort = PLAYER_PORTNUM;

    playerClient=new PlayerClient(params_->playerHost.c_str(), playerPort);
    playerCamera = new CameraProxy(playerClient,params_->playerDevice,'r');

    if (playerCamera->GetAccess() != 'r') {
      throw Miro::Exception("Could not connect to Player");
    }

  }
}
