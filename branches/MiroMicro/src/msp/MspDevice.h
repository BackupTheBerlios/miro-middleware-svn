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
#ifndef MspDevice_h
#define MspDevice_h

namespace Msp 
{
  // number of the master msp (for sonar triggering)
  static const int masterMspAddr = 0x20;

  // sonar parameter structure
  struct SonParmsType
  {
    int echoCount;		// number of echos to capture 
    int echoTimeout;		// time to wait for possible echos 1/150 sec 
    int fireDelay;		// don't worry about this  
    int fireInterval;		// time between sets 1/150 sec 
    int echoBlankTime;		// between echo blanking 
    int initialBlankTime;		// ping to look for echo blanking 
    int startDelay;		// don't worry about this either 
  };

  static const int NUM_SONARS = 24;
  static const int SONARS_PER_SET = 6;
  static const int NUM_SETS = 4; // How many sets for full table
  static const int BOGUS_THRESHOLD =0;
  static const unsigned short NO_RETURN = 0x7FFF;
  static const double MM_PER_CLICK = 0.568;

  static const int TACTILE_PER_MSP = 8;
  static const int TACTILE_MSPS = 7;

  static const unsigned int TACTILE_ROWS = 4;
  static const unsigned int TACTILE_COLS = 16;

  static const int IR_PER_MSP = 8;
  static const int IR_MSPS = 7;

  static const unsigned int INFRARED_ROWS = 3;
  static const unsigned int INFRARED_COLS = 24;

  extern const unsigned long sonarAddr[25];
  extern const unsigned long mspAddr[];

  extern const int irNum2Row[IR_MSPS][IR_PER_MSP];
  extern const int irNum2Col[IR_MSPS][IR_PER_MSP]; 
  
  extern const int tactileNum2Row[TACTILE_MSPS][TACTILE_PER_MSP];
  extern const int tactileNum2Col[TACTILE_MSPS][TACTILE_PER_MSP];

  //----------------------------------------------------------------------------
  // Opcodes for msp communication
  //----------------------------------------------------------------------------

  // 0x00 - 0x7F are reserved for vendor usage
  // 0xC0 - 0xC8 are reserved for vendor usage 

  // MSP Control/Status opcodes 
  //   Protocol bit == 1 
  const unsigned char AB_RWI_OPCODE      = 0x7E; // RWI opcode 
  const unsigned char AB_MSP_OPCODE      = 0x77; // RWI MSP opcode 

  const unsigned char AB_RWI_DBG_STR     = 0x01; // Debug Output 
  const unsigned char AB_RWI_DBG_BIN     = 0x02; // Debug Code (binary) 
  const unsigned char AB_RWI_AB_TX       = 0x03; // Ab Trasmit 
  const unsigned char AB_RWI_CONFIG_REQ  = 0x04; // System Configuration 
  const unsigned char AB_RWI_CONFIG_RPL  = 0x05; // System Configuration Reply 
  const unsigned char AB_RWI_VR_REQ      = 0x06; // Version Request 
  const unsigned char AB_RWI_VR_RPL      = 0x07; // Version Reply 
  const unsigned char AB_RWI_RD_MEM_REQ  = 0x08; // Read Memory request 
  const unsigned char AB_RWI_RD_MEM_RPL  = 0x09; // Read Memory reply 
  const unsigned char AB_RWI_WR_MEM      = 0x0A; // Write Memory 
  const unsigned char AB_RWI_EXEC        = 0x0B; // Execute at Address 
  const unsigned char AB_RWI_SET_FD      = 0x0C; // send async reports to this port 


  const unsigned char MSP_AD_REQ         = 0x11;
  const unsigned char MSP_AD_RPL         = 0x12;
  const unsigned char MSP_AD_PARMS       = 0x13;	// not yet supported
  const unsigned char MSP_AD_PARMS_RPL   = 0x14;	// not yet supported

  const unsigned char MSP_BMP_REQ        = 0x21;
  const unsigned char MSP_BMP_RPL        = 0x22;
  const unsigned char MSP_BMP_PARMS      = 0x23;	// not yet supported
  const unsigned char MSP_BMP_PARMS_RPL  = 0x24;	// not yet supported

  const unsigned char MSP_IR_REQ         = 0x31;
  const unsigned char MSP_IR_RPL         = 0x32;
  const unsigned char MSP_IR_PARMS       = 0x33;
  const unsigned char MSP_IR_PARMS_RPL   = 0x34;

  const unsigned char MSP_SON_REQ        = 0x41;
  const unsigned char MSP_SON_RPL        = 0x42;
  const unsigned char MSP_SON_PARMS      = 0x43;
  const unsigned char MSP_SON_PARMS_RPL  = 0x44;
  const unsigned char MSP_SON_TABLE      = 0x45;
  const unsigned char MSP_SON_TABLE_RPL  = 0x46;
  const unsigned char MSP_SON_START      = 0x47;

};
#endif // mspMessage_hh




