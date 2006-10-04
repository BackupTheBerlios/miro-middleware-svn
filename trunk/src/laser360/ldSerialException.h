
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldSerialException.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LD_SERIAL_EXCEPTION_H_
  #define _LD_SERIAL_EXCEPTION_H_

  #include <string>
  #include "ldBaseException.h"

namespace ldoem_
{

  //---------------------------------------------------------------------------
  ///
  /// SerialCom Exception class
  class SerialException : public BaseException
  {
  public:
    //---------------------------------------------------------------------------
    /// constructor
    ///
    /// @param msg Errortext that you can get afterwards with getMessage()
    SerialException( const std::string msg ) : BaseException( "SerialComExcpetion: " + msg )
    {
    };

  };
}; // end namespace ldoem_

#endif // _LD_Serial_EXCEPTION_H_
