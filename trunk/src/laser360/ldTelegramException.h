
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldTelegramException.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------


#ifndef _LD_TELEGRAM_EXCEPTION_H_
  #define _LD_TELEGRAM_EXCEPTION_H_

  #include <string>
  #include "ldBaseException.h"

namespace ldoem_
{

  //---------------------------------------------------------------------------
  /// Telegram Exception class
  class TelegramException : public BaseException
  {
  public:
    //---------------------------------------------------------------------------
    /// constructor
    /// @param msg Errortext that you can get afterwards with getMessage()
    TelegramException( const std::string msg ) : BaseException( msg )
    {
    };

  };
}; // end namespace ldoem_

#endif // TELEGRAM_EXCEPTION_H_
