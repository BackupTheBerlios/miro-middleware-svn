// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
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
#include "VideoDevicePlayer.h"
#include "BufferManager.h"

#include <libplayerc++/playerc++.h>
#include <libplayerc++/playerclient.h>

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
