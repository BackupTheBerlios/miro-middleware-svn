
#include "VideoDevicePlayer.h"
#include "BufferManager.h"

#include <playerc++.h>
#include <playerclient.h>

namespace Video
{
  using namespace PlayerCc;

  FILTER_PARAMETERS_FACTORY_IMPL(DevicePlayer);


  DevicePlayer::DevicePlayer(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    playerClient_(NULL),
    playerCamera_(NULL)
  {
    MIRO_LOG(LL_NOTICE,"Create DevicePlayer");
    interfaceAllowed_ = true;
  }

    
  DevicePlayer::~DevicePlayer()
  {
  }
	

  void
  DevicePlayer::process()
  {
    try {
      playerClient_->Read();
    } catch (PlayerCc::PlayerError & e) {
      throw Miro::EDevIO("Could not read from Player camera");
    }
    assert(inputFormat_.width==playerCamera_->GetWidth());
    assert(inputFormat_.height==playerCamera_->GetHeight());
    
    // is there a good reason to make it so complicated? hopefully not

//     unsigned char * outBuffer = outputBuffer();
//     unsigned char tempBuffer[playerCamera_->GetImageSize()];
//     unsigned char * inBuffer;
//     inBuffer = tempBuffer;
//     playerCamera_->GetImage(inBuffer);
//     //do work
//     unsigned long int i;
//     for (i=0; i<playerCamera_->GetImageSize(); i++) {
//       *outBuffer=*inBuffer++;
//       outBuffer++;
//     }

    unsigned char inBuffer[playerCamera_->GetImageSize()];
    unsigned char * outBuffer = outputBuffer();

    playerCamera_->GetImage(inBuffer);
    memcpy(outBuffer, inBuffer, playerCamera_->GetImageSize());

    bufferManager_->bufferTimeStamp(outputBufferIndex_, ACE_OS::gettimeofday());
  }


  void
  DevicePlayer::init(Miro::Server& _server, FilterParameters const * _params)
  {
    Super::init(_server, _params);

    MIRO_LOG(LL_NOTICE,"Initializing DevicePlayer");

    DevicePlayerParameters const * params = dynamic_cast<DevicePlayerParameters const *>(_params);
    assert(params != NULL);

    MIRO_LOG(LL_NOTICE, "Video::DevicePlayer: Connecting to Player");

    int playerPort=params->playerPort;
    if (playerPort < 0) 
      playerPort = PLAYER_PORTNUM;

    playerClient_ = new PlayerClient(params->playerHost.c_str(), playerPort);
    playerCamera_ = new CameraProxy(playerClient_, params->playerDevice);
  }
}
