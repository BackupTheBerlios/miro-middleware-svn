// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef CanonPanTiltDevice_h
#define CanonPanTiltDevice_h

namespace Canon
{
  // forward declarations
  class Message;

  // types of timers registerd for Mcp::EventHandler
  //  typedef enum { PING_TIMER, SYNCH_TIMER } TimerType;

  // value indicating beginning and end of a Canon message
  static const unsigned char COMMAND_HEADER     = 0xff;
  static const unsigned char ANSWER_HEADER      = 0xfe;
  static const unsigned char TERMINATION_HEADER = 0xfa;
  static const unsigned char CASCADE_HEADER     = 0xf8;
  static const unsigned char REMOTE_HEADER_ON   = 0xfd; //button pushed
  static const unsigned char REMOTE_HEADER_OFF  = 0xfc; //button released
  static const unsigned char END_MARK           = 0xef;

  // error codes
  static const unsigned short ERROR_NO_ERROR  =0x3030;
  static const unsigned short ERROR_BUSY      =0x3130;
  static const unsigned short ERROR_PARAMETER =0x3530;
  static const unsigned short ERROR_MODE      =0x3930;

  // camera index values
  static const unsigned short SET_ALL_CAMERAS = 0x3030;
  static const unsigned short SET_CAMERA1     = 0x3031;
  static const unsigned short SET_CAMERA2     = 0x3032;
  static const unsigned short SET_CAMERA3     = 0x3033;
  static const unsigned short SET_CAMERA4     = 0x3034;
  static const unsigned short SET_CAMERA5     = 0x3035;
  static const unsigned short SET_CAMERA6     = 0x3036;
  static const unsigned short SET_CAMERA7     = 0x3037;
  static const unsigned short SET_CAMERA8     = 0x3038;
  static const unsigned short SET_CAMERA9     = 0x3039;

  //pedestal (PanTilt) command codes
  static const unsigned short SET_PAN_SPEED         = 0x0050;
  static const unsigned short SET_TILT_SPEED        = 0x0051;
  static const unsigned short GET_SPEED             = 0x0052; //shared
  static const unsigned short GET_PAN_SPEED         = 0x0052; //shared 0
  static const unsigned short GET_TILT_SPEED        = 0x0052; //shared 1

  static const unsigned short PAN_TILT_MOVE         = 0x0053; //shared
  static const unsigned short PAN_TILT_STOP         = 0x0053; //shared 0
  static const unsigned short PAN_RIGHT_START       = 0x0053; //shared 1
  static const unsigned short PAN_LEFT_START        = 0x0053; //shared 2
  static const unsigned short TILT_UP_START         = 0x0053; //shared 3
  static const unsigned short TILT_DOWN_START       = 0x0053; //shared 4

  static const unsigned short MOVE_HOME             = 0x0057;
  static const unsigned short INITIALIZE            = 0x0058; //shared
  static const unsigned short INITIALIZE1           = 0x0058; //shared 0
  static const unsigned short INITIALIZE2           = 0x0058; //shared 1
 
  static const unsigned short GET_SPEED_LIMITS      = 0x0059; //shared
  static const unsigned short GET_PAN_MIN_SPEED     = 0x0059; //shared 0
  static const unsigned short GET_PAN_MAX_SPEED     = 0x0059; //shared 1
  static const unsigned short GET_TILT_MIN_SPEED    = 0x0059; //shared 2
  static const unsigned short GET_TILT_MAX_SPEED    = 0x0059; //shared 3

  static const unsigned short GET_ANGLE_RATIOS      = 0x005B; //shared
  static const unsigned short GET_PAN_ANGLE_RATIO   = 0x005B; //shared 0
  static const unsigned short GET_TILT_ANGLE_RATIO  = 0x005B; //shared 1

  static const unsigned short GET_ANGLE_LIMITS      = 0x005C; //shared
  static const unsigned short GET_PAN_MIN_ANGLE     = 0x005C; //shared 0
  static const unsigned short GET_PAN_MAX_ANGLE     = 0x005C; //shared 1
  static const unsigned short GET_TILT_MIN_ANGLE    = 0x005C; //shared 2
  static const unsigned short GET_TILT_MAX_ANGLE    = 0x005C; //shared 3

  static const unsigned short PAN_TILT_START_STOP   = 0x0060; 

  static const unsigned short PAN_TILT_SET_POSITION = 0x0062; 
  static const unsigned short PAN_TILT_GET_POSITION = 0x0063; 

  static const unsigned short SET_ANGLE_RANGE       = 0x0064; //shared
  static const unsigned short SET_PAN_RANGE_LIMIT   = 0x0064; //shared 0
  static const unsigned short SET_TILT_RANGE_LIMIT  = 0x0064; //shared 1  
  static const unsigned short GET_ANGLE_RANGE       = 0x0065; //shared
  static const unsigned short GET_PAN_RANGE_LIMIT   = 0x0065; //shared 0
  static const unsigned short GET_TILT_RANGE_LIMIT  = 0x0065; //shared 1

  //camera command codes

  static const unsigned short SET_POWER             = 0x00A0; //shared
  static const unsigned short POWER_OFF             = 0x00A0; //shared 0
  static const unsigned short POWER_ON              = 0x00A0; //shared 1

  static const unsigned short FOCUS_OPERATE         = 0x00A1; //shared
  static const unsigned short FOCUS_AUTO            = 0x00A1; //shared 0
  static const unsigned short FOCUS_MANUAL          = 0x00A1; //shared 1
  static const unsigned short FOCUS_NEAR            = 0x00A1; //shared 2
  static const unsigned short FOCUS_FAR             = 0x00A1; //shared 3
  static const unsigned short SET_FOCUS_POSITION    = 0x00B0;
  static const unsigned short FOCUS_REQUEST         = 0x00B1; //shared
  static const unsigned short GET_FOCUS_POSITION    = 0x00B1; //shared 0
  static const unsigned short ONE_PUSH_AF           = 0x00B1; //shared 1
  static const unsigned short GET_FOCUS_RANGE       = 0x00B1; //shared 2

  static const unsigned short ZOOM_OPERATE          = 0x00A2; //shared
  static const unsigned short ZOOM_STOP             = 0x00A2; //shared 0
  static const unsigned short ZOOM_WIDE             = 0x00A2; //shared 1
  static const unsigned short ZOOM_TELE             = 0x00A2; //shared 2
  static const unsigned short ZOOM_HI_WIDE          = 0x00A2; //shared 3
  static const unsigned short ZOOM_HI_TELE          = 0x00A2; //shared 4
  static const unsigned short SET_ZOOM_POSITION1    = 0x00A3;
  static const unsigned short GET_ZOOM_POSITION1    = 0x00A4;
  static const unsigned short SET_ZOOM_POSITION2    = 0x00B3;
  static const unsigned short GET_ZOOM_POSITION2    = 0x00B4; //shared 0
  static const unsigned short SET_ZOOM_SPEED        = 0x00B4; //shared 1
  static const unsigned short GET_ZOOM_SPEED        = 0x00B4; //shared 2
  static const unsigned short GET_ZOOM_MAX          = 0x00B4; //shared 3

  //...
  
  static const unsigned short CAMERA_RESET          = 0x00AA;
  static const unsigned short GET_ZOOM_RATIO        = 0x00AB;

  //...

  //system command codes

  // ...
  static const unsigned short SET_CONTROL_MODE      = 0x0090; //shared
  static const unsigned short HOST_CONTROL_MODE     = 0x0090; //shared 0
  static const unsigned short LOCAL_CONTROL_MODE    = 0x0090; //shared 1
  //...
  
};

#endif
