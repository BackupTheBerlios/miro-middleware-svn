
#ifndef VideoDevicePlayer_h
#define VideoDevicePlayer_h

#include "VideoDevice.h"

// forward declarations
namespace PlayerCc {
  class PlayerClient;
  class CameraProxy;
}

namespace Video
{
  /// Miro video device which connects to the player robot simulator
  class DevicePlayer : public Device
  {
    typedef Device Super;

  public:
    DevicePlayer(const Miro::ImageFormatIDL& _inputFormatParameters);
    virtual ~DevicePlayer();
	    	    
    FILTER_PARAMETERS_FACTORY(DevicePlayer);

  protected:
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void process();

    PlayerCc::PlayerClient * playerClient_;
    PlayerCc::CameraProxy * playerCamera_;
  };
}

#endif
