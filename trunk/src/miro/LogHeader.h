// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miro_LogHeader_h
#define miro_LogHeader_h

#include "Exception.h"

#include <ace/Basic_Types.h>

namespace Miro
{
  struct LogHeader
  {
    //--------------------------------------------------------------------------
    // public types
    //--------------------------------------------------------------------------
    MIRO_EXCEPTION_TYPE(EVersion);
    MIRO_EXCEPTION_TYPE(EFileType);

    struct READ {};
    struct WRITE {};

    //--------------------------------------------------------------------------
    // public constants
    //--------------------------------------------------------------------------

    static ACE_UINT32 const PROTOCOL_ID = 0x474f4c4d;      // "MLOG";
    static ACE_UINT16 const PROTOCOL_VERSION = 0x0003;
    static ACE_UINT16 const MAX_VERSION = 0x0003;

    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    LogHeader(WRITE);
    LogHeader(READ) throw (EFileType, EVersion);
    
    //--------------------------------------------------------------------------
    // public data
    //--------------------------------------------------------------------------

    ACE_UINT32 id;
    ACE_UINT16 version;
    ACE_UINT16 byteOrder;
  };
}
#endif // miro_LogHeader_h
