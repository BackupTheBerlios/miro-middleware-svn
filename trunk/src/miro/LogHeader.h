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

#include <ace/CDR_Stream.h>

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

    static unsigned long const PROTOCOL_ID = 0x474f4c4d;      // "MLOG";
    static unsigned short const PROTOCOL_VERSION = 0x0002;
    static unsigned short const MAX_VERSION = 0x0002;

    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    LogHeader(WRITE);
    LogHeader(READ) throw (EFileType, EVersion);
    
    //--------------------------------------------------------------------------
    // public data
    //--------------------------------------------------------------------------

    unsigned long id;
    unsigned short version;
    unsigned short byteOrder;
  };

  inline
  LogHeader::LogHeader(WRITE) :
    id(PROTOCOL_ID),
    version(PROTOCOL_VERSION),
    byteOrder(ACE_CDR_BYTE_ORDER)
  {}
}
#endif // miro_LogHeader_h
