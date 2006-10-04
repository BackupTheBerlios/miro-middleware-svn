
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldTelegramCodes.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LD_TELEGRAM_CODES_H_
  #define _LD_TELEGRAM_CODES_H_

  #include "ldUserTypes.h"


namespace ldoem_
{
  //---------------------------------------------------------------------------
  ///
  /// Sick LD_OEM telegramm codes

  //------------------------------------------------------------------------------------------------------------
  /// Telegram trailer LD_OEM
  ///
  static const uint8 STX = 0x02;
  static const uint8 ETX = 0x03;

  //------------------------------------------------------------------------------------------------------------
  /// Service codes Host to LD_OEM
  ///
  static const uint16 GET_IDENTIFICATION = 0x0101;
  static const uint16 GET_STATUS         = 0x0102;
  static const uint16 GET_SIGNAL         = 0x0104;
  static const uint16 SET_SIGNAL         = 0x0105;

  //------------------------------------------------------------------------------------------------------------
  /// Configuration Service codes Host to LD_OEM
  ///
  static const uint16 SET_CONFIG     = 0x0201;
  static const uint16 GET_CONFIG     = 0x0202;
  static const uint16 SET_TIME_ABS   = 0x0203;
  static const uint16 SET_TIME_REL   = 0x0204;
  static const uint16 GET_SYNC_CLOCK = 0x0205;
  static const uint16 GET_FUNCTION   = 0x020b;
  static const uint16 SET_FUNCTION   = 0x020a;

  //------------------------------------------------------------------------------------------------------------
  /// Measurement Service codes Host to LD_OEM
  ///
  static const uint16 GET_PROFILE    = 0x0301;
  static const uint16 CANCEL_PROFILE = 0x0302;

  //------------------------------------------------------------------------------------------------------------
  /// Response Service codes LD_OEM to Host
  ///
  static const uint16 RESPONSE_IDENTIFACATION = 0x8101;
  static const uint16 RESPONSE_STATUS         = 0x8102;

  //------------------------------------------------------------------------------------------------------------
  /// Working codes Host to LD_OEM
  ///
  static const uint16 DO_RESET       = 0x0401;
  static const uint16 TRANSE_IDLE    = 0x0402;
  static const uint16 TRANSE_ROTATE  = 0x0403;
  static const uint16 TRANSE_MEASURE = 0x0404;

  //------------------------------------------------------------------------------------------------------------
  /// Response working codes LD_OEM to Host
  ///
  static const uint16 RESPONSE_DO_RESET       = 0x8401;
  static const uint16 RESPONSE_TRANSE_IDLE    = 0x8402;
  static const uint16 RESPONSE_TRANSE_ROTATE  = 0x8403;
  static const uint16 RESPONSE_TRANSE_MEASURE = 0x8404;

}; // end of namespace ldoem_

#endif // _LD_TELEGRAM_CODES_H_
