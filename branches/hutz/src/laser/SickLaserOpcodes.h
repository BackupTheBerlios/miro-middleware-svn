// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef laserOpcodes_hh
#define laserOpcodes_hh

namespace Miro {
  /* opcodes for the sick laserscanner, see manual page 2/67
   */
  /*
   * host to LMS/LMI
   */

  static const unsigned char LO_STX                       = 0x02;
  static const unsigned char LO_STARTSEQUENCE             = 0x0b;
  static const unsigned char LO_DOWNLOADDATA              = 0x0c;
  static const unsigned char LO_INIT                      = 0x10;
  static const unsigned char LO_CHANGEMODE                = 0x20;
  static const unsigned char LO_REQ_DATA                  = 0x30;
  static const unsigned char LO_REQ_STATUS                = 0x31;
  static const unsigned char LO_REQ_ERROR                 = 0x32;
  static const unsigned char LO_REQ_TEST_DIAGMODE         = 0x33;
  static const unsigned char LO_REQ_MEMORY_DUMP           = 0x34;
  static const unsigned char LO_REQ_AVERAGED_DATA         = 0x36;
  static const unsigned char LO_REQ_DATA_RANGE            = 0x37;
  static const unsigned char LO_REQ_LMI_CONF              = 0x38; 
  static const unsigned char LO_REQ_LMI_HANDLE_DEFS       = 0x39;
  static const unsigned char LO_REQ_LMS_TYPE              = 0x3a;
  static const unsigned char LO_LMS_CHANGE_VARIANT        = 0x3b;
  static const unsigned char LO_FIELD_CONF                = 0x40;
  static const unsigned char LO_PASSWORD_CHANGE           = 0x42;
  static const unsigned char LO_REQ_FIELD_CONF            = 0x45;
  static const unsigned char LO_LEARN_FIELD_CONF          = 0x46;
  static const unsigned char LO_CONF_DYN_RECT             = 0x48;
  static const unsigned char LO_CONF_DYN_SEGMENT          = 0x4b;
  static const unsigned char LO_DYN_FIELD_INDEX_OR_SPEED  = 0x4c;
  static const unsigned char LO_SET_OUTPUT                = 0x4d;
  static const unsigned char LO_LMI_INPUT                 = 0x4e;
  static const unsigned char LO_SIMULATE_INPUT            = 0x4f;
  static const unsigned char LO_CALIBRATE                 = 0x50;
  static const unsigned char LO_CHANGE_PERMANENT_BAUDRATE = 0x66;
  static const unsigned char LO_CHANGE_ADDRESS            = 0x67;
  static const unsigned char LO_ACTIVATE_SENDER           = 0x68;
  static const unsigned char LO_LMI_CONF                  = 0x70;
  static const unsigned char LO_LMI_HANDLE_DEFS           = 0x72;
  static const unsigned char LO_PIXEL_ORIENTED            = 0x74;
  static const unsigned char LO_CARTESIAN                 = 0x76;
  static const unsigned char LO_CONF_LMS                  = 0x77;

  /**
   * lmi/lms to host
   */
  static const unsigned char LR_ACK                       = 0x06;
  static const unsigned char LR_NACK                      = 0x15;
  static const unsigned char LR_POWER_ON                  = 0x90;
  static const unsigned char LR_ACK_RESET                 = 0x91;
  static const unsigned char LR_NOT_ACK                   = 0x92; // ?0x15 ??
  static const unsigned char LR_MODE_CHANGE               = 0xa0;
  static const unsigned char LR_REQ_DATA                  = 0xb0;
  static const unsigned char LR_LMS_STATUS                = 0xb1;
  static const unsigned char LR_LMI_STATUS                = 0xb1; // !
  static const unsigned char LR_ERROR_TEST                = 0xb2;
  static const unsigned char LR_MEMORY_DUMP               = 0xb4;
  static const unsigned char LR_AVERAGE_DATA              = 0xb6;
  static const unsigned char LR_DATA_RANGE                = 0xb7;
  static const unsigned char LR_LMI_CONF_DATA             = 0xb8;
  static const unsigned char LR_LMI_HANDLE_DEFS           = 0xb9;
  static const unsigned char LR_LMS_TYPE                  = 0xba;
  static const unsigned char LR_LMS_VARIANT_CHANGE        = 0xbb;
  static const unsigned char LR_FIELD_CONF                = 0xc0;
  static const unsigned char LR_PASSWORD_CHANGE           = 0xc2;
  static const unsigned char LR_FIELD_CONF_DATA           = 0xc5;
  static const unsigned char LR_LEARN_FIELD_CONF          = 0xc6;
  static const unsigned char LR_CONF_DYN_RECT             = 0xc8;
  static const unsigned char LR_CONF_DYN_SEGMENT          = 0xcb;
  static const unsigned char LR_DYN_FIELD_INDEX_OR_SPEED  = 0xcc;
  static const unsigned char LR_SET_OUTPUT                = 0xcd;
  static const unsigned char LR_LMI_INPUT                 = 0xce;
  static const unsigned char LR_SIMULATE_INPUT            = 0xcf;
  static const unsigned char LR_CALIBRATE                 = 0xd0;
  static const unsigned char LR_CHANGE_PERMANENT_BAUDRATE = 0xe6;
  static const unsigned char LR_CHANGE_ADDRESS            = 0xe7;
  static const unsigned char LR_ACTIVATE_SENDER           = 0xe8;
  static const unsigned char LR_LMI_CONF                  = 0xf0;
  static const unsigned char LR_HANDLE_DEFS               = 0xf2;
  static const unsigned char LR_PIXEL_ORIENTED            = 0xf4;
  static const unsigned char LR_CARTESIAN                 = 0xf6;
  static const unsigned char LR_CONF_LMS                  = 0xf7;

  /**
   * status byte der lms
   *
   */
  static const unsigned char LS_NOERR        = 0x0;
  static const unsigned char LS_INFO         = 0x1;
  static const unsigned char LS_WARNING      = 0x2;
  static const unsigned char LS_ERROR        = 0x3;
  static const unsigned char LS_FATALERROR   = 0x4;
  static const unsigned char LS_SOURCE       = 0x8;
  static const unsigned char LS_TBD          = 0x10;
  static const unsigned char LS_RESTART      = 0x20;
  static const unsigned char LS_NOTPLAUSIBLE = 0x40;
  static const unsigned char LS_DIRTY        = 0x80;
};
#endif




