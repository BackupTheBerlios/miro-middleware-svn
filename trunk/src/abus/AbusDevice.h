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
#ifndef AbusDevice_h
#define AbusDevice_h

#include <netinet/in.h>

namespace Abus
{
  struct DeviceId
  {
    static const int PROT_REV_OFFSET = 0;
    static const int MOD_REV_OFFSET  = 1;
    static const int VENDOR_OFFSET   = 8;
    static const int MOD_NAME_OFFSET = 16;
    static const int DEV_NUM_OFFSET  = 24;

    static const int PROT_REV_LEN     = 1;
    static const int MOD_REV_LEN      = 7;
    static const int VENDOR_LEN       = 8; 
    static const int MOD_NAME_LEN     = 8;
    static const int DEV_NUM_LEN      = 4;

    unsigned char buffer[PROT_REV_LEN + MOD_REV_LEN 
			+ VENDOR_LEN + MOD_NAME_LEN + DEV_NUM_LEN];

    unsigned char   protRev();
    unsigned char * moduleRev();
    unsigned char * vendorName();
    unsigned char * moduleName();
    long            devNum();
  };

  struct DeviceProtocol
  {
    static const int PROT_OFFSET     = 0;
    static const int TYPE_OFFSET     = 9;
    static const int MODEL_OFFSET    = 18;

    static const int PROT_LEN        = 9;
    static const int TYPE_LEN        = 9;
    static const int MODEL_LEN       = 9;

    char buffer[PROT_LEN + TYPE_LEN + MODEL_LEN];

    char * prot();
    char * type();
    char * model();
  };

  union DeviceStatus
  {
    struct 
    {
      unsigned char enable :1;
      unsigned char real   :1;
      unsigned char active :1;
      unsigned char good   :1;
      unsigned char kbd    :1;
      unsigned char dummy  :2;
      unsigned char inuse  :1;
    } flags;
    unsigned char byte;
  };

  typedef enum { NOT, NEW, ACK, READY } DeviceState;

  struct DeviceLongId
  {
    static const int ID_OFFSET       = 0;
    static const int PROTOCOL_OFFSET = sizeof(DeviceId);
    static const int STATUS_OFFSET   = sizeof(DeviceId) + sizeof(DeviceProtocol);

    unsigned char buffer[sizeof(DeviceId) 
			+ sizeof(DeviceProtocol) + sizeof(DeviceStatus)];

    DeviceId&       devId();
    DeviceProtocol& devProtocol();
    DeviceStatus&   devStatus();

    const DeviceId&       devId() const;
    const DeviceProtocol& devProtocol() const;
    DeviceStatus    devStatus() const;
  };

  struct DeviceDescription
  {
    DeviceState    state;    // is this struct in use, etc.
    DeviceLongId   longId;
  };

  // array of abus devices for device management
  extern DeviceDescription deviceTable[256];

  long devId2devNum(unsigned char index);
  unsigned char devNum2devId (long mspNum);

  //---------------------------------------------------------------------------
  // Opcodes for device communication
  //---------------------------------------------------------------------------

  static const unsigned char MC_ADDR     = 0x50;
  static const unsigned char MGR_ADDR    = 0x51;
  static const unsigned char ABDEF_ADDR  = 0x6E;

  // some useful self test constants

  static const unsigned char ST_SUCCESS      = 0x00;
  static const unsigned char ST_ROM_CHECKSUM = 0x01;
  static const unsigned char ST_INT_RAM      = 0x02;
  static const unsigned char ST_EXT_RAM      = 0x04;
  static const unsigned char ST_POWER_DOWN   = 0x08;
  static const unsigned char ST_DEVICE       = 0x10;

  // Status bits

  static const unsigned char AB_STATUS_LINKED    = 0x80;
  static const unsigned char AB_STATUS_GOOD      = 0x08;
  static const unsigned char AB_STATUS_ACTIVE    = 0x04;
  static const unsigned char AB_STATUS_PHYS      = 0x02;
  static const unsigned char AB_STATUS_ENABLE    = 0x01;

  // Required Control/Status opcodes
  //   Protocol bit == 1

  // 0x00 - 0x7F are reserved for vendor usage

  // ACCESS.bus defined opcodes

  static const unsigned char AB_APPL_HDW_SIG   = 0xA0; // Application Hdw Signal; optional 
  static const unsigned char AB_APPL_TST_RPL   = 0xA1; // Application Test Reply 
  static const unsigned char AB_APPL_STAT_MSG  = 0xA2; // Application Status Message 

  static const unsigned char AB_APPL_TST       = 0xB1; // Application Test 

  // 0xC0 - 0xC8 are reserved for vendor usage

  static const unsigned char AB_ATTN           = 0xE0; // Attention 
  static const unsigned char AB_IDENT_RPL      = 0xE1; // Identification Reply 
  static const unsigned char AB_RES_E2         = 0xE2; // Reserved 
  static const unsigned char AB_CAP_RPL        = 0xE3; // Capabilities Reply 
  static const unsigned char AB_RES_E4         = 0xE4; // Reserved 
  static const unsigned char AB_RSRC_REQ       = 0xE5; // Resource Request; optional 
  static const unsigned char AB_PWR_USG_RPL    = 0xE6; // Power Usage Reply; optional 
  static const unsigned char AB_RES_E7         = 0xE7; // Reserved 
  static const unsigned char AB_BW_USG_RPL     = 0xE8; // Bandwidth Usage Reply; optional 

  static const unsigned char AB_RESET          = 0xF0; // Reset 
  static const unsigned char AB_IDENT_REQ      = 0xF1; // Identification Request 
  static const unsigned char AB_ASGN_ADDR      = 0xF2; // Assign Address 
  static const unsigned char AB_CAP_REQ        = 0xF3; // Capabilities Request 
  static const unsigned char AB_RSRC_GRANT     = 0xF4; // Resource Grant; optional 
  static const unsigned char AB_EN_AP_REP      = 0xF5; // Enable Application Report 
  static const unsigned char AB_PWR_MGMT       = 0xF6; // Power Management; optional 
  static const unsigned char AB_PRES_CHK       = 0xF7; // Presence Check; optional 
  static const unsigned char AB_DEV_BW_MGMT    = 0xF8; // Device Bandwidth Mngmnt; optional 

  //---------------------------------------------------------------------------
  // Driver to manager commands
  //---------------------------------------------------------------------------

  static const unsigned char D2M_EXT_START          = 0x20;
  static const unsigned char D2M_RESET              = 0x20;
  static const unsigned char D2M_LINK_REQUEST       = 0x21;
  static const unsigned char D2M_LINK_APPROVE       = 0x22;
  static const unsigned char D2M_GET_LONG_ID        = 0x23;
  static const unsigned char D2M_GET_ID             = 0x24;
  static const unsigned char D2M_GET_TYPE           = 0x25;
  static const unsigned char D2M_GET_STATUS         = 0x26;
  static const unsigned char D2M_ENABLE             = 0x27;
  static const unsigned char D2M_MSG_TO_DEVICE      = 0x28;
  static const unsigned char D2M_DISCONNECT         = 0x29;
  static const unsigned char D2M_EXT_END            = 0x29;

  //---------------------------------------------------------------------------
  // Manager to driver commands
  //---------------------------------------------------------------------------
  static const unsigned char M2D_START              = 0x40;
  static const unsigned char M2D_LINK_REPLY         = 0x40;
  static const unsigned char M2D_LONG_ID            = 0x41;
  static const unsigned char M2D_ID                 = 0x42;
  static const unsigned char M2D_TYPE               = 0x43;
  static const unsigned char M2D_STATUS             = 0x44;
  static const unsigned char M2D_MSG_TO_DRIVER      = 0x45;
  static const unsigned char M2D_DISCONNECT         = 0x46;
  static const unsigned char M2D_PDT                = 0x47;
  static const unsigned char M2D_ERROR              = 0x48;
  static const unsigned char M2D_LINK_APPROVE_ACK   = 0x49;
  static const unsigned char M2D_END                = 0x49;


  //---------------------------------------------------------------------------
  // DeviceId inlines
  //---------------------------------------------------------------------------
  inline
  unsigned char
  DeviceId::protRev() {
    return buffer[PROT_REV_OFFSET]; 
  }
  inline
  unsigned char * 
  DeviceId::moduleRev() {
    return &buffer[MOD_REV_OFFSET]; 
  }
  inline
  unsigned char * 
  DeviceId::vendorName() {
    return &buffer[VENDOR_OFFSET]; 
  }
  inline
  unsigned char *
  DeviceId::moduleName() {
    return &buffer[MOD_NAME_OFFSET]; 
  }
  inline
  long 
  DeviceId::devNum() {
    return ntohl(*((int*)&buffer[DEV_NUM_OFFSET])); 
  }

  //---------------------------------------------------------------------------
  // DeviceProtocol inlines
  //---------------------------------------------------------------------------
  inline
  char * 
  DeviceProtocol::prot() {
    return &buffer[PROT_OFFSET]; 
  }
  inline
  char * 
  DeviceProtocol::type() {
    return &buffer[TYPE_OFFSET]; 
  }
  inline
  char * 
  DeviceProtocol::model() {
    return &buffer[MODEL_OFFSET]; 
  }

  //---------------------------------------------------------------------------
  // DeviceLongId inlines
  //---------------------------------------------------------------------------
  inline
  DeviceId&
  DeviceLongId::devId() {
    return *(DeviceId*)       &buffer[ID_OFFSET]; 
  }
  inline
  DeviceProtocol&
  DeviceLongId::devProtocol() {
    return *(DeviceProtocol*) &buffer[PROTOCOL_OFFSET]; 
  }
  inline
  DeviceStatus&
  DeviceLongId::devStatus() {
    return *(DeviceStatus*)   &buffer[STATUS_OFFSET]; 
  }
  inline
  const DeviceId&
  DeviceLongId::devId() const { 
    return *(DeviceId*)       &buffer[ID_OFFSET]; 
  }
  inline
  const DeviceProtocol&
  DeviceLongId::devProtocol() const { 
    return *(DeviceProtocol*) &buffer[PROTOCOL_OFFSET]; 
  }
  inline
  DeviceStatus  
  DeviceLongId::devStatus() const { 
    return *(DeviceStatus*)   &buffer[STATUS_OFFSET]; 
  }

  //---------------------------------------------------------------------------
  // global helper functions
  //---------------------------------------------------------------------------
  inline
  long 
  devId2devNum(unsigned char index) {
    return deviceTable[index].longId.devId().devNum();
  }
  inline
  unsigned char 
  devNum2devId (long mspNum) {
    for (int i = 1; i < 256; ++i) {
      if ((deviceTable[i].state != NOT)
	  && (deviceTable[i].longId.devId().devNum() == mspNum)) 
	return (unsigned char) i;
    }
    return 0;
  }
};

#endif




